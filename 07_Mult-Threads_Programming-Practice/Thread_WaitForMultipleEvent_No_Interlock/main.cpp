// ThreadBasic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>


enum THREAD_EVENT
{
	THREAD_EVENT_PROCESS,
	THREAD_EVENT_DESTROY,
	THREAD_EVENT_COUNT
};
struct THREAD_ARG
{
	HANDLE hThread;
	HANDLE hEventList[THREAD_EVENT_COUNT];
	DWORD dwThreadIndex;
	DWORD dwMaxCount;
};

LONG volatile g_lCount = 0;
HANDLE* g_phCompleteEventList = nullptr;

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

	g_phCompleteEventList = new HANDLE[dwWorkerThreadCount];

	// ������ ���� �� �̺�Ʈ ��ü ����
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{	
		g_phCompleteEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);	// worker�������� �۾��� �Ϸ�Ǿ����� �ñ׳ε� �̺�Ʈ

		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			pArgList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		pArgList[i].dwThreadIndex = i;
		pArgList[i].dwMaxCount = MAX_INCREASE_COUNT;

		UINT ThreadID = 0;
		pArgList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, pArgList + i, 0, &ThreadID);
	}
	// Handshaking
	// Main Thread       worker threads
	// [pass 0]
	//    run                wait
	//    SetEvent()		 wait
	//	  wait				 awake
	//	  wait				 run
	//	  wait				 SetEvent()
	//	  awake				 wait
	// [pass 1]
	//	  run				 wait
	//    SetEvent()		 wait
	//	  wait				 awake
	//	  wait				 run
	//	  wait				 SetEvent()
	//	  awake				 wait
	// 
	// (��� �����带 ������ ī��Ʈ-�Ϸ�ó��)x4ȸ ����
	// �����Ӵ� MAX_PASS_COUNTȸ��ŭ ��Ŀ�����忡 �۾��� ��ų �� �����带 ���� �����ߴٸ�? SetEvent()/WaitFor...()�� ����� ��� vs _beginthreadex()���

	for (DWORD i = 0; i < MAX_PASS_COUNT; i++)
	{
		// worker�����忡 �۾�����
		wprintf_s(L"Begin Worker threads. pass:[%u]\n", i);

		// ��� worker thread�� �۾��� �����е��� ����
		for (DWORD j = 0; j < dwWorkerThreadCount; j++)
		{
			SetEvent(pArgList[j].hEventList[THREAD_EVENT_PROCESS]);
		}

		// ��� worker thread�� �۾��� �Ϸ��� ������ ���
		WaitForMultipleObjects(dwWorkerThreadCount, g_phCompleteEventList, TRUE, INFINITE);
	}
	wprintf_s(L"Total Count:%u\n", g_lCount);

	// ������ ���� ����
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		SetEvent(pArgList[i].hEventList[THREAD_EVENT_DESTROY]);
	}

	
	if (pArgList)
	{
		// �� �����尡 ������ �����Ҷ����� ���
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			WaitForSingleObject(pArgList[i].hThread, INFINITE);
			CloseHandle(pArgList[i].hThread);
			for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
			{
				CloseHandle(pArgList[i].hEventList[j]);
				pArgList[i].hEventList[j] = nullptr;
			}
			CloseHandle(g_phCompleteEventList[i]);
			g_phCompleteEventList[i] = nullptr;
		}
		delete[] pArgList;
		pArgList = nullptr;
	}
	if (g_phCompleteEventList)
	{
		delete[] g_phCompleteEventList;
		g_phCompleteEventList = nullptr;
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

	while (1)
	{
		DWORD dwEventIndex = WaitForMultipleObjects(THREAD_EVENT_COUNT, pThreadArg->hEventList, FALSE, INFINITE);
		switch (dwEventIndex)
		{
			case THREAD_EVENT_PROCESS:
				{
					for (DWORD i = 0; i < pThreadArg->dwMaxCount; i++)
					{
						g_lCount++;
					}
					wprintf_s(L"[ThreadIndex:%u, ThreadI:%x] Completed.\n", dwThreadIndex, dwThreadID);
					SetEvent(g_phCompleteEventList[dwThreadIndex]);
				}
				break;
			case THREAD_EVENT_DESTROY:
				goto lb_exit;
		}
	}
lb_exit:
	wprintf_s(L"endthread - [ThreadIndex:%u, ThreadI:%u]\n", dwThreadIndex, dwThreadID);
	_endthreadex(0);
	return 0;
}