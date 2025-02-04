// VirtuaAlloc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>


int main()
{
	// alloc and reserve 1MB
	DWORD* pCommittedMemory = (DWORD*)VirtualAlloc(NULL, 1 * 1024 * 1024, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	
	for (DWORD i = 0; i < 1024 * 1024; i++)
	{
		char chr_index = (char)(i % 26);
		*((char*)pCommittedMemory + i) = 'a' + chr_index;
	}
	
	wprintf(L"Committed Memory Address: %p\n", pCommittedMemory);
	wprintf(L"Press any key\n");
	_getch();

	// 메모리 속성을 쓰기 금지로 변경
	wprintf(L"Trying change Memory's property to read-only: %p\n", pCommittedMemory);
	DWORD dwOldProtect; 
	if (!VirtualProtect(pCommittedMemory, 1024 * 1024, PAGE_READONLY, &dwOldProtect))
	{
		__debugbreak();
	}
	wprintf(L"Committed Memory Address: %p changed to readonly.\n", pCommittedMemory);
	wprintf(L"Press any key\n");
	_getch();
	//pCommittedMemory[0] = 100;

	// 메모리 속성을 읽기/쓰기 금지로 변경
	wprintf(L"Trying change Memory's property to no-access: %p\n", pCommittedMemory);
	if (!VirtualProtect(pCommittedMemory, 1024 * 1024, PAGE_NOACCESS, &dwOldProtect))
	{
		__debugbreak();
	}
	wprintf(L"Committed Memory Address: %p changed to No-Access.\n", pCommittedMemory);
	wprintf(L"Press any key\n");
	_getch();
		
	//DWORD value = pCommittedMemory[0];

	wprintf(L"Try Free Memory Address: %p changed to readonly.\n", pCommittedMemory);
	// 
	// free memory
	if (!VirtualFree(pCommittedMemory, 0, MEM_RELEASE))
	{
		__debugbreak();
	}
	wprintf(L"Committed Memory Address: %p freeed.\n", pCommittedMemory);
	wprintf_s(L"Program finished.\nPress any key.\n");
	_getch();
	return 0;
}

