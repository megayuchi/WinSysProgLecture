// Process_ExplicitLink.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <conio.h>

int typedef (__stdcall *DLL_Add_Proc)(int a, int b);
void typedef (__stdcall *DLL_MemCpy_32Bytes_Proc)(char* pDest, const char* pSrc, size_t Size);


int main()
{
    
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const WCHAR* wchDLLFileName = nullptr;
#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#ifdef _DEBUG
		wchDLLFileName = L"../DLL/DLL_UtilityLib_arm64_debug.dll");
	#else
		wchDLLFileName = "../DLL/DLL_UtilityLib_arm64_release.dll";
	#endif
#elif defined(_M_AMD64)
	#ifdef _DEBUG
		wchDLLFileName = L"../DLL/DLL_UtilityLib_x64_debug.dll";
	#else
		wchDLLFileName = L"../DLL/DLL_UtilityLib_x64_releaes.dll";
	#endif
#elif defined(_M_IX86)
	#ifdef _DEBUG
		wchDLLFileName = L"../DLL/DLL_UtilityLib_x86_debug.dll";
	#else
		wchDLLFileName = L"../DLL/DLL_UtilityLib_x86_release.dll";
	#endif
#endif
	wprintf_s(L"Trying to Load DLL:%s\n", wchDLLFileName);

	HMODULE hDLL = LoadLibrary(wchDLLFileName);
	if (hDLL)
	{
		wprintf_s(L"OK.\n", wchDLLFileName);
	}
	else
	{
		DWORD dwErrCode = GetLastError();
		wprintf_s(L"Fail to LoadLibrary(%s) - Error Code: %u", wchDLLFileName, dwErrCode);
		__debugbreak();
	}

	DLL_Add_Proc DLL_Add = (DLL_Add_Proc)GetProcAddress(hDLL, "DLL_Add");
	DLL_MemCpy_32Bytes_Proc DLL_MemCpy_32Bytes = (DLL_MemCpy_32Bytes_Proc)GetProcAddress(hDLL, "DLL_MemCpy_32Bytes");
	
	// __declspec(export)로 export한 c함수 호출
	int a = 1;
	int b = 2;
	int r = DLL_Add(1, 2);
	wprintf_s(L"call DLL_Add(%d, %d) = %d\n", a, b, r);

	// .def 로 export시킨 x64 asm함수 호출
	__declspec(align(32)) WCHAR wchSrc[32] = L"Hello World";
	__declspec(align(32)) WCHAR wchDest[32] = {};
	DLL_MemCpy_32Bytes((char*)wchDest, (const char*)wchSrc, 32);
	wprintf_s(L"call DLL_MemCpy_32Bytes() - src:%s -> dest:%s\n", wchSrc, wchDest);
	
	wprintf_s(L"Program finished. Press any key.\n");
	_getch();

	if (hDLL)
	{
		FreeLibrary(hDLL);
		hDLL = nullptr;
	}

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

}
