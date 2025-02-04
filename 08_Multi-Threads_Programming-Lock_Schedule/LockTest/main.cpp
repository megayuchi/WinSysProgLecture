// LockTest.cpp : Defines the entry point for the console application.
//
#include "pch.h"
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <conio.h>
#include "../../Util/QueryPerfCounter.h"
#include "../../Util/SpinLock.h"


UINT WINAPI WorkerThreadFunc(LPVOID pArg);
void TestSpinLockNoYield();
void TestSpinLock();
void TestSRWLock();
void TestCriticalSection();
void TestCriticalSectionAndSpinCount();
void TestMutex();
void TestSemaphore(DWORD dwThreadIndex);

enum THREAD_EVENT
{
	THREAD_EVENT_TEST_MUTEX,
	THREAD_EVENT_TEST_SEMAPHORE,
	THREAD_EVENT_TEST_CSECTION,
	THREAD_EVENT_TEST_CSECTION_SPINCOUNT,
	THREAD_EVENT_TEST_SRWLOCK,
	THREAD_EVENT_TEST_SPINLOCK_NO_YIELD,
	THREAD_EVENT_TEST_SPINLOCK,	
	THREAD_EVENT_DESTROY,
	THREAD_EVENT_COUNT
};

const DWORD MAX_THREAD_COUNT = 4;
//const DWORD TEST_COUNT = 100000000;
const DWORD TEST_COUNT = 10000000;
struct THREAD_CONTEXT
{
	DWORD	dwTestCount;
	DWORD	dwThreadIndex;
	HANDLE	hEventList[THREAD_EVENT_COUNT];
	HANDLE	hCompleteEvent;
};

LONG volatile g_lSpinLock = 0;
SRWLOCK	g_SRWLock = {};
CRITICAL_SECTION	g_cs = {};
CRITICAL_SECTION	g_csSpin = {};
HANDLE g_hMutex = nullptr;
HANDLE g_hSemaphore = nullptr;
DWORD	g_dwCount = 0;


int main()
{
	QCInit();
	//_InterlockedIncrement(&g_lSpinLock);

	HANDLE	hThreadList[MAX_THREAD_COUNT] = {};
	THREAD_CONTEXT	ContextList[MAX_THREAD_COUNT] = {};

	InitializeSRWLock(&g_SRWLock);
	InitializeCriticalSection(&g_cs);
	InitializeCriticalSectionAndSpinCount(&g_csSpin, 1024);
	
	g_hMutex = CreateMutex(nullptr,              // default security attributes
						   FALSE,             // initially not owned
						   nullptr);             // unnamed mutex

	g_hSemaphore = CreateSemaphore(nullptr, 1, 1, nullptr);

	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);

	DWORD	dwThreadCount = sysInfo.dwNumberOfProcessors;
	if (dwThreadCount > MAX_THREAD_COUNT)
	{
		dwThreadCount = MAX_THREAD_COUNT;
	}
	HANDLE	hCompleteEventList[MAX_THREAD_COUNT] = {};


	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		ContextList[i].dwThreadIndex = i;
		ContextList[i].dwTestCount = TEST_COUNT / dwThreadCount;
		if (i == dwThreadCount - 1)
		{
			ContextList[i].dwTestCount += (TEST_COUNT % dwThreadCount);
		}
		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			ContextList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		hCompleteEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		ContextList[i].hCompleteEvent = hCompleteEventList[i];

		UINT	uiThreadID = 0;
		hThreadList[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThreadFunc, ContextList + i, 0, &uiThreadID);
	}
	LARGE_INTEGER	PrvCounter = {};
	float	fElapsedTick = 0.0f;

	// Test SRWLock
	wprintf_s(L"<SRWLock>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_SRWLOCK]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, SRWLock - %.2fms Elapsed.\n", g_dwCount, fElapsedTick);
	//wprintf_s(L"Press any key.\n");
	//_getch();

	// Test SpinLock without yield
	wprintf_s(L"<SpinLock without Yield>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_SPINLOCK_NO_YIELD]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, SpinLock Without Yield- %.2fms Elapsed.\n", g_dwCount, fElapsedTick);
	//wprintf_s(L"Press any key.\n");
	//_getch();

	// Test SpinLock with yield
	wprintf_s(L"<SpinLock with Yield>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_SPINLOCK]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, SpinLock With Yield - %.2fms Elapsed.\n", g_dwCount, fElapsedTick);
	//wprintf_s(L"Press any key.\n");
	//_getch();

	// Test Critical Section
	wprintf_s(L"<CriticalSection>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_CSECTION]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, CriticalSection - %.2fms Elapsed.\n", g_dwCount, fElapsedTick);
	//wprintf_s(L"Press any key.\n");
	//_getch();

	// Test Critical SectionAndSpinCount
	wprintf_s(L"<CriticalSectionAndSpinCount>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_CSECTION_SPINCOUNT]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, CriticalSectionAndSpinCount - %.2fms Elapsed.\n", g_dwCount, fElapsedTick);
	//wprintf_s(L"Press any key.\n");
	//_getch();

	// Test Mutex
	wprintf_s(L"<Mutex>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_MUTEX]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, Mutex - %.2fms Elapsed.\n", g_dwCount, fElapsedTick);

	// Test Semaphore
	wprintf_s(L"<Semaphore>\n");
	g_dwCount = 0;
	PrvCounter = QCGetCounter();
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_TEST_SEMAPHORE]);
	}
	WaitForMultipleObjects(dwThreadCount, hCompleteEventList, TRUE, INFINITE);
	fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
	wprintf_s(L"Result: %u, Semaphore - %.2fms Elapsed.\n", g_dwCount, fElapsedTick);

	//wprintf_s(L"Press any key.\n");
	//_getch();


	

	// finish
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(ContextList[i].hEventList[THREAD_EVENT_DESTROY]);
	}
	WaitForMultipleObjects(dwThreadCount, hThreadList, TRUE, INFINITE);

	// free all reosurces
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			CloseHandle(ContextList[i].hEventList[j]);
			ContextList[i].hEventList[j] = nullptr;
		}
		CloseHandle(hThreadList[i]);
		hThreadList[i] = nullptr;

		CloseHandle(hCompleteEventList[i]);
	}
	wprintf_s(L"Test finished. Press any key.\n");
	_getch();

	DeleteCriticalSection(&g_cs);
	DeleteCriticalSection(&g_csSpin);

	if (g_hMutex)
	{
		CloseHandle(g_hMutex);
		g_hMutex = nullptr;
	}
	if (g_hSemaphore)
	{
		CloseHandle(g_hSemaphore);
		g_hSemaphore = nullptr;
	}
	return 0;
}


void TestSpinLockNoYield()
{
	AcquireSpinLockNoYield(&g_lSpinLock);
	g_dwCount++;
	ReleaseSpinLock(&g_lSpinLock);
}
void TestSpinLock()
{
	AcquireSpinLock(&g_lSpinLock);
	g_dwCount++;
	ReleaseSpinLock(&g_lSpinLock);
}

void TestSRWLock()
{
	AcquireSRWLockExclusive(&g_SRWLock);
	g_dwCount++;
	ReleaseSRWLockExclusive(&g_SRWLock);
}
void TestCriticalSection()
{
	EnterCriticalSection(&g_cs);
	g_dwCount++;
	LeaveCriticalSection(&g_cs);
}
void TestCriticalSectionAndSpinCount()
{
	EnterCriticalSection(&g_csSpin);
	g_dwCount++;
	LeaveCriticalSection(&g_csSpin);
}
void TestMutex()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	g_dwCount++;
	ReleaseMutex(g_hMutex);
}
void TestSemaphore(DWORD dwThreadIndex)
{
	WaitForSingleObject(g_hSemaphore, INFINITE);
	g_dwCount++;
	LONG lPrvCount = 0;
	ReleaseSemaphore(g_hSemaphore, 1, &lPrvCount);
}

UINT WINAPI WorkerThreadFunc(LPVOID pArg)
{
	THREAD_CONTEXT*	pThreadContext = (THREAD_CONTEXT*)pArg;
	DWORD dwThreadIndex = pThreadContext->dwThreadIndex;
	const HANDLE*	phEventList = pThreadContext->hEventList;
	while (1)
	{
		DWORD	dwEventIndex = WaitForMultipleObjects(THREAD_EVENT_COUNT, phEventList, FALSE, INFINITE);
		switch (dwEventIndex)
		{
			case THREAD_EVENT_TEST_SPINLOCK_NO_YIELD:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestSpinLockNoYield();
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_TEST_SPINLOCK:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestSpinLock();
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_TEST_SRWLOCK:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestSRWLock();
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_TEST_CSECTION:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestCriticalSection();
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_TEST_CSECTION_SPINCOUNT:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestCriticalSectionAndSpinCount();
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_TEST_MUTEX:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestMutex();
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_TEST_SEMAPHORE:
				for (DWORD i = 0; i < pThreadContext->dwTestCount; i++)
				{
					TestSemaphore(dwThreadIndex);
				}
				SetEvent(pThreadContext->hCompleteEvent);
				break;
			case THREAD_EVENT_DESTROY:
				goto lb_exit;
		}
	}

lb_exit:
	_endthreadex(0);
	return 0;
}