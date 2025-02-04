// ChildProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <conio.h>
#include "RpcClient.h"

#pragma comment(lib, "Rpcrt4.lib")

//
// RPC_Client(이 프로젝트)가 RPC_Server프로세스를 생성하지 않을 경우, RPC_Server프로세스가 먼저 실행되어야 한다.
// 종료시 RPC_Client를 먼저, RPC_Server를 나중에 종료시킨다.
//
int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	
	
	const WCHAR* wchRpcServerExeName = nullptr;
#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#ifdef _DEBUG
		wchRpcServerExeName = L"\\RPC_Server_arm64_debug.exe";
	#else
		wchRpcServerExeName = L"\\RPC_Server_arm64_release.exe";
	#endif
#elif defined(_M_AMD64)
	#ifdef _DEBUG
		wchRpcServerExeName = L"\\RPC_Server_x64_debug.exe";
	#else
		wchRpcServerExeName = L"\\RPC_Server_x64_release.exe";
	#endif
#elif defined(_M_IX86)
	#ifdef _DEBUG
		wchRpcServerExeName = L"\\RPC_Server_x86_debug.exe";
	#else
		wchRpcServerExeName = L"\\RPC_Server_x86_release.exe";
	#endif
#endif
	// 연결할 RPC서버의 fullpath 실행 파일의 full path를 얻는다.
	WCHAR wchFullPath[_MAX_PATH] = {};
	SetCurrentDirectory(L"../RPC_Server/");
	GetCurrentDirectory(_MAX_PATH, wchFullPath);
	wcscat_s(wchFullPath, wchRpcServerExeName);
	
	// RPC서버 프로세스를 실행, 또는 이미 RPC서버가 실행되어있을 경우 RPC서버에 연결.
	// RPC서버를 수동으로 실행할 경우, InitRpcServerProcess()함수의 마지막 인자를 FALSE로 전달한다. 이 경우 RPC_Server프로세스(이 프로젝트)가 먼저 실행되어있어야 한다.
	WCHAR wchOutEndPointProtocol[128] = {};
	HANDLE hRpcBinding = nullptr;
	if (InitRpcServerProcess(wchFullPath, wchOutEndPointProtocol, (DWORD)_countof(wchOutEndPointProtocol), TRUE))	// RPC서버를 수동으로 실행할 경우 마지막 인자를 FALSE로...
	{
		// RPC서버에 연결.
		hRpcBinding = InitRpcClient(wchOutEndPointProtocol);
		if (hRpcBinding)
		{
			wprintf_s(L"RPC-Client Initialized.\n");
		}
		else
		{
			wprintf_s(L"Failed to initilized RPC-Client.\n");
			__debugbreak();
		}
	}
		

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HWND	hConsoleWindow = nullptr;// GetConsoleWindow();
	INPUT_RECORD	irBuffer;
	memset(&irBuffer, 0, sizeof(INPUT_RECORD));
	DWORD	dwResult;

	BOOL IsLoop = TRUE;
	while (IsLoop)
	{
		ReadConsoleInput(hIn, &irBuffer, 1, &dwResult);

		if (irBuffer.EventType == KEY_EVENT)
		{
			if (irBuffer.Event.KeyEvent.bKeyDown)
			{
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F1)
				{
					const WCHAR* wchStr = L"hello world~~";
					WCHAR wchResult[128] = {};
					if (hRpcBinding)
					{
						DWORD dwResultLen = RpcConvertToUpperCase(hRpcBinding, (DWORD)_countof(wchResult), wchResult, wchStr, (DWORD)wcslen(wchStr));
						wprintf_s(L"called RpcConvertToUpperCase() - %s -> %s\n", wchStr, wchResult);
					}
				}
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F2)
				{
					const WCHAR* wchSrc = L"RPC-MemCpyTest";
					WCHAR wchDest[128] = {};
					if (hRpcBinding)
					{
						DWORD dwSrcSize = (DWORD)wcslen(wchSrc) * sizeof(WCHAR);
						DWORD dwDestSize = (DWORD)sizeof(wchDest);
						DWORD dwResultLen = RpcMemCpy(hRpcBinding, (unsigned char*)wchDest, dwDestSize, (const unsigned char*)wchSrc, dwSrcSize);
						wprintf_s(L"called RpcMemCpy() - %s -> %s\n", wchSrc, wchDest);
					}
				}
			
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
				{
					int iResult = ::MessageBox(hConsoleWindow, L"Will you Shutdown RPC Client, Really?", L"Shutdown", MB_YESNO | MB_SYSTEMMODAL);
					if (IDYES == iResult)
					{
						IsLoop = FALSE;
					}
				}
			}
		}
	}

	
	

	if (hRpcBinding)
	{
		RpcClientCleanup(hRpcBinding);
		hRpcBinding = nullptr;
	}

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;
}
