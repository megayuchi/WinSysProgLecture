// ParentProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>


HANDLE InitNamedPipeAsServer(DWORD dwRecvBufferSize, DWORD dwSendBufferSize);


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

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// file을 생성하는 측이니까 server라 하자.
	wprintf_s(L"Named Pipe Server initializing...\n");

	HANDLE hPipe = InitNamedPipeAsServer(1024, 1024);
	if (INVALID_HANDLE_VALUE != hPipe)
	{
		wprintf_s(L"Ok.\n");

		wprintf_s(L"Writing data to named pipe...\n");
		PIPE_MSG msg;
		msg.dwStrLen = swprintf_s(msg.wchStr, L"Hello~ this is Server process.");

		DWORD dwTransferredBytes = 0;
		BOOL bWriteResult = WriteFile(hPipe, &msg, msg.GetPacketSize(), &dwTransferredBytes, nullptr);
		if (bWriteResult)
		{
			wprintf_s(L"data was written to named pipe successfully.\n");
		}
		else
		{
			DWORD dwErr = GetLastError();
			__debugbreak();
		}
	}
	wprintf_s(L"Program Finished. Press any key.\n");
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



HANDLE InitNamedPipeAsServer(DWORD dwRecvBufferSize, DWORD dwSendBufferSize)
{
	// PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE조합으로 메시지 단위로 전송. 버퍼 사이즈가 부족하지 않다면 데이터가 쪼개지는 일이 없다. 버퍼사이즈가 부족하면 데이터는 쪼개져서 전송될 수 있다.
	HANDLE hPipe = CreateNamedPipe(L"\\\\.\\pipe\\MyNamedPipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, dwSendBufferSize, dwRecvBufferSize, 0, nullptr);

	if (INVALID_HANDLE_VALUE == hPipe)
	{
	#ifdef _DEBUG
		__debugbreak();
	#endif
		DWORD dwErr = GetLastError();
		goto lb_return;
	}
	// 다른 프로세스가 CreateFile()로 \\\\.\\pipe\\MyNamedPipe 을 열 때까지 대기
	if (!ConnectNamedPipe(hPipe, nullptr))
	{
		DWORD dwErr = GetLastError();
		CloseHandle(hPipe);
		hPipe = nullptr;
	}
lb_return:
	return hPipe;
}