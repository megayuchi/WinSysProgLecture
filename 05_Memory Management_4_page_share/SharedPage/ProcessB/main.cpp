// ProcessA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
typedef DWORD (__stdcall *PROC_GetDllString)(WCHAR** ppOutBuffer);
PROC_GetDllString GetDllString = nullptr;
void Test();

int main()
{
	Test();
}
void Test()
{
	const WCHAR* wchDLLFileName = L"";
#if defined(_M_ARM64EC) || defined(_M_ARM64)
#ifdef _DEBUG
	wchDLLFileName = L"../DLL/SharedDLL_arm64_debug.dll";
#else
	wchDLLFileName = L"../DLL/SharedDLL_arm64_release.dll";
#endif
#elif defined(_M_AMD64)
#ifdef _DEBUG
	wchDLLFileName = L"../DLL/SharedDLL_x64_debug.dll";
#else
	wchDLLFileName = L"../DLL/SharedDLL_x64_release.dll";
#endif
#elif defined(_M_IX86)
#ifdef _DEBUG
	wchDLLFileName = L"../DLL/SharedDLL_x86_debug.dll";
#else
	wchDLLFileName = L"../DLL/SharedDLL_x86_release.dll";
#endif
#endif
	HMODULE hDLL = LoadLibrary(wchDLLFileName);
	if (!hDLL)
	{
		DWORD dwErr = GetLastError();
		int a = 0;
		__debugbreak();
	}
	GetDllString = (PROC_GetDllString)GetProcAddress(hDLL, "GetDllString");

	WCHAR* wchStrBuffer = nullptr;
	DWORD dwStrLen = GetDllString(&wchStrBuffer);
	wprintf_s(L"ProcessB.exe - String from DLL %s, buffer ptr:%p\n", wchStrBuffer, wchStrBuffer);
	wprintf_s(L"Trying write data to buffer from DLL. Press any key.\n");
	_getch();
	for (DWORD i = 0; i < dwStrLen; i++)
	{
		wchStrBuffer[i] = L'A';
	}
	wprintf_s(L"ProcessB.exe - String from DLL %s, buffer ptr:%p\n", wchStrBuffer, wchStrBuffer);
	wprintf_s(L"Program Finisted.Press any key.\n");
	_getch();
	if (hDLL)
	{
		FreeLibrary(hDLL);
		hDLL = nullptr;
	}
}