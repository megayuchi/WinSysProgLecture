// ChildProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
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
const MEM_HEADER* g_pReadMemory = nullptr;
MEM_HEADER* g_pLocalMemory = nullptr;
DWORD g_dwMaxLocalMemSize = 0;
const DWORD SHARED_MAX_MEM_SIZE = 4096;

BOOL InitSharedMemory(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxMemSize);
DWORD ReadSharedMemory(char* pDest, DWORD dwMaxMemSize);
void CleanupSharedMemory();
void PrintMemory(const MEM_HEADER* pMemory, DWORD dwMemSize);

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	InitSharedMemory(L"SharedMemorySample-MemoryMap", L"SharedMemorySample-Mutex", SHARED_MAX_MEM_SIZE);		

	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			int iResult = ::MessageBox(nullptr, L"Will you Shutdown Shared-Memory Reader, Really?", L"Shutdown", MB_YESNO | MB_SYSTEMMODAL);
			if (IDYES == iResult)
			{
				break;
			}
		}
		DWORD dwReadSize = ReadSharedMemory((char*)g_pLocalMemory, SHARED_MAX_MEM_SIZE);
		PrintMemory(g_pLocalMemory, dwReadSize);
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
    g_hMemMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, wchMemoryMappedFileName);
    if (!g_hMemMap)
    {
        __debugbreak();
    }

    g_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, wchMutexName);
    if (!g_hMutex)
    {
        __debugbreak();
    }

    g_pReadMemory = (const MEM_HEADER*)MapViewOfFile(g_hMemMap, FILE_MAP_ALL_ACCESS, 0, 0, dwMaxMemSize);
	g_pLocalMemory = (MEM_HEADER*)malloc(dwMaxMemSize);
	memset(g_pLocalMemory, 0, dwMaxMemSize);
	
    return TRUE;
}
DWORD ReadSharedMemory(char* pDest, DWORD dwMaxMemSize)
{
	DWORD dwReadSize = 0;

    if (!g_hMemMap)
        goto lb_return;

    if (!g_pReadMemory)
        goto lb_return;


    WaitForSingleObject(g_hMutex, INFINITE);

	if (dwMaxMemSize < g_pReadMemory->dwCurSize)
	{
		__debugbreak();
	}
		
	memcpy(pDest, g_pReadMemory, g_pReadMemory->dwCurSize);
	dwReadSize = g_pReadMemory->dwCurSize;

	ReleaseMutex(g_hMutex);

lb_return:
	return dwReadSize;    
}
void CleanupSharedMemory()
{
    if (g_pLocalMemory)
    {
        free(g_pLocalMemory);
        g_pLocalMemory = nullptr;
    }
	if (g_hMutex)
	{
		CloseHandle(g_hMutex);
		g_hMutex = nullptr;
	}
    if (g_pReadMemory)
    {
        UnmapViewOfFile(g_pReadMemory);
        g_pReadMemory = nullptr;
    }
    if (g_hMemMap)
    {
        CloseHandle(g_hMemMap);
        g_hMemMap = nullptr;
    }
}
void PrintMemory(const MEM_HEADER* pMemory, DWORD dwMemSize)
{
	if (pMemory->dwCurSize != dwMemSize)
		__debugbreak();

	// MEM_HEADER(12) | str_len(4) | str(str_len*2) | 0(2) | str_len(4) | str(str_len*2) | 0(2) | str_len(4) | str(str_len*2) | 0(2) | str_len(4) | str(str_len*2) | 0(2) | .....
	const char* pSrc = pMemory->pReadDataEntry;
	for (DWORD i = 0; i < pMemory->dwItemCount; i++)
	{
		DWORD dwLen = *(DWORD*)pSrc;
		pSrc += sizeof(DWORD);
		const WCHAR* wchStr = (const WCHAR*)pSrc;
		wprintf_s(L"[%s]", wchStr);
		pSrc += (dwLen + 1) * sizeof(WCHAR);
	}
}