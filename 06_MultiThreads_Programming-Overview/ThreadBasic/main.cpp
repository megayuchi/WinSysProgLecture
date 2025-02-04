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
};
void SingleThreadWhile();
UINT __stdcall MultiThreadWhile(void* pArg);

//const DWORD MAX_INCREASE_COUNT = 1000000;
const DWORD MAX_INCREASE_COUNT = 500000;
const DWORD MAX_THREAD_COUNT = 128;

int main()
{
	// Initialize memory validation
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	wprintf_s(L"Hello World!!!\n");

	HANDLE hMainThread = GetCurrentThread();

	// 현재 스레드가  0번 core에서 실행되도록 선호도 설정
	//DWORD_PTR	ThreadAffinityMask = 1;
	//DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hMainThread, ThreadAffinityMask);

	THREAD_ARG* pArgList = new THREAD_ARG[MAX_THREAD_COUNT];
	memset(pArgList, 0, sizeof(THREAD_ARG) * MAX_THREAD_COUNT);

	DWORD dwWorkerThreadCount = 0;

	
	// Lauch single thread
	//SingleThreadWhile();
	

	// Launch N workr threads
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD dwLogicalCores = sysInfo.dwNumberOfProcessors;

	dwWorkerThreadCount = dwLogicalCores;
	if (dwWorkerThreadCount > MAX_THREAD_COUNT)
		dwWorkerThreadCount = MAX_THREAD_COUNT;

	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		pArgList[i].dwMaxCount = MAX_INCREASE_COUNT / dwWorkerThreadCount;
		pArgList[i].dwThreadIndex = i;

		UINT ThreadID = 0;
		pArgList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, MultiThreadWhile, pArgList + i, 0, &ThreadID);
		wprintf_s(L"Thread(ID:%x, Handle:%p) created.\n", ThreadID, pArgList[i].hThread);

		// 스레드에 이름 지정
		WCHAR wchThreadName[64] = {};
		swprintf_s(wchThreadName, L"WorkerThread[%u]", i);
		SetThreadDescription(pArgList[i].hThread, wchThreadName);
	}

	// 스레드 함수가 완료하기 전에 아무키를 눌러서 넘어가면 워커스레드들은 비정상 종료됨
	wprintf_s(L"wait for all worker threads. Press a any key to cancel.\n");
	_getch();

	// free all resources
	if (pArgList)
	{
		for (DWORD i = 0; i < dwWorkerThreadCount; i++)
		{
			if (pArgList[i].hThread)
			{
				CloseHandle(pArgList[i].hThread);
				pArgList[i].hThread = nullptr;
			}
		}
		delete[] pArgList;
		pArgList = nullptr;
	}

	// check memory validation
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;
}

void SingleThreadWhile()
{
	DWORD Count = 0;
	for (DWORD i = 0; i < MAX_INCREASE_COUNT; i++)
	{
		wprintf_s(L"ST-Count: %u\n", Count);
		Count++;
	}
}

UINT __stdcall MultiThreadWhile(void* pArg)
{
	THREAD_ARG* pThreadArg = (THREAD_ARG*)pArg;

	char* p = (char*)malloc(100);
	//WCHAR* wchThreadName = nullptr;
	//GetThreadDescription(pThreadArg->hThread, &wchThreadName);

	DWORD dwThreadID = GetCurrentThreadId();

	DWORD dwThreadIndex = pThreadArg->dwThreadIndex;
	DWORD dwMaxCount = pThreadArg->dwMaxCount;
	DWORD Count = 0;
	for (DWORD i = 0; i < dwMaxCount; i++)
	{
		//Sleep(1);
		wprintf_s(L"MT [ThreadIndex:%u] - Count: %u\n", dwThreadIndex, Count);
		Count++;
	}
	wprintf_s(L"endthread - [ThreadIndex:%u, ThreadI:%u]\n", dwThreadIndex, dwThreadID);

	if (p)
	{
		free(p);
		p = nullptr;
	}

	_endthreadex(0);
	return 0;
}