// ParentProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "RpcServer.h"

#pragma comment(lib, "Rpcrt4.lib")

DWORD g_dwParentProcessId = 0;
HANDLE g_hCompleteEvent = nullptr;

//
// RPC_Client�� RPC_Server���μ���(�� ������Ʈ)�� �������� ���� ���, RPC_Server���μ���(�� ������Ʈ)�� ���� ����Ǿ�� �Ѵ�.
// ����� RPC_Client�� ����, RPC_Server�� ���߿� �����Ų��.
//
int wmain(int argc, WCHAR *argv[])
{
	
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	WCHAR	wchEndPointName[64] = {};
	if (argc > 1)
	{
		g_dwParentProcessId = _wtoi(argv[1]);
		
	}
	
	wprintf_s(L"RPC-Server initializing...\n");
	
	DWORD dwThreadId = GetCurrentThreadId();
	wprintf_s(L"Main Thread:%u\n", dwThreadId);

	swprintf_s(wchEndPointName, L"RpcServer-%08u", g_dwParentProcessId);

    // �Ϸ�ó�� �̺�Ʈ 
	g_hCompleteEvent = CreateEvent(nullptr, FALSE, FALSE, wchEndPointName);

	// RPC�����μ��� �ʱ�ȭ
	RpcServerInit(wchEndPointName);

	SetEvent(g_hCompleteEvent);
		
	wprintf_s(L"OK.\n");
	

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
			
				}
			
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
				{
					int iResult = ::MessageBox(hConsoleWindow, L"Will you Shutdown RPC Server, Really?", L"Shutdown", MB_YESNO | MB_SYSTEMMODAL);
					if (IDYES == iResult)
					{
						IsLoop = FALSE;
					}
				}
			}
		}
	}


	RpcServerCleanup();

	if (g_hCompleteEvent)
	{
		CloseHandle(g_hCompleteEvent);
		g_hCompleteEvent = nullptr;
	}
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;

}
