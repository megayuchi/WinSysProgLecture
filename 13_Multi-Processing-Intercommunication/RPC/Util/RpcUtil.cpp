#include "pch.h"
#include <Windows.h>
#include <Rpc.h>
#include "RpcUtil.h"

void GetRpcError(WCHAR* wchOutBuffer, DWORD dwMaxBufferCount, RPC_STATUS status)
{
	//	https://learn.microsoft.com/ko-kr/windows/win32/debug/system-error-codes--1700-3999-
	const WCHAR* wchErr = L"";
	switch (status)
	{
		case RPC_S_OK:
			wchErr = L"RPC_S_OK";
			break;
		case RPC_S_PROTOCOL_ERROR:
			wchErr = L"RPC_S_PROTOCOL_ERROR";
			break;
		case RPC_S_INVALID_BINDING:
			wchErr = L"RPC_S_INVALID_BINDING";
			break;
		case RPC_S_WRONG_KIND_OF_BINDING:
			wchErr = L"RPC_S_WRONG_KIND_OF_BINDING";
			break;
		case RPC_S_PROTSEQ_NOT_SUPPORTED:
			wchErr = L"RPC_S_PROTSEQ_NOT_SUPPORTED";
			break;
		case RPC_S_INVALID_RPC_PROTSEQ:
			wchErr = L"RPC_S_INVALID_RPC_PROTSEQ";
			break;
		case RPC_S_INVALID_ENDPOINT_FORMAT:
			wchErr = L"RPC_S_INVALID_ENDPOINT_FORMAT";
			break;
		case RPC_S_ALREADY_LISTENING:
			wchErr = L"RPC_S_ALREADY_LISTENING";
			break;
		case RPC_S_OUT_OF_MEMORY:
			wchErr = L"RPC_S_OUT_OF_MEMORY";
			break;
		case RPC_S_DUPLICATE_ENDPOINT:
			wchErr = L"RPC_S_DUPLICATE_ENDPOINT";
			break;
		case RPC_S_INVALID_SECURITY_DESC:
			wchErr = L"RPC_S_INVALID_SECURITY_DESC";
			break;
	}
	wcscpy_s(wchOutBuffer, dwMaxBufferCount, wchErr);
}