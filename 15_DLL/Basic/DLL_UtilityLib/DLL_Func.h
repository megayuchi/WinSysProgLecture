#pragma once

// 이 헤더파일을 include하는 .exe 프로젝트 입장에선 함수들 이름에 __declspec(dllimport)  지시어가 붙은것으로 인식하게 된다.
#ifndef DLL_PROC
#define DLL_PROC extern "C" __declspec(dllimport) 
#endif

DLL_PROC int __stdcall DLL_Add(int a, int b);
DLL_PROC void __stdcall DLL_MemCpy_1Byte(char* pDest, const char* pSrc, size_t Size);
DLL_PROC void __stdcall DLL_MemCpy_4Bytes(char* pDest, const char* pSrc, size_t Size);
DLL_PROC void __stdcall DLL_MemCpy_8Bytes(char* pDest, const char* pSrc, size_t Size);	// 전달될 메모리 포인터는 8 bytes align필수
DLL_PROC void __stdcall DLL_MemCpy_16Bytes(char* pDest, const char* pSrc, size_t Size);	// 전달될 메모리 포인터는 32 bytes align필수
DLL_PROC void __stdcall DLL_MemCpy_32Bytes(char* pDest, const char* pSrc, size_t Size);	// 전달될 메모리 포인터는 32 bytes align필수
