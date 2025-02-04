// KernelObject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
    HANDLE hEvent = CreateEvent(nullptr, FALSE, FALSE, L"EVENT");

    if (hEvent)
    {
        wprintf_s(L"Event Handle :%p\n", hEvent);
        wprintf_s(L"Press any key.\n");
        _getch();
        CloseHandle(hEvent);
        hEvent = nullptr;
    }

    const WCHAR* wchFileName = L"Test.txt";
	HANDLE hFile = CreateFile(wchFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        wprintf_s(L"File Handle :%p\n", hFile);
        wprintf_s(L"Press any key.\n");
        _getch();
        CloseHandle(hFile);
        hFile = nullptr;
    }

    wprintf_s(L"Profram finished. Press any key.\n");
    _getch();
}
