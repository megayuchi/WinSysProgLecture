#include "pch.h"
#include <Windows.h>
#include "LinkedList.h"

void LinkToLinkedList(volatile LINK** ppLinkHead, LINK* pLink)
{
	LINK* pOldHead = (LINK*)_InterlockedExchange64((volatile LONG64*)ppLinkHead, (LONG64)pLink);
	pLink->pNext = pOldHead;	// g_pHead�� �ƴ� pLink�� next�� �����Կ� ����.
}