#include "pch.h"
#include "RpcClient.h"

#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#include "../RPC_Stub/arm64/RPC_Stub_h.h"
#elif defined(_M_AMD64)
	#include "../RPC_Stub/x64/RPC_Stub_h.h"
#elif defined(_M_IX86)
	#include "../RPC_Stub/x86/RPC_Stub_h.h"
#endif

BOOL g_bRpcServerEnabled = FALSE;

BOOL InitRpcServerProcess(const WCHAR* wchExeName, WCHAR* wchOutEndPointProtocol, DWORD dwMaxBufferCount, BOOL bCreateProcess)
{
	BOOL bResult = FALSE;
	BOOL bMustTerminateProcess = FALSE;
	HANDLE	hEventCompleteInit = nullptr;

	PROCESS_INFORMATION pi = {};
	WCHAR wcEndPointName[128] = {};
	DWORD dwProcessID = 0;
	if (bCreateProcess)
	{
		//
		// RPC서버 프로세스를 생성
		//
		dwProcessID = GetCurrentProcessId();
		STARTUPINFO si = {};
		si.cb = sizeof(si);

		WCHAR	wchCmdArg[64] = {};
		_itow_s(dwProcessID, wchCmdArg, 10);

		WCHAR	wchCmdLine[_MAX_PATH] = { 0 };
		swprintf_s(wchCmdLine, L"%s %s", wchExeName, wchCmdArg);

		DWORD	dwCreationFlag = CREATE_NEW_CONSOLE;
		if (!CreateProcess(NULL, wchCmdLine, NULL, NULL, FALSE, dwCreationFlag, NULL, NULL, &si, &pi))
			goto lb_return;
	}
	// EndPoint이름 설정
	swprintf_s(wcEndPointName, L"RpcServer-%08u", dwProcessID);

	// RPC서버 프로세스의 초기화가 완료되었는지 확인하기위한 EVENT객체를 얻는다
	const DWORD  MAX_FIND_EVENT_COUNT = 10;	// 10 시도
	for (DWORD i = 0; i < MAX_FIND_EVENT_COUNT; i++)
	{
		hEventCompleteInit = OpenEvent(EVENT_ALL_ACCESS, FALSE, wcEndPointName);
		if (hEventCompleteInit)
		{
			break;
		}
		else
		{
			wprintf_s(L"InitRpcServerProcess(), Failed to OpenEvent() - %s. Retrying....%u\n", wcEndPointName, i + 1);
			Sleep(100);
		}
	}
	if (!hEventCompleteInit)
	{
		// 이벤트를 못찾았으면 실패처리. 하지만 위에서 이미 프로세스를 생성했으므로 프로세스를 제거한다.
		bMustTerminateProcess = TRUE;
		goto lb_return;
	}
	const	DWORD START_PROCESS_TIMEOUT = 1000 * 3;	// 3초가 지나도록 실행 못하면 프로세스가 초기화에 실패했을 것이다.
	wprintf_s(L"InitRpcServerProcess(), Waiting for start server - %s\n", wchExeName);
	DWORD dwWaitResult = WaitForSingleObject(hEventCompleteInit, START_PROCESS_TIMEOUT);
	if (WAIT_OBJECT_0 == dwWaitResult)
	{
		bResult = TRUE;
		g_bRpcServerEnabled = TRUE;
		
		swprintf_s(wchOutEndPointProtocol, dwMaxBufferCount, L"ncalrpc:[%s]", wcEndPointName);
	}
	else
	{
		if (WAIT_TIMEOUT == dwWaitResult)
		{
			// 타임아우스로 이벤트가 Set되지 않은걸로 판단. 
			wprintf_s(L"InitRpcServerProcess(), Timed out.\n");
		}
		else
		{
			// 알 수 없는 이유로 Set되지 않은걸로 판단. 
			wprintf_s(L"InitRpcServerProcess(), Unknown\n");
		}
		// 위에서 이미 프로세스를 생성했으므로 프로세스를 제거한다.
		bMustTerminateProcess = TRUE;
	}
lb_return:
	if (hEventCompleteInit)
	{
		CloseHandle(hEventCompleteInit);
		hEventCompleteInit = nullptr;
	}
	if (pi.hProcess)
	{
		if (bMustTerminateProcess)
		{
			TerminateProcess(pi.hProcess, -1);
		}
		CloseHandle(pi.hProcess);
		pi.hProcess = nullptr;
	}
	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
		pi.hThread = nullptr;
	}
	return bResult;
}
HANDLE InitRpcClient(const WCHAR* wchEndPointProtocol)
{
	HANDLE hBindingHandle = nullptr;

	RPC_STATUS status = RpcBindingFromStringBinding((RPC_WSTR)wchEndPointProtocol, &hBindingHandle);

	if (status != RPC_S_OK)
	{
	#ifdef _DEBUG
		__debugbreak();
	#endif
	}

	return hBindingHandle;
}

void RpcClientCleanup(HANDLE hRpcBinding)
{
	if (hRpcBinding)
	{
		RPC_STATUS status = RpcBindingFree(&hRpcBinding);
		if (status != RPC_S_OK)
		{
		#ifdef _DEBUG
			__debugbreak();
		#endif
		}
	}
}

DWORD RpcConvertToUpperCase(HANDLE hRpcBinding, DWORD dwMaxBufferCount, WCHAR* wchOutBuffer, const WCHAR* wchSrc, DWORD dwLen)
{
	DWORD dwResultLen = RPC_ConvertToUpperCase(hRpcBinding, wchOutBuffer, dwMaxBufferCount, wchSrc, dwLen);
	return dwResultLen;
}
DWORD RpcMemCpy(HANDLE hRpcBinding, unsigned char* pDest, DWORD dwDestSize, const unsigned char* pSrc, DWORD dwSrcSize)
{
	DWORD dwCpySize = RPC_MemCpy(hRpcBinding, pDest, dwDestSize, pSrc, dwSrcSize);
	return dwCpySize;
}
