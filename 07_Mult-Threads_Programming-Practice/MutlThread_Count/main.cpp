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
	DWORD* pCountBuffer;			// �����庰�� ����� ������ ����
	DWORD dwMaxBufferCount;	// �����庰�� ����� ������ ������ ���� �ִ� ����
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

	// ���� �����尡  0�� core���� ����ǵ��� ��ȣ�� ����
	//DWORD_PTR	ThreadAffinityMask = 1;
	//DWORD_PTR	OldThreadAffinityMask = SetThreadAffinityMask(hMainThread, ThreadAffinityMask);

	DWORD dwWorkerThreadCount = 0;

	srand(1);
	DWORD dwChrCount = 'z' - 'a' + 1;		// a - z���� ���ĺ� ����

	// �Է� ������ ����. a - z���� �����ϰ� ���ڸ� ��ġ
	g_pDataList = new BYTE[MAX_DATA_COUNT];
	for (DWORD i = 0; i < MAX_DATA_COUNT; i++)
	{
		g_pDataList[i] = (BYTE)(rand() % dwChrCount) + 'a';
	}

	// ������� ����
	DWORD* pCounResultList = new DWORD[dwChrCount];	// a - z ������ �� ���ں� ������ �� �迭

	// ��� ���� �ʱ�ȭ
	memset(pCounResultList, 0, sizeof(DWORD) * dwChrCount);

	LARGE_INTEGER PrvCounter = QCGetCounter();

	// �̱۽������ ���� ���� ī��Ʈ
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

	// ��� �����尡 �Ϸ������ üũ�ϱ� ���� �̺�Ʈ �迭
	g_phCompleteEventList = new HANDLE[dwWorkerThreadCount];

	// �����忡 �����͸� ������ ����ü ����
	THREAD_ARG* pArgList = new THREAD_ARG[dwWorkerThreadCount];
	memset(pArgList, 0, sizeof(THREAD_ARG) * dwWorkerThreadCount);

	DWORD dwItemCountPerThread = (MAX_DATA_COUNT / dwWorkerThreadCount);
	DWORD dwItemCountExt = (MAX_DATA_COUNT % dwWorkerThreadCount);
	
	// ������ ���� �� �̺�Ʈ ��ü ����
	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{	
		g_phCompleteEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);	// worker�������� �۾��� �Ϸ�Ǿ����� �ñ׳ε� �̺�Ʈ

		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			pArgList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		pArgList[i].dwThreadIndex = i;
		pArgList[i].dwStartIndex = i * dwItemCountPerThread;
		pArgList[i].dwItemCount = dwItemCountPerThread;
		pArgList[i].pCountBuffer = new DWORD[dwChrCount];	// a - z ������ �� ���ں� ������ �� �迭
		memset(pArgList[i].pCountBuffer, 0, sizeof(DWORD) * dwChrCount);
		pArgList[i].dwMaxBufferCount = dwChrCount;

		UINT ThreadID = 0;
		pArgList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, pArgList + i, 0, &ThreadID);
	}
	// ������ �����忡 ������ ���� �߰�
	pArgList[dwWorkerThreadCount-1].dwItemCount += dwItemCountExt;

	// ��� ���� �ʱ�ȭ
	memset(pCounResultList, 0, sizeof(DWORD) * dwChrCount);

	// ��Ƽ������� ���� ���� ī��Ʈ. ��� worker thread�� �۾��� �����ϵ��� ����
	PrvCounter = QCGetCounter();

	for (DWORD i = 0; i < dwWorkerThreadCount; i++)
	{
		SetEvent(pArgList[i].hEventList[THREAD_EVENT_PROCESS]);
	}

	// ��� worker thread�� �۾��� �Ϸ��� ������ ���
	WaitForMultipleObjects(dwWorkerThreadCount, g_phCompleteEventList, TRUE, INFINITE);

	// �����庰�� ī������ ������� �����Ͽ� ���� ����� �ջ�
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
	// �־��� �迭 �ȿ��� 'a' - 'a'������ ���ڰ� ���� �� ������ ã�Ƽ� pdwOutResultBuffer[]�� ���� 'a' -> 0, 'z' -> 25
	// single-thread, multi-threads ���������� ���
	
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