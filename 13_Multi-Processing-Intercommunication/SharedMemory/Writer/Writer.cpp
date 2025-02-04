#include "pch.h"
#include <Windows.h>
#include <Rpc.h>
#include "../Util/RpcUtil.h"
#include "RpcServer.h"

#if defined(_M_ARM64EC) || defined(_M_ARM64)
	#include "../RPC_Stub/arm64/RPC_Stub_h.h"
#elif defined(_M_AMD64)
	#include "../RPC_Stub/x64/RPC_Stub_h.h"
#elif defined(_M_IX86)
	#include "../RPC_Stub/x86/RPC_Stub_h.h"
#endif

BOOL RpcServerInit(const WCHAR* wchEndPointName)
{
	WCHAR wchErr[64] = {};

	// ncalrpc	Local RPC - 같은 PC 내 프로세스 간 통신 (빠름)
	// ncacn_ip_tcp - TCP/IP
	// ncacn_np - Named Pipe, SMB기반 IPC
	// ncacn_http - HTTP
	// ncadg_ip_udp - UDP

	RPC_STATUS status = RpcServerUseProtseqEp(
		(RPC_WSTR)L"ncalrpc",    // 프로토콜 시퀀스
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT,   // 최대 요청 수
		(RPC_WSTR)wchEndPointName,   // 엔드포인트 이름
		nullptr
	);
	if (RPC_S_OK != status)
	{
		GetRpcError(wchErr, (DWORD)_countof(wchErr), status);
	#ifdef _DEBUG
		__debugbreak();
	#endif
	}

	status = RpcServerRegisterIf2(
		RPC_StubInteface_v1_0_s_ifspec,
		nullptr,
		nullptr,
		RPC_IF_ALLOW_LOCAL_ONLY,
		RPC_C_LISTEN_MAX_CALLS_DEFAULT,
		-1,
		nullptr
	);
	if (RPC_S_OK != status)
	{
		GetRpcError(wchErr, (DWORD)_countof(wchErr), status);
	#ifdef _DEBUG
		__debugbreak();
	#endif
	}
	status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, 1);
	if (RPC_S_OK != status)
	{
		GetRpcError(wchErr, (DWORD)_countof(wchErr), status);
	#ifdef _DEBUG
		__debugbreak();
	#endif
	}
	
	return TRUE;
}
BOOL RpcServerCleanup()
{
	WCHAR wchErr[64] = {};
	RPC_STATUS status = RpcMgmtStopServerListening(nullptr);
	if (RPC_S_OK != status)
	{
		GetRpcError(wchErr, (DWORD)_countof(wchErr), status);
	#ifdef _DEBUG
		__debugbreak();
	#endif
	}
	return TRUE;
}
