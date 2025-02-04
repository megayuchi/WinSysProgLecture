// ThreadBasic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include "../../Util/QueryPerfCounter.h"


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
	DWORD dwStartIndex;
	DWORD dwItemCount;
	DWORD* pCountBuffer;			// 스레드별로 결과를 저장할 버퍼
	DWORD dwMaxBufferCount;	// 스레드별로 결과를 저장할 버퍼의 원소 최대 개수
};

HANDLE* g_phCompleteEventList = nullptr;
BYTE*	g_pDataList = nullptr;

void CountCharacter(DWORD* pdwOutResultBuffer, DWORD dwMaxResultBufferCount, BYTE* pDataList, DWORD dwDataCount);
UINT __stdcall ThreadFunc(void* pArg);

const DWORD MAX_DATA_COUNT = 100000000;
const DWORD MAX_WORKER_THREAD_COUNT = 128;

int main()
{
	QCInit();

	// Initialize memory validation
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	HANDLE hMainThread = GetCurrentThread();

	// 현재 스레드가  0번 core에서 실행되도록 선호도 설정
	//DWORD_PTR	ThreadAffinityMask = 1;
	//DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hMainThread, ThreadAffinityMask);

	DWORD dwWorkerThreadCount = 0;

	srand(1);
	DWORD dwChrCount = 'z' - 'a' + 1;		// a - z까지 알파벳 개수

	// 입력 데이터 생성. a - z까지 랜덤하게 문자를 배치
	g_pDataList = new BYTE[MAX_DATA_COUNT];
	for (DWORD i = 0; i < MAX_DATA_COUNT; i++)
	{
		g_pDataList[i] = (BYTE)(rand() % dwChrCount) + 'a';
	}

	// 결과버퍼 생성
	DWORD* pCounResultList = new DWORD[dwChrCount];	// a - z 까지의 각 문자별 개수가 들어갈 배열

	// 결과 버퍼 초기화
	memset(pCounResultList, 0, sizeof(DWORD) * dwChrCount);

	LARGE_INTEGER PrvCounter = QCGetCounter();

	// 싱글스레드로 문자 개수 카운트
	CountCharacter(pCounResultList, dwChrCount, g_pDataList, MAX_DATA_COUNT);

	float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	wprintf_s(L"Single-Thread , %.4fms elapsed.\n", fElapsedTick);
	for (DWORD i = 0; i < dwChrCount; i++)
	{
		printf_s("%c:%u\n", i + 'a', pCounResultList[i]);
	}
	wprintf_s(L"-------------------------------------------------------------------------------------\n");

	// Launch N workr threads
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD dwLogicalCores = sysInfo.dwNumberOfProcessors;

	dwWorkerThreadCount = dwLogicalCores;
	if (dwWorkerThreadCount > MAX_WORKER_THREAD_COUNT)
		dwWorkerThreadCount = MAX_WORKER_THREAD_COUNT;

	if (dwWorkerThreadCount > MAX_DATA_COUNT)
		dwWorkerThreadCount = MAX_DATA_COUNT;

	// 모든 스레드가 완료됐음을 체크하기 위한 이벤트 배열
	g_phCompleteEventList = new HANDLE[dwWorkerThreadCount];

	// 스레드에 데이터를 전달할 구조체 생성
	THREAD_ARG* pArgList = new THREAD_ARG[dwWorkerThreadCount];
	memset(pArgList, 0, sizeof(THREAD_ARG) * dwWorkerThreadCount);

	DWORD dwItemCountPerThread = (MAX_DATA_COUNT / dwWorkerThreadCount);
	DWORD dwItemCountExt = (MAX_DATA_COUNT % dwWorkerThreadCount);
	
	// 스레드 생성 및 이벤트 객체 생성
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{	
		g_phCompleteEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);	// worker스레드의 작업이 완료되었을때 시그널될 이벤트

		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			pArgList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		pArgList[i].dwThreadIndex = i;
		pArgList[i].dwStartIndex = i * dwItemCountPerThread;
		pArgList[i].dwItemCount = dwItemCountPerThread;
		pArgList[i].pCountBuffer = new DWORD[dwChrCount];	// a - z 까지의 각 문자별 개수가 들어갈 배열
		memset(pArgList[i].pCountBuffer, 0, sizeof(DWORD) * dwChrCount);
		pArgList[i].dwMaxBufferCount = dwChrCount;

		UINT ThreadID = 0;
		pArgList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, pArgList + i, 0, &ThreadID);
	}
	// 마지막 스레드에 나머지 개수 추가
	pArgList[dwWorkerThreadCount-1].dwItemCount += dwItemCountExt;

	// 결과 버퍼 초기화
	memset(pCounResultList, 0, sizeof(DWORD) * dwChrCount);

	// 멀티스레드로 문자 개수 카운트. 모든 worker thread가 작업을 시작하도록 지시
	PrvCounter = QCGetCounter();

	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		SetEvent(pArgList[i].hEventList[THREAD_EVENT_PROCESS]);
	}

	// 모든 worker thread가 작업을 완료할 때까지 대기
	WaitForMultipleObjects(dwWorkerThreadCount, g_phCompleteEventList, TRUE, INFINITE);

	// 스레드별로 카운팅한 결과값을 취합하여 최종 결과로 합산
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		for (DWORD j = 0; j < dwChrCount; j++)
		{
			pCounResultList[j] += (pArgList[i].pCountBuffer[j]);
		}
	}
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	wprintf_s(L"Multi-Threads(%u) , %.4fms elapsed.\n", dwWorkerThreadCount, fElapsedTick);
	for (DWORD i = 0; i < dwChrCount; i++)
	{
		printf_s("%c:%u\n", i + 'a', pCounResultList[i]);
	}

	// 스레드 종료 지시
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		SetEvent(pArgList[i].hEventList[THREAD_EVENT_DESTROY]);
	}

	
	if (pArgList)
	{
		// 각 스레드가 스스로 종료할때까지 대기
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

			if (pArgList[i].pCountBuffer)
			{
				delete[] pArgList[i].pCountBuffer;
				pArgList[i].pCountBuffer = nullptr;
			}
		}
		delete[] pArgList;
		pArgList = nullptr;
	}
	if (g_phCompleteEventList)
	{
		delete[] g_phCompleteEventList;
		g_phCompleteEventList = nullptr;
	}
	if (g_pDataList)
	{
		delete[] g_pDataList;
		g_pDataList = nullptr;
	}
	if (pCounResultList)
	{
		delete[] pCounResultList;
		pCounResultList = nullptr;
	}
	wprintf_s(L"Press a any key.\n");
	_getch();

	// check memory validation
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}

void CountCharacter(DWORD* pdwOutResultBuffer, DWORD dwMaxResultBufferCount, BYTE* pDataList, DWORD dwDataCount)
{
	// 주어진 배열 안에서 'a' - 'a'까지의 문자가 각각 몇 개인지 찾아서 pdwOutResultBuffer[]에 저장 'a' -> 0, 'z' -> 25
	// single-thread, multi-threads 공통적으로 사용
	
	for (DWORD i = 0; i < dwDataCount; i++)
	{
		BYTE ch = pDataList[i];
		if (ch >= 'a' && ch <= 'z')
		{
			BYTE bIndex = ch - 'a';
			pdwOutResultBuffer[bIndex]++;
		}
		else
		{
			__debugbreak();
		}
	}



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
					CountCharacter(pThreadArg->pCountBuffer, pThreadArg->dwMaxBufferCount, g_pDataList + pThreadArg->dwStartIndex, pThreadArg->dwItemCount);
					SetEvent(g_phCompleteEventList[dwThreadIndex]);
				}
				break;
			case THREAD_EVENT_DESTROY:
				goto lb_exit;
		}
	}
lb_exit:
	_endthreadex(0);
	return 0;
}