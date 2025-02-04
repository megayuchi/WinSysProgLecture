#include "pch.h"
#include <Windows.h>
#include "NamedPipe.h"

CNamedPipe::CNamedPipe()
{
}

BOOL CNamedPipe::InitCommon(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize)
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
BOOL CNamedPipe::InitializeAsServer(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize)
{
	BOOL bResult = FALSE;

	InitCommon(dwRecvBufferSize, dwSendBufferSize, dwMaxMsgBufferSize);

	// PIPE_ACCESS_DUPLEX -> ����� ����
	// FILE_FLAG_OVERLAPPED -> �񵿱� ���
	// PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE -> ���� ���� �޽���. �⺻�����δ� ������ŭ�� �޽����� ���ֵǾ �޽��� ������ ���ŵ�. �ɰ����� ����. ������ ���� ����� �����ϸ� ��������� �ɰ���.
	// PIPE_TYPE_BYTE | PIPE_READMODE_BYTE -> ���۴��� ����Ʈ ��Ʈ��. ����Ʈ ������ ���۵ǹǷ� �����ʹ� ������ �ɰ��� �� ����.

	m_hPipe = CreateNamedPipe(L"\\\\.\\pipe\\MyNamedPipe", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, dwSendBufferSize, dwRecvBufferSize, 0, nullptr);
	//m_hPipe = CreateNamedPipe(L"\\\\.\\pipe\\MyNamedPipe", PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, dwSendBufferSize, dwRecvBufferSize, 0, nullptr);

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
BOOL CNamedPipe::InitializeAsClient(DWORD dwRecvBufferSize, DWORD dwSendBufferSize, DWORD dwMaxMsgBufferSize)
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
void CNamedPipe::Run(BOOL* pbOutOnConnected, BOOL* pbOutPacketExist)
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
				// ���� ���� �ȵ� ����
			}
			int a = 0;
		}
	}
	if (m_bIsSending)
	{
		// �񵿱� write�۾��� �������� ���
		DWORD dwTransferredBytes = 0;
		BOOL bResult = GetOverlappedResult(m_hPipe, &m_OvlSend, &dwTransferredBytes, FALSE);
		if (bResult)
		{
			OnSendCompleted(dwTransferredBytes);

			if (m_dwPendingSendBufferBytes)
			{
				// ���� �������� �����Ͱ� �����ִ� ��� ���� ������ ����
				FlushSendBuffer();
			}
		}
		else
		{
			DWORD dwErr = GetLastError();
			if (ERROR_IO_INCOMPLETE == dwErr)
			{
				// ���� ���۵��� ����.
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
				// ��Ģ ó�� �ȵ�
				int a = 0;
			}
		}
	}
	if (m_bIsConnected)
	{
		// ������ �� �����ε� ���� Recv(ReadFileȣ��)�õ��� ���� ���� ���
		if (!m_bIsReceiving)
		{
			// Recv(ReadFileȣ��)�õ�
			Recv();
		}
	}
	else
	{
		int a = 0;
	}
	if (m_dwMsgCount)
	{
		// ���� ���ŵǾ� ó���� ����ϰ� �ִ� ������ �޽����� ������
		*pbOutPacketExist = TRUE;
	}
}
DWORD CNamedPipe::CollectMsgFromRecvBuffer()
{
	//
	// RecvBuffer�κ��� ó���� �� �ִ� ��Ŷ�� ����
	//
	BOOL bResult = FALSE;
	DWORD dwCollectedBytes = 0;

	DWORD dwRemainedBytes = m_dwRecvBufferBytes;	// RecvBuffer�� �����ִ� ������ ������
	const char* pSrc = (const char*)m_pRecvBuffer;	// RecvBuffer���� �о ������

	// RecvBuffer�� ������ ��ĵ�ϸ� �ִ��� ������ �� �ִ¸�ŭ ��Ŷ�� ����
	// ��Ŷ ���� -  size(4Bytes) | Data(N Bytes) 
	while (1)
	{
		if (dwRemainedBytes < sizeof(DWORD))
		{
			// �ּ� ��� ������(4bytes)�� �̴�
			// ��Ŷ�� �� �����ؾ߸� ó���� �� �����Ƿ� ����������.
			break;
		}

		DWORD dwMsgSize = *(DWORD*)pSrc;
		if (dwRemainedBytes - sizeof(DWORD) < dwMsgSize)
		{
			// RecvBuffer�� ���� ����Ʈ ���� ���(4Bytes) + ������ ������(N Bytes)�� �̴�
			// ��Ŷ�� �� �����ؾ߸� ó���� �� �����Ƿ� ����������.
			break;
		}

		DWORD dwPacketSize = sizeof(DWORD) + dwMsgSize;
		if (0 != (dwPacketSize % 2))
			__debugbreak();

		if (m_dwMsgBufferWritePos + dwPacketSize > m_dwMsgBufferSize)
		{
			// ������ �޽����� ��Ƶ� ���� ������ ������ ���
		#ifdef _DEBUG
			__debugbreak();
		#endif
			break;
		}
			
		// �޽������ۿ� ī��
		memcpy(m_pMsgBuffer + m_dwMsgBufferWritePos, pSrc, dwPacketSize);
		m_dwMsgBufferWritePos += dwPacketSize;
		
		// RecvBuffer�� �����ְ� �� ������
		dwRemainedBytes -= dwPacketSize;

		// ������ �޽��� ������
		dwCollectedBytes += dwPacketSize;

		// ���� ��Ŷ�� �о RecvBuffer������ ����
		pSrc += dwPacketSize;

		// �޽��� ���� ����
		m_dwMsgCount++;
	}
	// ���� ��Ŷ�� ������ RecvBuffer��ġ ����
	if (dwRemainedBytes)
	{
		// ������ ��ŭ�� �����͸� ���ʺ��� �����ϰ� ������ �����͸� �� ������ ��ܿ´�.
		memmove(m_pRecvBuffer, m_pRecvBuffer + dwCollectedBytes, dwRemainedBytes);
	#ifdef _DEBUG
		//OutputDebugString(L"memmove(m_pRecvBuffer, m_pRecvBuffer + dwCollectedBytes, dwRemainedBytes)\n");
	#endif
	}
	m_dwRecvBufferBytes = dwRemainedBytes;	// RecvBuffer�� �����ִ� ����Ʈ��

lb_return:
	return dwCollectedBytes;
}
const char* CNamedPipe::DispatchMsgSeq(DWORD* pdwOutSize)
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
void CNamedPipe::ClearMsgBuffer()
{
	m_dwMsgBufferWritePos = 0;
	m_dwMsgBufferReadPos = 0;
	m_dwMsgCount = 0;
}
BOOL CNamedPipe::Recv()
{
	//
	// �����ڵ�κ��� �񵿱�� ReadFileȣ��
	//
	BOOL bResult = FALSE;

	if (INVALID_HANDLE_VALUE == m_hPipe)
		goto lb_return;

	// ���� RecvBuffer�� �׿��ִ� �������� ���ʿ� �ٿ��� ReadFile�� ȣ��
	DWORD dwAvailableBufferSize = m_dwRecvBufferSize - m_dwRecvBufferBytes;	// ReadFile�� �־��� ������� ������
	if (!dwAvailableBufferSize)
		goto lb_return;

	memset(&m_OvlRecv, 0, sizeof(m_OvlRecv));
	//memcpy(m_pRecvBufferBackup + m_dwRecvBufferBytes, m_pRecvBuffer + m_dwRecvBufferBytes, dwAvailableBufferSize);	// backup 
	DWORD dwTransferredBytes = 0;
	bResult = ReadFile(m_hPipe, m_pRecvBuffer + m_dwRecvBufferBytes, dwAvailableBufferSize, &dwTransferredBytes, &m_OvlRecv);
	if (bResult)
	{
		// ReadFile()�� ��� �Ϸ�� ���
		OnRecvCompleted(dwTransferredBytes);
	}
	else
	{
		DWORD dwErr = GetLastError();
		if (ERROR_IO_PENDING == dwErr)
		{
			// �񵿱�� pending�� ���
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
				// NamedPipe���� ���ۿ� dwAvailableBufferSize���� ū �������� �����Ͱ� pending�Ǿ��ִ�. 
				// NamedPipe�� PIPE_READMODE_MESSAGE�� �����Ǿ�����, �߻��� �� �ִ�. ��û�� ����� �о��� ��� '�޽��� ������ �д´ٴ� ��Ģ'�� ������ ������ �߻��ϴ� �����̴�.PIPE_READMODE_BYTE�̸� �߻����� �ʴ´�.
				// (����!)���� ������(m_pRecvBuffer + m_dwRecvBufferBytes)�� dwAvailableBufferSize��ŭ�� �����ʹ� ����Ǿ������� dwTransferredBytes�� ����� ������ ũ�Ⱑ ��������� �ʴ�.
				// ���� ReadFile�� ��û�� �����ŭ�� ���ŵǾ��ٰ� �����Ѵ�.
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
DWORD CNamedPipe::Send(const char* pData, DWORD dwSize)
{
	DWORD dwPushedSize = 0;

	// ������ SendBuffer ����� ������ ũ��� 4Bytes + dwSize, ����� ������ �ִ��� üũ
	if (m_dwPendingSendBufferBytes + sizeof(DWORD) + dwSize > m_dwSendBufferSize)
	{
		goto lb_return;
	}
	// ���� �����͸� ����� ��ġ
	char* pDest = (char*)(m_pSendBuffer + m_dwPendingSendBufferBytes);

	// ��Ŷ�� ������ 4����Ʈ ��ֱ�
	*(DWORD*)pDest = dwSize;

	// dwSize��ŭ�� ������ ��ü ��ֱ�
	pDest += sizeof(DWORD);
	memcpy(pDest, pData, dwSize);

	
	dwPushedSize = sizeof(DWORD) + dwSize;	// ���������� ����� ����Ʈ ��
	m_dwPendingSendBufferBytes += dwPushedSize;	// SendBuffer�� �׿��ִ� ����Ʈ ��

	if (m_bIsSending)
	{
		// �ռ� ������ �Ϸ�Ǳ⸦ ����Ѵ�.
		int a = 0;
	}
	else
	{
		// ���� WriteFile()�� ���������� ������ ���ۿ� �ִ� ������ �ٷ� ����
		FlushSendBuffer();
	}
lb_return:
	return dwPushedSize;
}
BOOL CNamedPipe::FlushSendBuffer()
{
	//
	// SendBuffer�� �׿��ִ� �����͸� �����ڵ�� ��ִ´�(����)
	//
	memset(&m_OvlSend, 0, sizeof(m_OvlSend));
	DWORD dwTransferredBytes = 0;
	BOOL bResult = WriteFile(m_hPipe, m_pSendBuffer, m_dwPendingSendBufferBytes, &dwTransferredBytes, &m_OvlSend);
	if (bResult)
	{
		// ������ ��� ������ ���, ��� ���μ����� ���� ���� �ʾƵ� out���ۿ� ������ �ִ� ������ ��� �����Ѵ�. 
		OnSendCompleted(dwTransferredBytes);
	}
	else
	{
		// ��� ���μ����� �����͸� ���� �ʾ� out������ ũ�Ⱑ �� á�� ��� 
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
void CNamedPipe::OnRecvCompleted(DWORD dwTransferredBytes)
{
	m_dwRecvBufferBytes += dwTransferredBytes;	// RecvBuffer�� �׿��ִ� ������ ������ ����
	m_bIsReceiving = FALSE; // ReadFile()���� �ƴ�

	// RecvBuffer�κ��� ��Ŷ�� ����
	CollectMsgFromRecvBuffer();
}
void CNamedPipe::OnSendCompleted(DWORD dwTransferredBytes)
{
	//
	// WriteFile�� �Ϸ�� ���. WriteFileȣ�� ���� �Ϸ�Ȱ����� �Ǹ�ǰų� Overlapped I/O�� �Ϸ������ Ȯ���� ���
	//
	DWORD dwRemainedBytes = m_dwPendingSendBufferBytes - dwTransferredBytes;	// ���� ���۵ǰ� ���� ����Ʈ ��
	if (dwRemainedBytes)
	{
		// ���� ����Ʈ ���� ������ �׸�ŭ ������ �� ������ ���ܿ´�.
		memmove(m_pSendBuffer, m_pSendBuffer + dwTransferredBytes, dwRemainedBytes);
	#ifdef _DEBUG
		//OutputDebugString(L"memmove(m_pSendBuffer, m_pSendBuffer + dwTransferredBytes, dwRemainedBytes);\n");
	#endif
	}
	m_dwPendingSendBufferBytes = dwRemainedBytes;	// SendBuffer�� ���� ����Ʈ �� ����
	m_bIsSending = FALSE;
}

void CNamedPipe::CleanupCommon()
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
void CNamedPipe::Cleanup()
{
	if (INVALID_HANDLE_VALUE != m_hPipe)
	{
		if (m_bIsSending)
		{
			CancelIo(m_hPipe);
			// �񵿱� write�۾��� �������� ��� �Ϸ�ɶ����� ���
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
			// �񵿱� write�۾��� �������� ��� �Ϸ�ɶ����� ���
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
CNamedPipe::~CNamedPipe()
{
	Cleanup();
}