// ThreadBasic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>


struct THREAD_ARG
{
	HANDLE hThread;
	DWORD dwThreadIndex;
	DWORD dwMaxCount;
	LONG lAlive;
	LONG lStart;
	LONG lFinish;
	LONG lCompleted;
};

DWORD  g_dwCount = 0;

UINT __stdcall ThreadFunc(void* pArg);

const DWORD MAX_INCREASE_COUNT = 100000000;
const DWORD MAX_WORKER_THREAD_COUNT = 4;
const DWORD MAX_PASS_COUNT = 4;

int main()
{
	// Initialize memory validation
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	HANDLE hMainThread = GetCurrentThread();

	// ���� �����尡  0�� core���� ����ǵ��� ��ȣ�� ����
	//DWORD_PTR	ThreadAffinityMask = 1;
	//DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hMainThread, ThreadAffinityMask);

	DWORD dwWorkerThreadCount = 0;
	
	// Launch N workr threads
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD dwLogicalCores = sysInfo.dwNumberOfProcessors;

	dwWorkerThreadCount = dwLogicalCores;
	if (dwWorkerThreadCount > MAX_WORKER_THREAD_COUNT)
		dwWorkerThreadCount = MAX_WORKER_THREAD_COUNT;

	THREAD_ARG* pArgList = new THREAD_ARG[dwWorkerThreadCount];
	memset(pArgList, 0, sizeof(THREAD_ARG) * dwWorkerThreadCount);

	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{	
		pArgList[i].dwThreadIndex = i;
		pArgList[i].dwMaxCount = MAX_INCREASE_COUNT;
		pArgList[i].lAlive = 1;		// main thread -> worker thread
		pArgList[i].lStart = 0;		// main thread -> worker thread
		pArgList[i].lFinish = 0;	// main thread -> worker thread
		pArgList[i].lCompleted = 0;	// worker thread -> main thread

		UINT ThreadID = 0;
		pArgList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, pArgList + i, 0, &ThreadID);
	}

	for (DWORD i = 0; i < MAX_PASS_COUNT; i++)
	{	
		// worker�����忡 �۾�����
		wprintf_s(L"Begin Worker threads. pass:[%u]\n", i);
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			pArgList[i].lStart = 1;	// �� �������� worker�����尡 lStart������ �������� �� �� �ִ�. �ϴ� 4bytes align�Ǿ��ִٸ� ū ������ �ƴϴ�.
		}
		BOOL bCompleted = FALSE;
		while (!bCompleted)
		{
			DWORD dwCompletedThreadCount = 0;
			for (DWORD i = 0; i < dwWorkerThreadCount; i++)
			{
				dwCompletedThreadCount += (pArgList[i].lCompleted != 0);	 	// �� �������� worker�����尡 lCompleted������ ��������� �� �� �ִ�.
				if (dwWorkerThreadCount == dwCompletedThreadCount)
				{
					bCompleted = TRUE;
				}
			}
		}
		wprintf_s(L"End Worker threads. pass:[%u]\n", i);
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			pArgList[i].lCompleted = 0;	// �� �������� worker�����尡 lCompleted������ �������� �� �� �ִ�.
		}
	}
	wprintf_s(L"Total Count:%u\n", g_dwCount);

	// ������ ����
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		pArgList[i].lAlive = 0;
	}
	// ������ �Լ��� �Ϸ��ϱ� ���� �ƹ�Ű�� ������ �Ѿ�� ��Ŀ��������� ������ �����
	if (pArgList)
	{
		// �� �����尡 ������ �����Ҷ����� ���
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			WaitForSingleObject(pArgList[i].hThread, INFINITE);
			CloseHandle(pArgList[i].hThread);
			pArgList[i].hThread = nullptr;
		}
		delete[] pArgList;
		pArgList = nullptr;
	}
	wprintf_s(L"Press a any key.\n");
	_getch();

	// check memory validation
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}

UINT __stdcall ThreadFunc(void* pArg)
{
	THREAD_ARG* pThreadArg = (THREAD_ARG*)pArg;

	DWORD dwThreadID = GetCurrentThreadId();
	DWORD dwThreadIndex = pThreadArg->dwThreadIndex;

	
	while (pThreadArg->lAlive)
	{
		if (pThreadArg->lStart)
		{
			pThreadArg->lStart = 0;	// �� ������ ���ν����忡�� pThreadArg->lStart�� ���� 1�� ������ ���� �ִ�.
			for (DWORD i = 0; i < pThreadArg->dwMaxCount; i++)
			{
				//wprintf_s(L"MT [ThreadIndex:%u] - Count: %u\n", dwThreadIndex, g_dwCount);
				_InterlockedIncrement(&g_dwCount);
				//g_dwCount++;
			}
			wprintf_s(L"[ThreadIndex:%u, ThreadI:%u] Completed.\n", dwThreadIndex, dwThreadID);
			pThreadArg->lCompleted = 1;	// �� ������ ���ν����忡�� pThreadArg->lCompleted�� ���� ������ �� ���� �ִ�.
			
		}
		if (pThreadArg->lFinish)	// �� ������ ���ν����忡�� lFinish ���� ��ְ� ���� �� �ִ�.
		{
			break;
		}
	}

	wprintf_s(L"endthread - [ThreadIndex:%u, ThreadI:%u]\n", dwThreadIndex, dwThreadID);
	_endthreadex(0);
	return 0;
}