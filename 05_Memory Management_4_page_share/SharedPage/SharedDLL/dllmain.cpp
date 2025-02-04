// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#pragma data_seg("Shared")
WCHAR g_wchString[] = L"\"Happy new year!\"";
#pragma data_seg()

extern "C" DWORD __stdcall GetDllString(WCHAR** ppOutBuffer)
{
    *ppOutBuffer = g_wchString;
    DWORD dwLen = (DWORD)wcslen(g_wchString);
    return dwLen;
}
STDAPI DllCreateInstance(void** ppv)
{
	HRESULT hr = S_OK;
lb_return:
	return hr;
}
