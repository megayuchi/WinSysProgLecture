// HeapAlloc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
	HANDLE hHeap = HeapCreate(0, 1024 * 1024, 0);
	char* p = (char*)HeapAlloc(hHeap, 0, 1024 * 1024);
	
	for (DWORD i = 0; i < 1024 * 1024; i++)
	{
		char chr_index = (char)(i % 26);
		*((char*)p + i) = 'a' + chr_index;
	}

	if (p)
	{
		HeapFree(hHeap, 0, p);
		p = nullptr;
	}

	HeapDestroy(hHeap);

	wprintf_s(L"Program finished. Press any key.\n");
	_getch();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
