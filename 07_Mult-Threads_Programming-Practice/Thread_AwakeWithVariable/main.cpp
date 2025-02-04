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

	// 현재 스레드가  0번 core에서 실행되도록 선호도 설정
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
		// worker스레드에 작업시지
		wprintf_s(L"Begin Worker threads. pass:[%u]\n", i);
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			pArgList[i].lStart = 1;	// 이 시점에서 worker스레드가 lStart변수를 읽으려고 할 수 있다. 일단 4bytes align되어있다면 큰 문제는 아니다.
		}
		BOOL bCompleted = FALSE;
		while (!bCompleted)
		{
			DWORD dwCompletedThreadCount = 0;
			for (DWORD i = 0; i < dwWorkerThreadCount; i++)
			{
				dwCompletedThreadCount += (pArgList[i].lCompleted != 0);	 	// 이 시점에서 worker스레드가 lCompleted변수에 써넣으려고 할 수 있다.
				if (dwWorkerThreadCount == dwCompletedThreadCount)
				{
					bCompleted = TRUE;
				}
			}
		}
		wprintf_s(L"End Worker threads. pass:[%u]\n", i);
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			pArgList[i].lCompleted = 0;	// 이 시점에서 worker스레드가 lCompleted변수를 읽으려고 할 수 있다.
		}
	}
	wprintf_s(L"Total Count:%u\n", g_dwCount);

	// 스레드 종료
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		pArgList[i].lAlive = 0;
	}
	// 스레드 함수가 완료하기 전에 아무키를 눌러서 넘어가면 워커스레드들은 비정상 종료됨
	if (pArgList)
	{
		// 각 스레드가 스스로 종료할때까지 대기
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
			pThreadArg->lStart = 0;	// 이 시점에 메인스레드에서 pThreadArg->lStart의 값을 1로 변경할 수도 있다.
			for (DWORD i = 0; i < pThreadArg->dwMaxCount; i++)
			{
				//wprintf_s(L"MT [ThreadIndex:%u] - Count: %u\n", dwThreadIndex, g_dwCount);
				_InterlockedIncrement(&g_dwCount);
				//g_dwCount++;
			}
			wprintf_s(L"[ThreadIndex:%u, ThreadI:%u] Completed.\n", dwThreadIndex, dwThreadID);
			pThreadArg->lCompleted = 1;	// 이 시점에 메인스레드에서 pThreadArg->lCompleted의 값을 읽으려 할 수도 있다.
			
		}
		if (pThreadArg->lFinish)	// 이 시점에 메인스레드에서 lFinish 값을 써넣고 있을 수 있다.
		{
			break;
		}
	}

	wprintf_s(L"endthread - [ThreadIndex:%u, ThreadI:%u]\n", dwThreadIndex, dwThreadID);
	_endthreadex(0);
	return 0;
}