#pragma once

struct IMAGE_HEADER
{
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwPitch;
	DWORD pData[1];
};

class CSharedMemoryWriter
{
	HANDLE m_hMemMap = nullptr;
	DWORD m_dwMaxMemSize = 0;
	char* m_pWriteBuffer = nullptr;
	DWORD m_dwWriteBufferSize = 0;	
	HANDLE m_hMutex = nullptr;

	void	Cleanup();
public:
	BOOL	__stdcall Initialize(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxWidth, DWORD dwMaxHeight);
	BOOL	__stdcall Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch);

	CSharedMemoryWriter();
	~CSharedMemoryWriter();
};
