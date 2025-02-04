// AwakehreadTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// ���� ������ �۾��� ��Ƽ������� ó���Ϸ��� ��� ��Ŀ�����尡 ��������� �ɸ��� �ð�, ��Ŀ�������� �۾��ϷḦ ����ϴ� �ð��� �߰��ȴ�.
// ���������� �� ������ ������ �̱� �����庸�� ó���ð��� ������� ��찡 ����. ���伺 vs ó����
// �̸� �׽�Ʈ�ϱ� ���� ������Ʈ�̴�.
// N���� �迭�� ��� �Է°� q�� ���� Process()�Լ��� �����ϸ� 
// sin(q)�� ���� ���� ������ �۾��� ��� �̱۽����� ó���� �ξ� ������.
// Process()�Լ��� ó���� �ξ� ������ ����� ��Ƽ ������ ó���� �� ������.(����ǲ���� ������ ���� ���伺 ���)

#include "pch.h"
#include <Windows.h>
#include <process.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include "../../Util/QueryPerfCounter.h"

enum THREAD_EVENT_INDEX
{
	THREAD_EVENT_INDEX_PROCESS,
	THREAD_EVENT_INDEX_DESTROY,
	THREAD_EVENT_INDEX_COUNT
};

struct THREAD_DESC
{
	HANDLE hThread;
	HANDLE hCompleted;
	DWORD dwThreadIndex;
	HANDLE hEventList[THREAD_EVENT_INDEX_COUNT];
};
struct ITEM
{
	float q;
	float r;
	DWORD dwCount;
};

HANDLE* g_phCompleteEventList = nullptr;

DWORD g_dwThreadNum = 0;
THREAD_DESC* g_pThreadDescList = nullptr;
LONG volatile g_lProcessdCount = 0;

void CreateWorkerThreads(DWORD dwThreadCount);
void DestroyAllWorkerThreads();
UINT WINAPI ThreadFunc(void* pVoid);

float Process(float q, DWORD dwCount);
void ProcessByWorkerThread(DWORD dwThreadIndex);

ITEM* g_pItemList = nullptr;

const DWORD MAX_WORKER_THREAD_COUNT = 8;
const DWORD TEST_ITEM_COUNT = 10000;

#ifndef _DEBUG
#pragma optimize("gpsy",off)
#endif


#define USE_MULTIPLE_OBJECTS_FOR_CHECK_COMPLETED
int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	QCInit();

	// ó���� ���ҵ�(�Է°�)�� ����
	srand(1);
	g_pItemList = new ITEM[TEST_ITEM_COUNT];
	for (DWORD i = 0; i < TEST_ITEM_COUNT; i++)
	{
		g_pItemList[i].q = (float)(rand() % 100) * 0.01f;
		g_pItemList[i].r = 0.0f;
	}

	// ���� ����
	for (DWORD i = 0; i < TEST_ITEM_COUNT; i++)
	{
		g_pItemList[i].dwCount = (rand() % 100) + 1 + 100;
	}
	

	// ��Ŀ ������ ���� �� �ʱ�ȭ    
	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	DWORD dwWorkerThreadCount = sysInfo.dwNumberOfProcessors;
	if (dwWorkerThreadCount > MAX_WORKER_THREAD_COUNT)
		dwWorkerThreadCount = MAX_WORKER_THREAD_COUNT;

	CreateWorkerThreads(dwWorkerThreadCount);

	//wprintf_s(L"Press Any key.\n");
	//_getch();

	// �׽�Ʈ ����
	// single thread ////////////////////////////////////////////////////////////////////
	wprintf_s(L"Single Thread\n");

	LARGE_INTEGER PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < TEST_ITEM_COUNT; i++)
	{
		g_pItemList[i].r = Process(g_pItemList[i].q, g_pItemList[i].dwCount);
	}
	float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	wprintf_s(L"%.2f-%.2f ", g_pItemList[0].q, g_pItemList[0].r);
	wprintf_s(L"%.2f-%.2f ", g_pItemList[TEST_ITEM_COUNT / 2].q, g_pItemList[TEST_ITEM_COUNT / 2].r);
	wprintf_s(L"%.2f-%.2f ", g_pItemList[TEST_ITEM_COUNT - 1].q, g_pItemList[TEST_ITEM_COUNT - 1].r);
	//  for (DWORD i = 0; i < TEST_ITEM_COUNT; i++)
	//  {
		  //wprintf_s(L"%.2f-%.2f ", g_pItemList[i].q, g_pItemList[i].r);
	//  }
	wprintf_s(L"\n");
	wprintf_s(L"%f ms elasped.\n", fElapsedTick);
	wprintf_s(L"\n");

	/////////////////////////////////////////////////////////////////////////////////////


	// multi thread /////////////////////////////////////////////////////////////////////
	wprintf_s(L"Multi Threads : %u\n", dwWorkerThreadCount);

	PrvCounter = QCGetCounter();
	g_lProcessdCount = 0;
	// ������Ǯ�� ������� ��Ŀ��������� ����
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		SetEvent(g_pThreadDescList[i].hEventList[THREAD_EVENT_INDEX_PROCESS]);
	}

#ifdef USE_MULTIPLE_OBJECTS_FOR_CHECK_COMPLETED
	WaitForMultipleObjects(dwWorkerThreadCount, g_phCompleteEventList, TRUE, INFINITE);
#else
	// ��Ŀ �����尡 �۾��� �Ϸ��ϱ⸦ ��ٸ�
	for (DWORD i = 0; i < WORKER_THREAD_COUNT; i++)
	{
		WaitForSingleObject(g_pThreadDescList[i].hCompleted, INFINITE);
	}
#endif



	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

	wprintf_s(L"%.2f-%.2f ", g_pItemList[0].q, g_pItemList[0].r);
	wprintf_s(L"%.2f-%.2f ", g_pItemList[TEST_ITEM_COUNT / 2].q, g_pItemList[TEST_ITEM_COUNT / 2].r);
	wprintf_s(L"%.2f-%.2f ", g_pItemList[TEST_ITEM_COUNT - 1].q, g_pItemList[TEST_ITEM_COUNT - 1].r);
	//  for (DWORD i = 0; i < TEST_ITEM_COUNT; i++)
	//  {
		  //wprintf_s(L"%.2f-%.2f ", g_pItemList[i].q, g_pItemList[i].r);
	//  }
	wprintf_s(L"\n");
	wprintf_s(L"%f ms elasped.\n", fElapsedTick);
	wprintf_s(L"\n");

	/////////////////////////////////////////////////////////////////////////////////////

	DestroyAllWorkerThreads();

	if (g_pItemList)
	{
		delete[] g_pItemList;
		g_pItemList = nullptr;
	}

	wprintf_s(L"Press Any key.\n");
	_getch();
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;
}
#ifndef _DEBUG
#pragma optimize("gpsy",on)
#endif
void CreateWorkerThreads(DWORD dwThreadCount)
{
	g_pThreadDescList = new THREAD_DESC[dwThreadCount];
	memset(g_pThreadDescList, 0, sizeof(THREAD_DESC) * dwThreadCount);
	g_dwThreadNum = dwThreadCount;

	g_phCompleteEventList = new HANDLE[dwThreadCount];

	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		g_pThreadDescList[i].dwThreadIndex = i;

		for (DWORD j = 0; j < THREAD_EVENT_INDEX_COUNT; j++)
		{
			g_pThreadDescList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		g_pThreadDescList[i].hCompleted = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		g_phCompleteEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);


		UINT uiThreadID = 0;
		g_pThreadDescList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, g_pThreadDescList + i, 0, &uiThreadID);
	}
}

void DestroyAllWorkerThreads()
{
	for (DWORD i = 0; i < g_dwThreadNum; i++)
	{
		SetEvent(g_pThreadDescList[i].hEventList[THREAD_EVENT_INDEX_DESTROY]);
		WaitForSingleObject(g_pThreadDescList[i].hThread, INFINITE);
		CloseHandle(g_pThreadDescList[i].hThread);
		g_pThreadDescList[i].hThread = nullptr;

		for (DWORD j = 0; j < THREAD_EVENT_INDEX_COUNT; j++)
		{
			CloseHandle(g_pThreadDescList[i].hEventList[j]);
			g_pThreadDescList[i].hEventList[j] = nullptr;
		}
		CloseHandle(g_pThreadDescList[i].hCompleted);
		g_pThreadDescList[i].hCompleted = nullptr;

		CloseHandle(g_phCompleteEventList[i]);
		g_phCompleteEventList[i] = nullptr;
	}
	delete[] g_phCompleteEventList;
	g_phCompleteEventList = nullptr;

	if (g_pThreadDescList)
	{
		delete[] g_pThreadDescList;
		g_pThreadDescList = nullptr;
	}

}
float Process(float q, DWORD dwCount)
{
	// �� �׽�Ʈ���� �۾�(��) �����ϴ� �Լ�
	float r = sinf(q);
	for (DWORD i = 0; i < dwCount; i++)
	{
		r = sinf(r);
	}
	return r;
}

void ProcessByWorkerThread(DWORD dwThreadIndex)
{
	while (1)
	{
		LONG lCurCount = _InterlockedIncrement(&g_lProcessdCount);
		lCurCount--;
		if (lCurCount >= TEST_ITEM_COUNT)
		{
			_InterlockedDecrement(&g_lProcessdCount);
			break;
		}

		// processing
		g_pItemList[lCurCount].r = Process(g_pItemList[lCurCount].q, g_pItemList[lCurCount].dwCount);
	}

#ifdef USE_MULTIPLE_OBJECTS_FOR_CHECK_COMPLETED
	SetEvent(g_phCompleteEventList[dwThreadIndex]);
#else
	SetEvent(g_pThreadDescList[dwThreadIndex].hCompleted);
#endif

}
UINT WINAPI ThreadFunc(void* pArgs)
{
	THREAD_DESC* pThreadDesc = (THREAD_DESC*)pArgs;
	while (1)
	{
		DWORD dwEventIndex = WaitForMultipleObjects(THREAD_EVENT_INDEX_COUNT, pThreadDesc->hEventList, FALSE, INFINITE);
		switch (dwEventIndex)
		{
			case THREAD_EVENT_INDEX_PROCESS:
				ProcessByWorkerThread(pThreadDesc->dwThreadIndex);
				break;
			case THREAD_EVENT_INDEX_DESTROY:
				goto lb_exit;
		}
	}
lb_exit:
	_endthreadex(0);
	return 0;
}