// ParentProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <conio.h>

BOOL StartChildProcess(HANDLE* phOutProcess, HANDLE* phOutThread);

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	wprintf_s(L"[ParentProcess]\n");

    HANDLE hChildProcess = nullptr;
	HANDLE hChildThread = nullptr;
	StartChildProcess(&hChildProcess, &hChildThread);

	wprintf_s(L"Press any key.\n");
	_getch();

	HANDLE hDestroyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"ChildProcess-Destroy-Event");
	if (hDestroyEvent)
	{
		SetEvent(hDestroyEvent);
		CloseHandle(hDestroyEvent);
	}
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;

}


BOOL StartChildProcess(HANDLE* phOutProcess, HANDLE* phOutThread)
{
	BOOL bResult = FALSE;

	const WCHAR* wchExeName = L"";
#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#ifdef _DEBUG
		wchExeName = L"\\ChildProcess_arm64_debug.exe";
	#else
		wchExeName = L"\\ChildProcess_arm64_release.exe";
	#endif
#elif defined(_M_AMD64)
	#ifdef _DEBUG
		wchExeName = L"\\ChildProcess_x64_debug.exe";
	#else
		wchExeName = L"\\ChildProcess_x64_release.exe";
	#endif
#elif defined(_M_IX86)
	#ifdef _DEBUG
		wchExeName = L"\\ChildProcess_x86_debug.exe";
	#else
		wchExeName = L"\\ChildProcess_x86_release.exe";
	#endif
#endif

	WCHAR wchFullPath[_MAX_PATH] = {};

	// get full path
	SetCurrentDirectory(L"../ChildProcess/");
	GetCurrentDirectory(_MAX_PATH, wchFullPath);
	wcscat_s(wchFullPath, wchExeName);


	// create Childer process
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};

	si.cb = sizeof(si);
	DWORD	dwCreationFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
	if (CreateProcess(nullptr, wchFullPath, nullptr, nullptr, FALSE, dwCreationFlag, nullptr, nullptr, &si, &pi))
	{
		if (pi.hProcess)
		{
			*phOutProcess = pi.hProcess;
		}
		if (pi.hThread)
		{
			*phOutThread = pi.hThread;
		}
		bResult = TRUE;
	}
	return bResult;
}