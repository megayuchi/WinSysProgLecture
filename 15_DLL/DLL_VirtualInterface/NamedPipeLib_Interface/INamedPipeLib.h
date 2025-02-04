#pragma once

interface INamedPipeLib : public IUnknown
{
	virtual BOOL __stdcall InitializeAsServer(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize) = 0;
	virtual BOOL __stdcall InitializeAsClient(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize) = 0;
	virtual void __stdcall Run(BOOL* pbOutOnConnected, BOOL* pbOutPacketExist) = 0;
	virtual DWORD __stdcall Send(const char* pData, DWORD dwSize) = 0;
	virtual const char* __stdcall DispatchMsgSeq(DWORD* pdwOutSize) = 0;
	virtual void __stdcall ClearMsgBuffer() = 0;
};