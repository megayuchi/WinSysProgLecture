// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>

#pragma data_seg(".shared")
__declspec(dllexport) LONG volatile g_lRefCount = 0;
#pragma data_seg()

// 공유 데이터 섹션 속성 설정
#pragma comment(linker, "/SECTION:.shared,RWS")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    WCHAR wchTxt[128];
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		_InterlockedIncrement(&g_lRefCount);
		swprintf_s(wchTxt, L"DLL_PROCESS_ATTACH - ref count:%u\n", g_lRefCount);
		OutputDebugString(wchTxt);
		break;
	case DLL_THREAD_ATTACH:
		swprintf_s(wchTxt, L"DLL_THREAD_ATTACH - ref count:%u\n");
		OutputDebugString(wchTxt);
		break;
	case DLL_THREAD_DETACH:
		swprintf_s(wchTxt, L"DLL_THREAD_DETACH\n");
		OutputDebugString(wchTxt);
		break;
	case DLL_PROCESS_DETACH:
		_InterlockedDecrement(&g_lRefCount);
		swprintf_s(wchTxt, L"DLL_PROCESS_DETACH - ref count:%u\n", g_lRefCount);
		OutputDebugString(wchTxt);
        break;
    }
    return TRUE;
}
