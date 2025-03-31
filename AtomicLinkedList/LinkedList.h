#pragma once

struct LINK
{
	UINT64 Value;
	LINK* pNext;
};

void LinkToLinkedList(volatile LINK** ppLinkHead, LINK* pLink);


