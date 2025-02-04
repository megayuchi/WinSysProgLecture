// CRTIO.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>

int main()
{
    FILE* fp = nullptr;
    const WCHAR* wchStr = L"Hello World!!! from File";

    if (!_wfopen_s(&fp, L"Test.txt", L"wb"))
    {
        fwrite(wchStr, sizeof(WCHAR), wcslen(wchStr), fp);
        fclose(fp);
        fp = nullptr;
    }

    const DWORD MAX_READ_BUFFER_SIZE = 1024;
    char* pReadBuffer = (char*)malloc(MAX_READ_BUFFER_SIZE);
    memset(pReadBuffer, 0, MAX_READ_BUFFER_SIZE);

    // read data from file , fopen()/fread()
    if (!_wfopen_s(&fp, L"Test.txt", L"rb"))
    {
		size_t ReadSize = fread(pReadBuffer, sizeof(char), MAX_READ_BUFFER_SIZE - 1, fp);
		wprintf_s(L"fread() - %s\n", (WCHAR*)pReadBuffer);
        fclose(fp);
        fp = nullptr;
    }
    
    // read data from file, CreateFile()/ReadFile()
    memset(pReadBuffer, 0, MAX_READ_BUFFER_SIZE);
	HANDLE hFile = CreateFile(L"Test.txt", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) 
    {
        DWORD dwReadBytes = 0;
		if (ReadFile(hFile, pReadBuffer, MAX_READ_BUFFER_SIZE - (sizeof(WCHAR) * 1), &dwReadBytes, NULL))
        {
            wprintf_s(L"ReadFile() - %s\n", (WCHAR*)pReadBuffer);
        }
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
    if (pReadBuffer)
    {
        free(pReadBuffer);
        pReadBuffer = nullptr;
    }
    wprintf_s(L"Press any key.");    
    _getch();
    return 0;
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
