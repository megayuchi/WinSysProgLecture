#pragma once

BOOL InitRpcServerProcess(const WCHAR* wchExeName, WCHAR* wchOutEndPointProtocol, DWORD dwMaxBufferCount, BOOL bCreateProcess);
HANDLE InitRpcClient(const WCHAR* wchEndPointProtocol);
void RpcClientCleanup(HANDLE hRpcBinding);

// call RPC-Server's functions
DWORD RpcConvertToUpperCase(HANDLE hRpcBinding, DWORD dwMaxBufferSize, WCHAR* wchOutBuffer, const WCHAR* wchSrc, DWORD dwLen);
DWORD RpcMemCpy(HANDLE hRpcBinding, unsigned char* pDest, DWORD dwDestSize, const unsigned char* pSrc, DWORD dwSrcSize);