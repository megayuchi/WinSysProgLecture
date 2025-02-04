#include "pch.h"
#include "../../Util/SpinLock.h"
#include "WriteBuffer.h"

CWriteBuffer::CWriteBuffer()
{
}
void CWriteBuffer::Initialize(DWORD dwMaxBufferSize)
{
	m_pBuffer = new BYTE[dwMaxBufferSize];
	memset(m_pBuffer, 0, dwMaxBufferSize);
	m_dwMaxBufferSize = dwMaxBufferSize;

	InitializeCriticalSection(&m_cs);
	InitializeSRWLock(&m_SRWLock);
}
DWORD CWriteBuffer::PushWithLockBuffer_CS(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize)
{
	BOOL	bBufferFull = FALSE;
	DWORD	dwCpySize = dwSize;
	
	EnterCriticalSection(&m_cs);

	if (m_dwWritePos + dwCpySize > m_dwMaxBufferSize)
	{
		DWORD dwExceedSize = (m_dwWritePos + dwCpySize) - m_dwMaxBufferSize;
		dwCpySize -= dwExceedSize;
		bBufferFull = TRUE;
	}
	
	if (dwCpySize)
	{
		BYTE*	pDest = m_pBuffer + m_dwWritePos;
		memcpy(pDest, pData, dwCpySize);
		m_dwWritePos += dwCpySize;
	}

	LeaveCriticalSection(&m_cs);
	
	*pbOutBufferFull = bBufferFull;
	return dwCpySize;

}
DWORD CWriteBuffer::PushWithLockBuffer_SRWLock(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize)
{
	BOOL	bBufferFull = FALSE;
	DWORD	dwCpySize = dwSize;
	
	AcquireSRWLockExclusive(&m_SRWLock);

	if (m_dwWritePos + dwCpySize > m_dwMaxBufferSize)
	{
		DWORD dwExceedSize = (m_dwWritePos + dwCpySize) - m_dwMaxBufferSize;
		dwCpySize -= dwExceedSize;
		bBufferFull = TRUE;
	}
	
	if (dwCpySize)
	{
		BYTE*	pDest = m_pBuffer + m_dwWritePos;
		memcpy(pDest, pData, dwCpySize);
		m_dwWritePos += dwCpySize;
	}
	
	ReleaseSRWLockExclusive(&m_SRWLock);
	
	*pbOutBufferFull = bBufferFull;
	return dwCpySize;

}
DWORD CWriteBuffer::PushWithLockBuffer_SpinLock(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize)
{
	BOOL	bBufferFull = FALSE;
	DWORD	dwCpySize = dwSize;
	
	AcquireSpinLock(&m_lLockCount);	// Lock

	if (m_dwWritePos + dwCpySize > m_dwMaxBufferSize)
	{
		DWORD dwExceedSize = (m_dwWritePos + dwCpySize) - m_dwMaxBufferSize;
		dwCpySize -= dwExceedSize;
		bBufferFull = TRUE;
	}
	
	if (dwCpySize)
	{
		BYTE*	pDest = m_pBuffer + m_dwWritePos;
		memcpy(pDest, pData, dwCpySize);
		m_dwWritePos += dwCpySize;
	}
	
	ReleaseSpinLock(&m_lLockCount);	// Unlock
	
	*pbOutBufferFull = bBufferFull;
	return dwCpySize;

}

DWORD CWriteBuffer::PushWithLockAddress(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize)
{
	BOOL	bBufferFull = FALSE;
	DWORD	dwCpySize = dwSize;
	BYTE*	pDest = nullptr;

	AcquireSpinLock(&m_lLockCount);	// Lock

	if (m_dwWritePos + dwCpySize > m_dwMaxBufferSize)
	{
		DWORD dwExceedSize = (m_dwWritePos + dwCpySize) - m_dwMaxBufferSize;
		dwCpySize -= dwExceedSize;
		bBufferFull = TRUE;
	}
	pDest = m_pBuffer + m_dwWritePos;
	m_dwWritePos += dwCpySize;
	
	ReleaseSpinLock(&m_lLockCount);	// Unlock

	if (dwCpySize)
	{
		memcpy(pDest, pData, dwCpySize);
	}
	
	*pbOutBufferFull = bBufferFull;
	return dwCpySize;

}
void CWriteBuffer::Clear()
{
	memset(m_pBuffer, 0, m_dwMaxBufferSize);
	m_dwWritePos = 0;
}
UINT64 CWriteBuffer::CalcSum()
{
	UINT64	Sum = 0;
	for (DWORD i = 0; i < m_dwMaxBufferSize; i++)
	{
		Sum += (UINT64)m_pBuffer[i];
	}
	return Sum;
}
void CWriteBuffer::Cleanup()
{
	DeleteCriticalSection(&m_cs);
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = nullptr;
	}
}
CWriteBuffer::~CWriteBuffer()
{
	Cleanup();
}