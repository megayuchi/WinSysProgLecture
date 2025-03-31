// AtomicLinkedList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include "LinkedList.h"

UINT WINAPI ThreadFunc(void* pArg);

enum THREAD_EVENT
{
	THREAD_EVENT_PROCESS,
	THREAD_EVENT_DESTROY,
	THREAD_EVENT_COUNT
};

struct THREAD_DESC
{
	HANDLE hThread;
	LINK* pLinkMemoryPoolEntry;
	DWORD dwItemStartIndex;
	DWORD dwItemCount;
	HANDLE hCompleteEvent;
	HANDLE hEventList[THREAD_EVENT_COUNT];
};

volatile LINK* g_pLinkHead = nullptr;

const DWORD TEST_ITEM_COUNT = 10000;
const DWORD THREAD_COUNT = 4;

int main()
{
	// 싱글 링크드 리스트에 1부터 10000까지 숫자를 담은 링크노드를 삽입
	// 멀티스레드로 atomic operation만을 사용해서 스레드 세이프하게 삽입
	// 모든 스레드가 삽입 후 최종적으로 메인 스레드가 링크드 리스트를 순회하며 각 노드의 값을 합산
	// 링크 개수가 정확히 10000인지, 합계가 50005000인지 확인
	
	
	
	// 삽입할 링크 노드 메모리 초기화
	// 1 ~ TEST_ITEM_COUNT , EX) 1부터 100까지 합 등으로 적용
	LINK* pLinkMemoryPool = new LINK[TEST_ITEM_COUNT];
	memset(pLinkMemoryPool, 0, sizeof(LINK) * TEST_ITEM_COUNT);

	for (DWORD i = 0; i < TEST_ITEM_COUNT; i++)
	{
		pLinkMemoryPool[i].Value = i + 1;
	}

	// 스레드 생성 및 초기 데이터 전달
	
	THREAD_DESC pThreadDescList[THREAD_COUNT] = {};

	HANDLE hCompleteEvent[THREAD_COUNT] = {};
	for (DWORD i = 0; i < THREAD_COUNT; i++)
	{
		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			pThreadDescList[i].hEventList[j] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		hCompleteEvent[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		pThreadDescList[i].hCompleteEvent = hCompleteEvent[i];
		pThreadDescList[i].pLinkMemoryPoolEntry = pLinkMemoryPool;

		UINT uiThreadID = 0;
		pThreadDescList[i].hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, pThreadDescList + i, 0, &uiThreadID);
	}

	// 스레드당 처리할 노드의 개수와 시작 인덱스를 설정
	DWORD dwItemCountPerThread = TEST_ITEM_COUNT / THREAD_COUNT;
	DWORD dwItemStartIndex = 0;
	for (DWORD i = 0; i < THREAD_COUNT; i++)
	{
		pThreadDescList[i].dwItemStartIndex = dwItemStartIndex;
		pThreadDescList[i].dwItemCount = dwItemCountPerThread;
		dwItemStartIndex += dwItemCountPerThread;
	}
	// TEST_ITEM_COUNT가 스레드 개수로 나누어떨어지지 않는 경우 남는 아이템 개수는 마지막 스레드에 추가해준다.
	pThreadDescList[THREAD_COUNT - 1].dwItemCount += (TEST_ITEM_COUNT % THREAD_COUNT);

	wprintf_s(L"begin working.\n");
	// 각 스레드들이 작업을 시작하도록 깨움.
	for (DWORD i = 0; i < THREAD_COUNT; i++)
	{
		SetEvent(pThreadDescList[i].hEventList[THREAD_EVENT_PROCESS]);
	}

	// 모든 스레드들이 작업을 완료하기를 대기
	WaitForMultipleObjects(THREAD_COUNT, hCompleteEvent, TRUE, INFINITE);
	wprintf_s(L"All threads complelted.\n");


	// verify results
	LINK* pCur = (LINK*)g_pLinkHead;
	UINT64 Sum = 0;
	UINT LinkCount = 0;
	while (pCur)
	{
		Sum += pCur->Value;
		LinkCount++;
		pCur = pCur->pNext;
	}

	wprintf_s(L"Worker Threads : %u, Number of Links : %u, Sum = %I64u\n", THREAD_COUNT, LinkCount, Sum);
	wprintf_s(L"Program finished. Press any key.\n");

	// Destroy all threads.
	for (DWORD i = 0; i < THREAD_COUNT; i++)
	{
		SetEvent(pThreadDescList[i].hEventList[THREAD_EVENT_DESTROY]);
		WaitForSingleObject(pThreadDescList[i].hThread, INFINITE);
		CloseHandle(pThreadDescList[i].hThread);
		pThreadDescList[i].hThread = nullptr;
		for (DWORD j = 0; j < THREAD_EVENT_COUNT; j++)
		{
			CloseHandle(pThreadDescList[i].hEventList[j]);
			pThreadDescList[i].hEventList[j] = nullptr;
		}
		CloseHandle(hCompleteEvent[i]);
		hCompleteEvent[i] = nullptr;
	}

	if (pLinkMemoryPool)
	{
		delete[] pLinkMemoryPool;
		pLinkMemoryPool = nullptr;
	}

	_getch();
}

UINT WINAPI ThreadFunc(void* pArg)
{
	THREAD_DESC* pDesc = (THREAD_DESC*)pArg;
	DWORD dwItemCountPerThread = pDesc->dwItemCount;
	LINK* pLinkMemoryEntryPerThread = (pDesc->pLinkMemoryPoolEntry + pDesc->dwItemStartIndex);
	HANDLE* phEventList = pDesc->hEventList;
	HANDLE hCompleteEvent = pDesc->hCompleteEvent;

	while (1)
	{
		DWORD dwEventIndex = WaitForMultipleObjects(THREAD_EVENT_COUNT, phEventList, FALSE, INFINITE);
		switch (dwEventIndex)
		{
			case THREAD_EVENT_PROCESS:
				{
					for (DWORD i = 0; i < dwItemCountPerThread; i++)
					{
						LINK* pLink = pLinkMemoryEntryPerThread + i;
						LinkToLinkedList(&g_pLinkHead, pLink);
					}
				}
				SetEvent(hCompleteEvent);
				break;
			case THREAD_EVENT_DESTROY:
				goto lb_exit;
		}
	}
lb_exit:
	_endthreadex(0);
	return 0;
}