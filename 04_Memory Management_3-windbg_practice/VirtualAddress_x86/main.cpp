// VirtualAddress_x64.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

int main()
{

#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif
	
	//UINT64 mask = 0b 0000 0000 0000 0000 1111 1111 1111 1111 1111 1111 1111 1111 1111 0000 0000 0000;
	UINT64 AddrInEntryMask = 0b0000000000000000000000000000111111111111111111111111000000000000;

	UINT64 Addr = 0;
	wprintf_s(L"Input Virtual Address:");
	wscanf_s(L"%llx", &Addr);

	DWORD OffsetInPage = (DWORD)(Addr & 0b111111111111);
	DWORD PageTable = (DWORD)((Addr >> 12) & 0b111111111);
	DWORD PageDirectory = (DWORD)((Addr >> 9 >> 12) & 0b111111111);
	DWORD PageDirectoryPointer = (DWORD)((Addr >> 9 >> 9 >> 12) & 0b11);


	wprintf_s(L"Address : %p\n", Addr);
	wprintf_s(L"Page Drectory Pointer : %03x(%d)\n", PageDirectoryPointer, PageDirectoryPointer);
	wprintf_s(L"Page Directory : %03x(%d)\n", PageDirectory, PageDirectory);
	wprintf_s(L"Page Table : %03x(%d)\n", PageTable, PageTable);
	wprintf_s(L"OffsetInPage : %03x(%d)\n", OffsetInPage, OffsetInPage);

	wprintf_s(L"Presss any key.\n");
	_getch();

	

	

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
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
