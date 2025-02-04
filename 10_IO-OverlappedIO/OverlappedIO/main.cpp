// OverlappedIO.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "../../Util/QueryPerfCounter.h"


BOOL TestWriteFile(float* pfOutElapsedTick, const char* pData, DWORD dwSize, const WCHAR* wchFileName);
BOOL TestWriteFileOverlapped(float* pfOutElapsedTick, const char* pData, DWORD dwSize, const WCHAR* wchFileName);

int main()
{
    QCInit();

    const WCHAR* wchFileName = L"Test.bin";
    const DWORD dwDataSize = 1024 * 1024 * 1024;	// 1GB

    char* pData = (char*)malloc(dwDataSize);
    memset(pData, 0, dwDataSize);

    // write file with bloking mode(default)
    float fElapsedTick = 0.0f;
    TestWriteFile(&fElapsedTick, pData, dwDataSize, wchFileName);
    wprintf_s(L"WriteFile() with blocking mode - %s , %.4f ms elapsed.\n", wchFileName, fElapsedTick);

    // write file with Overlapped mode(non blocking)
    fElapsedTick = 0.0f;
    TestWriteFileOverlapped(&fElapsedTick, pData, dwDataSize, wchFileName);
    wprintf_s(L"WriteFile() with Overlapped mode - %s , %.4f ms elapsed.\n", wchFileName, fElapsedTick);

    if (pData)
    {
        free(pData);
        pData = nullptr;
    }
    _getch();
}

BOOL TestWriteFile(float* pfOutElapsedTick, const char* pData, DWORD dwSize, const WCHAR* wchFileName)
{
    BOOL bResult = FALSE;

    HANDLE hFile = CreateFile(wchFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, nullptr);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        LARGE_INTEGER PrvCounter = QCGetCounter();

        DWORD dwWrittenBytes = 0;
        bResult = WriteFile(hFile, pData, dwSize, &dwWrittenBytes, nullptr);

        float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);

        if (!bResult)
        {
            DWORD dwErr = GetLastError();
            __debugbreak();
        }
        *pfOutElapsedTick = fElapsedTick;

        CloseHandle(hFile);
        hFile = nullptr;
    }
    return bResult;
}
BOOL TestWriteFileOverlapped(float* pfOutElapsedTick, const char* pData, DWORD dwSize, const WCHAR* wchFileName)
{
    BOOL bResult = FALSE;
    
    HANDLE hFile = CreateFile(wchFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH, nullptr);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        OVERLAPPED ovl = {};    // �� �Լ��� ����� ���� I/O�� �Ϸ�Ǳ� ������ ���ÿ� ������ �Ҵ��� ����!!!
        ovl.Offset = (DWORD)(-1);
        ovl.OffsetHigh = (DWORD)(-1);

        LARGE_INTEGER PrvCounter = QCGetCounter();
        bResult = WriteFile(hFile, pData, dwSize, nullptr, &ovl);

        float fElapsedTick = QCMeasureElapsedTick(QCGetCounter(), PrvCounter);
        if (!bResult)
        {
            DWORD dwErr = GetLastError();
            if (ERROR_IO_PENDING != dwErr)
                __debugbreak();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        // WriteFile()�� ��� �����ϹǷ� GetOverlappedResult()�� ȣ���ؼ� ����� Ȯ���� ������ �ٸ� �۾��� �� �� �ִ�.
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////

        DWORD dwWrittenBytes = 0;
        if (GetOverlappedResult(hFile, &ovl, &dwWrittenBytes, TRUE))
        {
            bResult = TRUE;
        }
        else
        {
            DWORD dwErr = GetLastError();
            __debugbreak();
        }
        *pfOutElapsedTick = fElapsedTick;

        CloseHandle(hFile);
        hFile = nullptr;
    }
    return bResult;
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
