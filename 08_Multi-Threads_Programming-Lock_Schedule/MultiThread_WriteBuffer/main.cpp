// MultiThread_WriteBuffer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include "../../Util/SpinLock.h"
#include "../../Util/QueryPerfCounter.h"
#include "WriteBuffer.h"

//const DWORD MAX_THREAD_COUNT = 1;
const DWORD MAX_THREAD_COUNT = 16;
const DWORD MAX_BUFFER_SIZE = 1024u * 1024u * 1000u;
//const DWORD MAX_BUFFER_SIZE = 1024u * 1024u * 100u;

enum BARRIER_METHOD
{
	BARRIER_METHOD_LOCK_BUFFER_CS,
	BARRIER_METHOD_LOCK_BUFFER_SRWLOCK,
	BARRIER_METHOD_LOCK_BUFFER_SPINLOCK,
	BARRIER_METHOD_LOCK_ADDRESS,
	BARRIER_METHOD_COUNT
};
enum THREAD_EVENT
{
	THREAD_EVENT_PROCESS,
	THREAD_EVENT_DESTROY = THREAD_EVENT_PROCESS + BARRIER_METHOD_COUNT,
	THREAD_EVENT_COUNT
};

struct THREAD_DESC
{
	HANDLE	hThread;
	HANDLE phEventList[THREAD_EVENT_COUNT];
	UINT ThreadIndex;
	const BYTE*	pStream;
	DWORD	dwStreamSize;

};

CWriteBuffer* g_pWriteBuffer = nullptr;
LONG volatile g_lActiveThreadCount = 0;

HANDLE g_hCompleteEvent = nullptr;
THREAD_DESC g_pThradDescList[MAX_THREAD_COUNT] = {};


DWORD Init();
UINT64 Test(const BYTE* pStream, DWORD dwStreamSize, DWORD dwThreadCount, BARRIER_METHOD barrierMethod);
void Cleanup();

void CompleteByThread(UINT ThreadIndex);
void ProcessByThread(UINT ThreadIndex, const BYTE* pStream, DWORD dwStreamSize, BARRIER_METHOD barrierMethod);
UINT WINAPI WorkerThread(LPVOID lpVoid);


int main()
{
#ifdef _DEBUG
	int	flags = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flags);
#endif

	// dummy stream
	DWORD	dwStreamSize = MAX_BUFFER_SIZE;
	BYTE*	pStream = new BYTE[dwStreamSize];
	for (DWORD i = 0; i < dwStreamSize; i++)
	{
		pStream[i] = i % 256;
	}


	DWORD dwMaxThreadCount = Init();

	for (DWORD i = 0; i < dwMaxThreadCount; i++)
	{
		DWORD	dwTestThreadCount = i + 1;

		wprintf_s(L"[Thread : %u]\n", dwTestThreadCount);

		LARGE_INTEGER CurCounter, PrvCounter;
		float fElapsedTick;
		UINT64 Sum = 0;

		//
		wprintf_s(L"\t<Lock Buffer - Critical Section>\n");
		PrvCounter = QCGetCounter();
		Sum = Test(pStream, dwStreamSize, dwTestThreadCount, BARRIER_METHOD_LOCK_BUFFER_CS);
		CurCounter = QCGetCounter();

		fElapsedTick = QCMeasureElapsedTick(CurCounter, PrvCounter);
		wprintf_s(L"\tSum = %I64u, Elpased Tick = %.2f\n", Sum, fElapsedTick);
		wprintf_s(L"\n");

		//
		wprintf_s(L"\t<Lock Buffer - SRWLock>\n");
		PrvCounter = QCGetCounter();
		Sum = Test(pStream, dwStreamSize, dwTestThreadCount, BARRIER_METHOD_LOCK_BUFFER_SRWLOCK);
		CurCounter = QCGetCounter();

		fElapsedTick = QCMeasureElapsedTick(CurCounter, PrvCounter);
		wprintf_s(L"\tSum = %I64u, Elpased Tick = %.2f\n", Sum, fElapsedTick);
		wprintf_s(L"\n");

		//
		wprintf_s(L"\t<Lock Buffer - SpinLock>\n");
		PrvCounter = QCGetCounter();
		Sum = Test(pStream, dwStreamSize, dwTestThreadCount, BARRIER_METHOD_LOCK_BUFFER_SPINLOCK);
		CurCounter = QCGetCounter();

		fElapsedTick = QCMeasureElapsedTick(CurCounter, PrvCounter);
		wprintf_s(L"\tSum = %I64u, Elpased Tick = %.2f\n", Sum, fElapsedTick);
		wprintf_s(L"\n");

		//
		wprintf_s(L"\t<Lock Address - SpinLock>\n");
		PrvCounter = QCGetCounter();
		Sum = Test(pStream, dwStreamSize, dwTestThreadCount, BARRIER_METHOD_LOCK_ADDRESS);
		CurCounter = QCGetCounter();

		fElapsedTick = QCMeasureElapsedTick(CurCounter, PrvCounter);
		wprintf_s(L"\tSum = %I64u, Elpased Tick = %.2f\n", Sum, fElapsedTick);
		wprintf_s(L"\n");

		wprintf_s(L"---------------------------------------------------------------\n");
	}
	Cleanup();
	wprintf_s(L"Press any key\n");
	_getch();

	if (pStream)
	{
		delete[] pStream;
		pStream = nullptr;
	}
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

}
UINT64 Test(const BYTE* pStream, DWORD dwStreamSize, DWORD dwThreadCount, BARRIER_METHOD barrierMethod)
{
	g_pWriteBuffer->Clear();

	DWORD	dwSizePerThread = dwStreamSize / dwThreadCount;
	DWORD	dwReaminBytes = dwStreamSize % dwThreadCount;
	
	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		const BYTE*	pStreamEntryPerThread = (pStream + i * dwSizePerThread);
		g_pThradDescList[i].pStream = pStreamEntryPerThread;
		g_pThradDescList[i].dwStreamSize = dwSizePerThread;
	}
	g_pThradDescList[dwThreadCount - 1].dwStreamSize += dwReaminBytes;	// 자투리 바이트 처리

	g_lActiveThreadCount = dwThreadCount;
	THREAD_EVENT eventIndex = (THREAD_EVENT)(THREAD_EVENT_PROCESS + barrierMethod);

	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		SetEvent(g_pThradDescList[i].phEventList[eventIndex]);
	}
	WaitForSingleObject(g_hCompleteEvent, INFINITE);
	g_lActiveThreadCount = 0;

	UINT64 Sum = g_pWriteBuffer->CalcSum();
	return Sum;
}
DWORD Init()
{
	QCInit();

	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	DWORD	dwThreadCount = sysInfo.dwNumberOfProcessors;

	if (dwThreadCount > MAX_THREAD_COUNT)
	{
		dwThreadCount = MAX_THREAD_COUNT;
	}
	
	g_pWriteBuffer = new CWriteBuffer;
	g_pWriteBuffer->Initialize(MAX_BUFFER_SIZE);


	for (DWORD i = 0; i < dwThreadCount; i++)
	{
		g_pThradDescList[i].ThreadIndex = i;
		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			g_pThradDescList[i].phEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		UINT uiThreadID = 0;
		g_pThradDescList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, g_pThradDescList + i, 0, &uiThreadID);
		//wprintf_s(L"beginthread() - TheradID:%u\n", uiThreadID);

	}
	g_hCompleteEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	return dwThreadCount;
}
void Cleanup()
{
	for (DWORD i = 0; i < MAX_THREAD_COUNT; i++)
	{
		SetEvent(g_pThradDescList[i].phEventList[THREAD_EVENT_DESTROY]);
		WaitForSingleObject(g_pThradDescList[i].hThread, INFINITE);

		CloseHandle(g_pThradDescList[i].hThread);
		g_pThradDescList[i].hThread = nullptr;

		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			CloseHandle(g_pThradDescList[i].phEventList[j]);
			g_pThradDescList[i].phEventList[j] = nullptr;
		}
	}
	if (g_hCompleteEvent)
	{
		CloseHandle(g_hCompleteEvent);
		g_hCompleteEvent = nullptr;
	}
	if (g_pWriteBuffer)
	{
		delete g_pWriteBuffer;
		g_pWriteBuffer = nullptr;
	}
}
void CompleteByThread(UINT ThreadIndex)
{
	LONG lCurCount = _InterlockedDecrement(&g_lActiveThreadCount);
	if (0 == lCurCount)
	{
		SetEvent(g_hCompleteEvent);
	}
}
void ProcessByThread(UINT ThreadIndex, const BYTE* pStream, DWORD dwStreamSize, BARRIER_METHOD barrierMethod)
{
	srand(ThreadIndex);

	while (dwStreamSize > 0)
	{
		DWORD dwSize = (DWORD)(rand() % 256) + 1;
		
		if (dwSize > dwStreamSize)
			dwSize = dwStreamSize;

		BOOL	bBufferFull = FALSE;
		DWORD	dwWrittenBytes = 0;
		switch (barrierMethod)
		{
			case BARRIER_METHOD_LOCK_BUFFER_CS:
				dwWrittenBytes = g_pWriteBuffer->PushWithLockBuffer_CS(&bBufferFull, pStream, dwSize);
				break;
			case BARRIER_METHOD_LOCK_BUFFER_SRWLOCK:
				dwWrittenBytes = g_pWriteBuffer->PushWithLockBuffer_SRWLock(&bBufferFull, pStream, dwSize);
				break;
			case BARRIER_METHOD_LOCK_BUFFER_SPINLOCK:
				dwWrittenBytes = g_pWriteBuffer->PushWithLockBuffer_SpinLock(&bBufferFull, pStream, dwSize);
				break;
			case BARRIER_METHOD_LOCK_ADDRESS:
				dwWrittenBytes = g_pWriteBuffer->PushWithLockAddress(&bBufferFull, pStream, dwSize);
				break;
		}
		if (bBufferFull)
			break;
		
		pStream += dwWrittenBytes;
		dwStreamSize -= dwWrittenBytes;
	}
	CompleteByThread(ThreadIndex);
}
UINT WINAPI WorkerThread(LPVOID lpVoid)
{
	DWORD dwThreadID = GetCurrentThreadId();
	//wprintf_s(L"GetCurrentThreadId() - TheradID:%u\n", dwThreadID);
	THREAD_DESC*	pThreadDesc = (THREAD_DESC*)lpVoid;
	
	while (1)
	{
		DWORD	dwEventIndex = WaitForMultipleObjects(THREAD_EVENT_COUNT, pThreadDesc->phEventList, FALSE, INFINITE);
		if (WAIT_FAILED == dwEventIndex)
			goto lb_return;
		
		switch (dwEventIndex)
		{
			case (THREAD_EVENT_PROCESS + BARRIER_METHOD_LOCK_BUFFER_CS):
			case (THREAD_EVENT_PROCESS + BARRIER_METHOD_LOCK_BUFFER_SRWLOCK):
			case (THREAD_EVENT_PROCESS + BARRIER_METHOD_LOCK_BUFFER_SPINLOCK):
			case (THREAD_EVENT_PROCESS + BARRIER_METHOD_LOCK_ADDRESS):
				{
					BARRIER_METHOD barrierMethod = (BARRIER_METHOD)(dwEventIndex - BARRIER_METHOD_LOCK_BUFFER_CS);
					ProcessByThread(pThreadDesc->ThreadIndex, pThreadDesc->pStream, pThreadDesc->dwStreamSize, barrierMethod);
				}
				break;
				
			case THREAD_EVENT_DESTROY:
				goto lb_return;
		}
		
	}

lb_return:
	_endthreadex(0);
	return 0;
}