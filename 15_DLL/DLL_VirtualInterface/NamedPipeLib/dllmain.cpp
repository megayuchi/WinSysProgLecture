// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "../NamedPipeLib_Interface/INamedPipeLib.h"
#include "NamedPipeLib.h"

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


STDAPI DllCreateInstance(void** ppv)
{
	HRESULT hr;
	INamedPipeLib*	pNamedPipeLib = new CNamedPipeLib;
	if (!pNamedPipeLib)
	{
		hr = E_OUTOFMEMORY;
		goto lb_return;
	}
	hr = S_OK;
	*ppv = pNamedPipeLib;
lb_return:
	return hr;
}
