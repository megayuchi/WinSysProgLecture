#pragma once

#include "../NamedPipeLib_Interface/INamedPipeLib.h"

class CNamedPipeLib : public INamedPipeLib
{
	DWORD m_dwRefCount = 1;
	OVERLAPPED m_OvlConnect = {};
	HANDLE m_hPipe = INVALID_HANDLE_VALUE;
	BOOL	m_bIsConnected = FALSE;
	
	BOOL	m_bIsSending = FALSE;
	OVERLAPPED m_OvlSend = {};
	char*	m_pSendBuffer = nullptr;
	DWORD	m_dwSendBufferSize = 0;
	DWORD	m_dwPendingSendBufferBytes = 0;

	BOOL	m_bIsReceiving = FALSE;
	OVERLAPPED m_OvlRecv = {};
	char*	m_pRecvBuffer = nullptr;
	DWORD	m_dwRecvBufferSize = 0;
	DWORD	m_dwRecvBufferBytes = 0;
	
	char*	m_pMsgBuffer = nullptr;
	DWORD	m_dwMsgBufferWritePos = 0;
	DWORD	m_dwMsgBufferReadPos = 0;
	DWORD	m_dwMsgBufferSize = 0;
	DWORD	m_dwMsgCount = 0;

	BOOL	InitCommon(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize);
	void	CleanupCommon();
	BOOL	Recv();
	void	OnRecvCompleted(DWORD dwTransferredBytes);
	DWORD	CollectMsgFromRecvBuffer();
	BOOL	FlushSendBuffer();
	void	OnSendCompleted(DWORD dwTransferredBytes);

	void	Cleanup();
public:
	STDMETHODIMP			QueryInterface(REFIID, void** ppv);
	STDMETHODIMP_(ULONG)	AddRef();
	STDMETHODIMP_(ULONG)	Release();

	BOOL	__stdcall InitializeAsServer(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize);
	BOOL	__stdcall InitializeAsClient(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize);
	void	__stdcall Run(BOOL* pbOutOnConnected, BOOL* pbOutPacketExist);
	
	DWORD	__stdcall Send(const char* pData, DWORD dwSize);
	const char* __stdcall DispatchMsgSeq(DWORD* pdwOutSize);
	void	__stdcall ClearMsgBuffer();
	
	CNamedPipeLib();
	~CNamedPipeLib();

};