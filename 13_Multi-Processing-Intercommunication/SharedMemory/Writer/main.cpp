// ParentProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>



struct MEM_HEADER
{
	DWORD dwMaxMemSize;
	DWORD dwCurSize;
	DWORD dwItemCount;
	const char pReadDataEntry[0];
};

HANDLE g_hMemMap = nullptr;
HANDLE g_hMutex = nullptr;
MEM_HEADER* g_pWriteMemory = nullptr;

BOOL InitSharedMemory(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxMemSize);
BOOL WriteToSharedMemory(const WCHAR* wchStr, DWORD dwLen);
void ResetSharedMemory();
void CleanupSharedMemory();

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	srand(1);
	const WCHAR* wchTxtArgList[] = { L"A", L"BB", L"CCC", L"HITOMI", L"KASUMI", L"AYANE", L"LEIFANG", L"TINA", L"LISA" };
	DWORD dwTextArgCount = (DWORD)_countof(wchTxtArgList);

	InitSharedMemory(L"SharedMemorySample-MemoryMap", L"SharedMemorySample-Mutex", 4096);
	wprintf_s(L"SharedMemory Writer initialized.\n");

	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			int iResult = ::MessageBox(nullptr, L"Will you Shutdown Shared-Memory Writer, Really?", L"Shutdown", MB_YESNO | MB_SYSTEMMODAL);
			if (IDYES == iResult)
			{
				break;
			}
		}
		
		DWORD rnd_index = rand() % dwTextArgCount;
		const WCHAR* wchStr = wchTxtArgList[rnd_index];
		DWORD dwLen = wcslen(wchTxtArgList[rnd_index]);
		if (WriteToSharedMemory(wchStr, dwLen))
		{
			int a = 0;
		}
		else
		{
			ResetSharedMemory();
			Sleep(1000);
			
		}
		Sleep(1);
	}

	CleanupSharedMemory();
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;

}



BOOL InitSharedMemory(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxMemSize)
{
	if (dwMaxMemSize < sizeof(MEM_HEADER))
	{
		__debugbreak();
	}
	g_hMemMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, dwMaxMemSize, wchMemoryMappedFileName);
	if (!g_hMemMap)
	{
		__debugbreak();
	}
	g_hMutex = CreateMutex(nullptr, FALSE, wchMutexName);
	g_pWriteMemory = (MEM_HEADER*)MapViewOfFile(g_hMemMap, FILE_MAP_WRITE, 0, 0, dwMaxMemSize);
	if (!g_pWriteMemory)
	{
		__debugbreak();
	}
	g_pWriteMemory->dwMaxMemSize = dwMaxMemSize;
	g_pWriteMemory->dwCurSize = sizeof(MEM_HEADER);
	g_pWriteMemory->dwItemCount = 0;

	return TRUE;
}
void ResetSharedMemory()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	
	g_pWriteMemory->dwItemCount = 0;
	g_pWriteMemory->dwCurSize = sizeof(MEM_HEADER);

	ReleaseMutex(g_hMutex);
}
BOOL WriteToSharedMemory(const WCHAR* wchStr, DWORD dwLen)
{
	BOOL bResult = FALSE;

	// 길이(4) + (문자열 + null terminator)*2
	DWORD dwRequiredMemSize = sizeof(DWORD) + (dwLen + 1) * sizeof(WCHAR);

	// MEM_HEADER(12) | str_len(4) | str(str_len*2) | 0(2) | str_len(4) | str(str_len*2) | 0(2) | str_len(4) | str(str_len*2) | 0(2) | str_len(4) | str(str_len*2) | 0(2) | .....
	
	// lock buffer access
	WaitForSingleObject(g_hMutex, INFINITE);
	
	if (g_pWriteMemory->dwCurSize + dwRequiredMemSize <= g_pWriteMemory->dwMaxMemSize)
	{
		char* pDest = (char*)g_pWriteMemory + g_pWriteMemory->dwCurSize;
		char* pDestBackup = pDest;
		*(DWORD*)pDest = dwLen;
		pDest += sizeof(DWORD);
		DWORD dwCpySize = sizeof(WCHAR) * dwLen;
		memcpy(pDest, wchStr, dwCpySize);
		pDest += dwCpySize;
		*(WCHAR*)pDest = 0;
		pDest += sizeof(WCHAR);

		if ((DWORD)(DWORD_PTR)(pDest - pDestBackup) != dwRequiredMemSize)
			__debugbreak();

		g_pWriteMemory->dwCurSize += dwRequiredMemSize;
		g_pWriteMemory->dwItemCount++;
		bResult = TRUE;
	}
	else
	{
		int a = 0;
	}
	// unlock buffer access
	ReleaseMutex(g_hMutex);
	
lb_return:
	return bResult;
}
void CleanupSharedMemory()
{
	if (g_pWriteMemory)
	{
		UnmapViewOfFile(g_pWriteMemory);
		g_pWriteMemory = nullptr;
	}
	if (g_hMemMap)
	{		
		CloseHandle(g_hMemMap);
		g_hMemMap = nullptr;
	}
	if (g_hMutex)
	{
		CloseHandle(g_hMutex);
		g_hMutex = nullptr;
	}
}