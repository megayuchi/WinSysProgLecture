// VirtuaAlloc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>


int main()
{
	// alloc and reserve 1MB
	DWORD* pReservedMemory = (DWORD*)VirtualAlloc(NULL, 1 * 1024 * 1024, MEM_RESERVE, PAGE_READWRITE);
	if (!pReservedMemory)
	{
		__debugbreak();
	}
	//pReservedMemory[0] = 100;		// access violation

	// commit 512KB of 1MB
	DWORD* pCommittedMemory = (DWORD*)VirtualAlloc(pReservedMemory, 512 * 1024, MEM_COMMIT, PAGE_READWRITE);
	if (!pCommittedMemory)
	{
		__debugbreak();
	}
	for (DWORD i = 0; i < 512 * 1024; i++)
	{
		char chr_index = (char)(i % 26);
		*((char*)pCommittedMemory + i) = 'a' + chr_index;
	}
	
	wprintf(L"Committed Memory Address: %p\n", pCommittedMemory);
		
	// did't committed Memory yet.
	DWORD* pUnCommittedMemory = pCommittedMemory + 512 * 1024;
	//pUnCommittedMemory[0] = 0;	// access violation
	wprintf(L"Uncommitted address: %p\n", pUnCommittedMemory);
	wprintf(L"Press any key\n");
	_getch();

	// 
	// free memory
	if (!VirtualFree(pCommittedMemory, 0, MEM_RELEASE))
	{
		__debugbreak();
	}
	wprintf(L"Memory Address: %p freeed.\n", pCommittedMemory);
	wprintf_s(L"Program finished.\nPress any key.\n");
	_getch();
	return 0;
}

