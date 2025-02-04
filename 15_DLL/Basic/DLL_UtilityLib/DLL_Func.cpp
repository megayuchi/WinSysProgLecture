#include "pch.h"

// .dll 프로젝트 입장에선 함수들 이름에 __declspec(dllexport)  지시어가 붙은것으로 인식하게 된다.
#ifndef DLL_PROC
#define DLL_PROC extern "C" __declspec(dllexport)
#endif

DLL_PROC int __stdcall DLL_Add(int a, int b)
{
	int c = a + b;
	return c;
}

#if defined(_M_ARM64EC) || defined(_M_ARM64)
#elif defined(_M_AMD64)
#elif defined(_M_IX86)
#endif

#if !defined(_M_AMD64)
DLL_PROC void __stdcall DLL_MemCpy_1Byte(char* pDest, const char* pSrc, size_t Size)
{
}
DLL_PROC void __stdcall DLL_MemCpy_4Bytes(char* pDest, const char* pSrc, size_t Size)
{
}
DLL_PROC void __stdcall DLL_MemCpy_8Bytes(char* pDest, const char* pSrc, size_t Size)
{
}
DLL_PROC void __stdcall DLL_MemCpy_16Bytes(char* pDest, const char* pSrc, size_t Size)
{
}
DLL_PROC void __stdcall DLL_MemCpy_32Bytes(char* pDest, const char* pSrc, size_t Size)
{
}
#endif