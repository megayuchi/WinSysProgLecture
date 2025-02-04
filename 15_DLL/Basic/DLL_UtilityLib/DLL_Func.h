#pragma once

// �� ��������� include�ϴ� .exe ������Ʈ ���忡�� �Լ��� �̸��� __declspec(dllimport)  ���þ ���������� �ν��ϰ� �ȴ�.
#ifndef DLL_PROC
#define DLL_PROC extern "C" __declspec(dllimport) 
#endif

DLL_PROC int __stdcall DLL_Add(int a, int b);
DLL_PROC void __stdcall DLL_MemCpy_1Byte(char* pDest, const char* pSrc, size_t Size);
DLL_PROC void __stdcall DLL_MemCpy_4Bytes(char* pDest, const char* pSrc, size_t Size);
DLL_PROC void __stdcall DLL_MemCpy_8Bytes(char* pDest, const char* pSrc, size_t Size);	// ���޵� �޸� �����ʹ� 8 bytes align�ʼ�
DLL_PROC void __stdcall DLL_MemCpy_16Bytes(char* pDest, const char* pSrc, size_t Size);	// ���޵� �޸� �����ʹ� 32 bytes align�ʼ�
DLL_PROC void __stdcall DLL_MemCpy_32Bytes(char* pDest, const char* pSrc, size_t Size);	// ���޵� �޸� �����ʹ� 32 bytes align�ʼ�
