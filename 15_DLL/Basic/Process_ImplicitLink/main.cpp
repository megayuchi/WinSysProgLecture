// Process_ImplicitLink.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "../DLL_UtilityLib/DLL_Func.h"
#include <conio.h>

// .lib를 사용한 implicit link
#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#ifdef _DEBUG
		#pragma comment(lib, "../lib/DLL_UtilityLib_arm64_debug.lib")
	#else
		#pragma comment(lib, "../lib/DLL_UtilityLib_arm64_release.lib")
	#endif
#elif defined(_M_AMD64)
	#ifdef _DEBUG
		#pragma comment(lib, "../lib/DLL_UtilityLib_x64_debug.lib")
	#else
		#pragma comment(lib, "../lib/DLL_UtilityLib_x64_release.lib")
	#endif
#elif defined(_M_IX86)
	#ifdef _DEBUG
		#pragma comment(lib, "../lib/DLL_UtilityLib_x86_debug.lib")
	#else
		#pragma comment(lib, "../lib/DLL_UtilityLib_x86_release.lib")
	#endif
#endif


int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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
	
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
}
