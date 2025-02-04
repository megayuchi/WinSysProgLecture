// ChildProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <conio.h>

struct PIPE_MSG
{
	DWORD dwStrLen;
	WCHAR wchStr[64];
	DWORD GetPacketSize()
	{
		DWORD dwPacketSize = (DWORD)((DWORD_PTR)(void*)(this->wchStr) - (DWORD_PTR)(void*)this) + (dwStrLen + 1) * (DWORD)sizeof(WCHAR);
		return dwPacketSize;
	}
};
HANDLE InitNamedPipeAsClient(DWORD dwRecvBufferSize, DWORD dwSendBufferSize);

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	wprintf_s(L"Named Pipe Client initializing...\n");

	PIPE_MSG	msg = {};
	HANDLE hPipe = InitNamedPipeAsClient(1024, 1024);
	if (INVALID_HANDLE_VALUE != hPipe)
	{
		wprintf_s(L"Ok.\n");

		wprintf_s(L"Reading data from named pipe....\n");

		DWORD dwTransferredBytes = 0;
		if (ReadFile(hPipe, &msg, sizeof(msg), &dwTransferredBytes, nullptr))
		{
			wprintf_s(L"packet recevied : %s, [%u]\n", msg.wchStr, msg.dwStrLen);
		}
		else
		{
			DWORD dwErr = GetLastError();
			__debugbreak();
		}
	}
	
	wprintf_s(L"Program finished. Presss any key.\n");
	_getch();

	if (hPipe)
	{
		CloseHandle(hPipe);
		hPipe = nullptr;
	}


#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return 0;
}


HANDLE InitNamedPipeAsClient(DWORD dwRecvBufferSize, DWORD dwSendBufferSize)
{
	// NamedPipe�� �����Ǿ��ٰ� �����ϰ� �̿� ����. Named_Pipe_Server�� ���� ������ ��
	HANDLE hPipe = CreateFile(L"\\\\.\\pipe\\MyNamedPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING , 0, nullptr);

	if (INVALID_HANDLE_VALUE == hPipe)
	{
		DWORD dwErr = GetLastError();
		// server���μ����� ���� �����Ұ�.
	#ifdef _DEBUG
		__debugbreak();
	#endif
    }
	return hPipe;
}