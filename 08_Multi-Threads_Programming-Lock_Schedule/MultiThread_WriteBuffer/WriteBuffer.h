#pragma once

class CWriteBuffer
{
	BYTE* m_pBuffer = nullptr;
	DWORD m_dwWritePos = 0;
	DWORD m_dwMaxBufferSize = 0;
	CRITICAL_SECTION	m_cs = {};
	SRWLOCK	m_SRWLock = {};
	LONG volatile m_lLockCount = 0;

	void	Cleanup();
public:
	void	Initialize(DWORD dwMaxBufferSize);
	DWORD	PushWithLockBuffer_CS(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize);
	DWORD	PushWithLockBuffer_SRWLock(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize);
	DWORD	PushWithLockBuffer_SpinLock(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize);
	DWORD	PushWithLockAddress(BOOL* pbOutBufferFull, const BYTE* pData, DWORD dwSize);
	UINT64	CalcSum();

	void	Clear();
	CWriteBuffer();
	~CWriteBuffer();
};

