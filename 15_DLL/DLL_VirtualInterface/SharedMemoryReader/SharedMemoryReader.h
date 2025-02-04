#pragma once

struct IMAGE_HEADER
{
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwPitch;
	DWORD pData[1];
};
class CSharedMemoryReader
{
	DWORD m_dwMaxWidth = 0;
	DWORD m_dwMaxHeight = 0;
	HANDLE m_hMemMap = nullptr;
	HANDLE m_hMutex = nullptr;
	char* m_pBuffer = nullptr;
	DWORD m_dwMaxBufferSize = 0;
	char* m_pSharedMemory = nullptr;
	void Cleanup();
public:
	BOOL	Initialize(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxWidth, DWORD dwMaxHeight);
	BOOL	Read(char** ppOutImageBuffer, DWORD* pdwOutWidth, DWORD* pdwOutHeight);
	CSharedMemoryReader();
	~CSharedMemoryReader();
};
