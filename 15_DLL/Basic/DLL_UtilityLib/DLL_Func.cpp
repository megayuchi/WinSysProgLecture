#include "pch.h"

// .dll ������Ʈ ���忡�� �Լ��� �̸��� __declspec(dllexport)  ���þ ���������� �ν��ϰ� �ȴ�.
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