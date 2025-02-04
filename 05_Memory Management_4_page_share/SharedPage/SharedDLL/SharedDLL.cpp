#include "pch.h"
#include <Windows.h>
#include "../NamedPipeLib_Interface/INamedPipeLib.h"
#include "NamedPipeLib.h"

CNamedPipeLib::CNamedPipeLib()
{
}

STDMETHODIMP CNamedPipeLib::QueryInterface(REFIID refiid, void** ppv)
{

	return E_NOINTERFACE;


}
STDMETHODIMP_(ULONG) CNamedPipeLib::AddRef()
{
	m_dwRefCount++;
	return m_dwRefCount;

}
STDMETHODIMP_(ULONG) CNamedPipeLib::Release()
{
	DWORD	ref_count = --m_dwRefCount;
	if (!m_dwRefCount)
		delete this;

	return ref_count;
}

BOOL CNamedPipeLib::InitCommon(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize)
{
	m_dwRecvBufferSize = dwRecvBufferSize;
	m_pRecvBuffer = (char*)malloc(m_dwRecvBufferSize);
	memset(m_pRecvBuffer, 0, m_dwRecvBufferSize);

	//m_pRecvBufferBackup = (char*)malloc(m_dwRecvBufferSize);
	//memset(m_pRecvBufferBackup, 0, m_dwRecvBufferSize);

	m_dwSendBufferSize = dwSendBufferSize;
	m_pSendBuffer = (char*)malloc(m_dwSendBufferSize);
	memset(m_pSendBuffer, 0, m_dwSendBufferSize);

	m_dwMsgBufferSize = dwMaxMsgBufferSize;
	m_pMsgBuffer = (char*)malloc(m_dwMsgBufferSize);
	memset(m_pMsgBuffer, 0, m_dwMsgBufferSize);

	return TRUE;
}
BOOL __stdcall CNamedPipeLib::InitializeAsServer(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize)
{
	BOOL bResult = FALSE;

	InitCommon(dwRecvBufferSize, dwSendBufferSize, dwMaxMsgBufferSize);

	//PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED
	m_hPipe = CreateNamedPipe(L"\\\\.\\pipe\\MyNamedPipe", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, dwSendBufferSize, dwRecvBufferSize, 0, nullptr);

	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
	#ifdef _DEBUG
		__debugbreak();
	#endif
		DWORD dwErr = GetLastError();
		goto lb_return;
	}
	bResult = ConnectNamedPipe(m_hPipe, &m_OvlConnect);
	if (!bResult)
	{
		DWORD dwErr = GetLastError();
		if (ERROR_IO_PENDING == dwErr)
		{
			bResult = TRUE;
		}
		else
		{
		#ifdef _DEBUG
			__debugbreak();
		#endif
			CloseHandle(m_hPipe);
			m_hPipe = nullptr;
		}
	}
lb_return:
	return bResult;
}
BOOL __stdcall CNamedPipeLib::InitializeAsClient(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize)
{
	BOOL bResult = FALSE;

	InitCommon(dwRecvBufferSize, dwSendBufferSize, dwMaxMsgBufferSize);

	m_hPipe = CreateFile(L"\\\\.\\pipe\\MyNamedPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING , FILE_FLAG_OVERLAPPED, nullptr);

    if (m_hPipe == INVALID_HANDLE_VALUE) 
	{
	#ifdef _DEBUG
		__debugbreak();
	#endif
		goto lb_return;
    }
	
	m_bIsConnected = TRUE;
	bResult = TRUE;
lb_return:
	return bResult;
}
void __stdcall CNamedPipeLib::Run(BOOL* pbOutOnConnected, BOOL* pbOutPacketExist)
{
	// check connected
	if (!m_bIsConnected)
	{
		DWORD dwTransferredBytes = 0;
		BOOL bResult = GetOverlappedResult(m_hPipe, &m_OvlConnect, &dwTransferredBytes, FALSE);
		if (bResult)
		{
			*pbOutOnConnected = TRUE;
			m_bIsConnected = TRUE;
		}
		else
		{
			DWORD dwErr = GetLastError();
			if (ERROR_IO_INCOMPLETE == dwErr)
			{
				// 아직 접속 안된 상태
			}
			int a = 0;
		}
	}
	if (m_bIsSending)
	{
		// 비동기 write작업이 진행중인 경우
		DWORD dwTransferredBytes = 0;
		BOOL bResult = GetOverlappedResult(m_hPipe, &m_OvlSend, &dwTransferredBytes, FALSE);
		if (bResult)
		{
			OnSendCompleted(dwTransferredBytes);

			if (m_dwPendingSendBufferBytes)
			{
				// 아직 보내야할 데이터가 남아있는 경우 남은 데이터 전송
				FlushSendBuffer();
			}
		}
		else
		{
			DWORD dwErr = GetLastError();
			if (ERROR_IO_INCOMPLETE == dwErr)
			{
				// 아직 전송되지 않음.
				int a = 0;
			}
		}
	}
	if (m_bIsReceiving)
	{
		DWORD dwTransferredBytes = 0;
		BOOL bResult = GetOverlappedResult(m_hPipe, &m_OvlRecv, &dwTransferredBytes, FALSE);
		if (bResult)
		{
			OnRecvCompleted(dwTransferredBytes);
		}
		else
		{
			DWORD dwErr = GetLastError();
			if (ERROR_IO_INCOMPLETE == dwErr)
			{
				// 아칙 처리 안됨
				int a = 0;
			}
		}
	}
	if (m_bIsConnected)
	{
		if (!m_bIsReceiving)
		{
			Recv();
		}
	}
	else
	{
		int a = 0;
	}
	if (m_dwMsgCount)
	{
		*pbOutPacketExist = TRUE;
	}
}
DWORD CNamedPipeLib::CollectMsgFromRecvBuffer()
{
	//
	// RecvBuffer로부터 처리할 수 있는 패킷을 수집
	//
	BOOL bResult = FALSE;
	DWORD dwCollectedBytes = 0;

	DWORD dwRemainedBytes = m_dwRecvBufferBytes;	// RecvBuffer에 남아있는 데이터 사이즈
	const char* pSrc = (const char*)m_pRecvBuffer;	// RecvBuffer에서 읽어낼 포인터

	// RecvBuffer의 내용을 스캔하며 최대한 수집할 수 있는만큼 패킷을 수집
	// 패킷 포맷 -  size(4Bytes) | Data(N Bytes) 
	while (1)
	{
		if (dwRemainedBytes < sizeof(DWORD))
		{
			// 최소 헤더 사이즈(4bytes)에 미달
			// 패킷을 더 수신해야만 처리할 수 있으므로 빠져나간다.
			break;
		}

		DWORD dwMsgSize = *(DWORD*)pSrc;
		if (dwRemainedBytes - sizeof(DWORD) < dwMsgSize)
		{
			// RecvBuffer의 남은 바이트 수가 헤더(4Bytes) + 데이터 사이즈(N Bytes)에 미달
			// 패킷을 더 수신해야만 처리할 수 있으므로 빠져나간다.
			break;
		}

		DWORD dwPacketSize = sizeof(DWORD) + dwMsgSize;
		if (0 != (dwPacketSize % 2))
			__debugbreak();

		if (m_dwMsgBufferWritePos + dwPacketSize > m_dwMsgBufferSize)
		{
			// 수집한 메시지를 담아둘 버퍼 공간이 부족할 경우
		#ifdef _DEBUG
			__debugbreak();
		#endif
			break;
		}
			
		// 메시지버퍼에 카피
		memcpy(m_pMsgBuffer + m_dwMsgBufferWritePos, pSrc, dwPacketSize);
		m_dwMsgBufferWritePos += dwPacketSize;
		
		// RecvBuffer에 남아있게 될 사이즈
		dwRemainedBytes -= dwPacketSize;

		// 수집된 메시지 사이즈
		dwCollectedBytes += dwPacketSize;

		// 다음 패킷을 읽어낼 RecvBuffer포지션 조정
		pSrc += dwPacketSize;

		// 메시지 개수 증가
		m_dwMsgCount++;
	}
	// 다음 패킷을 수신할 RecvBuffer위치 조정
	if (dwRemainedBytes)
	{
		// 수집한 만큼의 데이터를 앞쪽부터 제거하고 이후의 데이터를 맨 앞으로 당겨온다.
		memmove(m_pRecvBuffer, m_pRecvBuffer + dwCollectedBytes, dwRemainedBytes);
	#ifdef _DEBUG
		//OutputDebugString(L"memmove(m_pRecvBuffer, m_pRecvBuffer + dwCollectedBytes, dwRemainedBytes)\n");
	#endif
	}
	m_dwRecvBufferBytes = dwRemainedBytes;	// RecvBuffer에 남아있는 바이트수

lb_return:
	return dwCollectedBytes;
}
const char* __stdcall CNamedPipeLib::DispatchMsgSeq(DWORD* pdwOutSize)
{
	const char* pMsg = nullptr;

	if (!m_dwMsgCount)
		goto lb_return;

	const char* pSrc = (const char*)(m_pMsgBuffer + m_dwMsgBufferReadPos);
	DWORD dwMsgSize = *(DWORD*)pSrc;
	pSrc += sizeof(DWORD);
	pMsg = pSrc;
	*pdwOutSize = dwMsgSize;
	m_dwMsgBufferReadPos += (sizeof(DWORD) + dwMsgSize);
	m_dwMsgCount--;

lb_return:
	return pMsg;
}
void __stdcall CNamedPipeLib::ClearMsgBuffer()
{
	m_dwMsgBufferWritePos = 0;
	m_dwMsgBufferReadPos = 0;
	m_dwMsgCount = 0;
}
BOOL CNamedPipeLib::Recv()
{
	//
	// 파일핸들로부터 비동기로 ReadFile호출
	//
	BOOL bResult = FALSE;

	if (INVALID_HANDLE_VALUE == m_hPipe)
		goto lb_return;

	// 현재 RecvBuffer에 쌓여있는 데이터의 뒷쪽에 붙여서 ReadFile을 호출
	DWORD dwAvailableBufferSize = m_dwRecvBufferSize - m_dwRecvBufferBytes;	// ReadFile에 넣어줄 가용버퍼 사이즈
	if (!dwAvailableBufferSize)
		goto lb_return;

	memset(&m_OvlRecv, 0, sizeof(m_OvlRecv));
	//memcpy(m_pRecvBufferBackup + m_dwRecvBufferBytes, m_pRecvBuffer + m_dwRecvBufferBytes, dwAvailableBufferSize);	// backup 
	DWORD dwTransferredBytes = 0;
	bResult = ReadFile(m_hPipe, m_pRecvBuffer + m_dwRecvBufferBytes, dwAvailableBufferSize, &dwTransferredBytes, &m_OvlRecv);
	if (bResult)
	{
		// ReadFile()이 즉시 완료된 경우
		OnRecvCompleted(dwTransferredBytes);
	}
	else
	{
		DWORD dwErr = GetLastError();
		if (ERROR_IO_PENDING == dwErr)
		{
			// 비동기로 pending된 경우
			bResult = TRUE;
			m_bIsReceiving = TRUE;
		}
		else
		{
			if (ERROR_MORE_DATA == dwErr)
			{
			#ifdef _DEBUG
				//OutputDebugString(L"ERROR_MORE_DATA\n");
			#endif
				// NamedPipe내부 버퍼에 dwAvailableBufferSize보다 큰 사이즈의 데이터가 pending되어있다.
				// (주의!)버퍼 포인터(m_pRecvBuffer + m_dwRecvBufferBytes)에 dwAvailableBufferSize만큼의 데이터는 저장되어있지만 dwTransferredBytes에 저장된 데이터 크기가 들어있지는 않다.
				// 따라서 ReadFile에 요청한 사이즈만큼은 수신되었다고 간주한다.
				dwTransferredBytes = dwAvailableBufferSize;
				OnRecvCompleted(dwTransferredBytes);
				bResult = TRUE;
			}
			else
			{
			#ifdef _DEBUG
				__debugbreak();
			#endif
			}
		}
	}
lb_return:
	return bResult;
}
DWORD __stdcall CNamedPipeLib::Send(const char* pData, DWORD dwSize)
{
	DWORD dwPushedSize = 0;

	// 실제로 SendBuffer 써넣을 데이터 크기는 4Bytes + dwSize, 써넣을 공간이 있는지 체크
	if (m_dwPendingSendBufferBytes + sizeof(DWORD) + dwSize > m_dwSendBufferSize)
	{
		goto lb_return;
	}
	// 보낼 데이터를 써넣을 위치
	char* pDest = (char*)(m_pSendBuffer + m_dwPendingSendBufferBytes);

	// 패킷의 사이즈 4바이트 써넣기
	*(DWORD*)pDest = dwSize;

	// dwSize만큼의 데이터 본체 써넣기
	pDest += sizeof(DWORD);
	memcpy(pDest, pData, dwSize);

	
	dwPushedSize = sizeof(DWORD) + dwSize;	// 최종적으로 써넣은 바이트 수
	m_dwPendingSendBufferBytes += dwPushedSize;	// SendBuffer에 쌓여있는 바이트 수

	if (m_bIsSending)
	{
		// 앞선 전송이 완료되기를 대기한다.
		int a = 0;
	}
	else
	{
		// 현재 WriteFile()이 진행중이지 않으면 버퍼에 있는 내용을 바로 전송
		FlushSendBuffer();
	}
lb_return:
	return dwPushedSize;
}
BOOL CNamedPipeLib::FlushSendBuffer()
{
	//
	// SendBuffer에 쌓여있는 데이터를 파일핸들로 써넣는다(전송)
	//
	memset(&m_OvlSend, 0, sizeof(m_OvlSend));
	DWORD dwTransferredBytes = 0;
	BOOL bResult = WriteFile(m_hPipe, m_pSendBuffer, m_dwPendingSendBufferBytes, &dwTransferredBytes, &m_OvlSend);
	if (bResult)
	{
		// 전송이 즉시 성공한 경우, 상대 프로세스가 아직 읽지 않아도 out버퍼에 여유가 있는 동안은 즉시 성공한다. 
		OnSendCompleted(dwTransferredBytes);
	}
	else
	{
		// 상대 프로세스가 데이터를 읽지 않아 out버퍼의 크기가 꽉 찼을 경우 
		DWORD dwErr = GetLastError();
		if (ERROR_IO_PENDING == dwErr)
		{
			m_bIsSending = TRUE;
			bResult = TRUE;
		}
		else
		{
			if (ERROR_NO_DATA == dwErr)
			{
				OutputDebugString(L"ERROR_NO_DATA\n");
			}
			else
			{
			#ifdef _DEBUG
				__debugbreak();
			#endif
			}

		}
	}
	return bResult;
}
void CNamedPipeLib::OnRecvCompleted(DWORD dwTransferredBytes)
{
	m_dwRecvBufferBytes += dwTransferredBytes;	// RecvBuffer에 쌓여있는 데이터 사이즈 증가
	m_bIsReceiving = FALSE; // ReadFile()중이 아님

	// RecvBuffer로부터 패킷을 수집
	CollectMsgFromRecvBuffer();
}
void CNamedPipeLib::OnSendCompleted(DWORD dwTransferredBytes)
{
	//
	// WriteFile이 완료된 경우. WriteFile호출 직후 완료된것으로 판명되거나 Overlapped I/O로 완료됐음을 확인한 경우
	//
	DWORD dwRemainedBytes = m_dwPendingSendBufferBytes - dwTransferredBytes;	// 실제 전송되고 남은 바이트 수
	if (dwRemainedBytes)
	{
		// 남은 바이트 수가 있으면 그만큼 버퍼의 맨 앞으로 땡겨온다.
		memmove(m_pSendBuffer, m_pSendBuffer + dwTransferredBytes, dwRemainedBytes);
	#ifdef _DEBUG
		//OutputDebugString(L"memmove(m_pSendBuffer, m_pSendBuffer + dwTransferredBytes, dwRemainedBytes);\n");
	#endif
	}
	m_dwPendingSendBufferBytes = dwRemainedBytes;	// SendBuffer에 남은 바이트 수 갱신
	m_bIsSending = FALSE;
}

void CNamedPipeLib::CleanupCommon()
{
	if (m_pRecvBuffer)
	{
		free(m_pRecvBuffer);
		m_pRecvBuffer = nullptr;
	}
	if (m_pSendBuffer)
	{
		free(m_pSendBuffer);
		m_pSendBuffer = nullptr;
	}
	if (m_pMsgBuffer)
	{
		free(m_pMsgBuffer);
		m_pMsgBuffer = nullptr;
	}
}
void CNamedPipeLib::Cleanup()
{
	if (INVALID_HANDLE_VALUE != m_hPipe)
	{
		if (m_bIsSending)
		{
			CancelIo(m_hPipe);
			// 비동기 write작업이 진행중인 경우 완료될때까지 대기
			DWORD dwTransferredBytes = 0;
			BOOL bResult = GetOverlappedResult(m_hPipe, &m_OvlSend, &dwTransferredBytes, TRUE);
			if (!bResult)
			{
				DWORD dwErr = GetLastError();
				switch (dwErr)
				{
					case ERROR_OPERATION_ABORTED:
						OutputDebugString(L"CancelIo() - ERROR_OPERATION_ABORTED\n");
						break;
					case ERROR_IO_INCOMPLETE:
						OutputDebugString(L"CancelIo() - ERROR_IO_INCOMPLETE\n");
						break;
				}
			}	
		}
		if (m_bIsReceiving)
		{
			CancelIo(m_hPipe);
			// 비동기 write작업이 진행중인 경우 완료될때까지 대기
			DWORD dwTransferredBytes = 0;
			BOOL bResult = GetOverlappedResult(m_hPipe, &m_OvlRecv, &dwTransferredBytes, TRUE);
			if (!bResult)
			{
				DWORD dwErr = GetLastError();
				switch (dwErr)
				{
					case ERROR_OPERATION_ABORTED:
						OutputDebugString(L"CancelIo() - ERROR_OPERATION_ABORTED\n");
						break;
					case ERROR_IO_INCOMPLETE:
						OutputDebugString(L"CancelIo() - ERROR_IO_INCOMPLETE\n");
						break;
				}
			}	
		}

		CloseHandle(m_hPipe);
		m_hPipe = nullptr;
	}
	CleanupCommon();
}
CNamedPipeLib::~CNamedPipeLib()
{
	Cleanup();
}