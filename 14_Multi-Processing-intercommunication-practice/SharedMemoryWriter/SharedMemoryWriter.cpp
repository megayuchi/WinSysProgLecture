#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "SharedMemoryWriter.h"


CSharedMemoryWriter::CSharedMemoryWriter()
{
}
BOOL __stdcall CSharedMemoryWriter::Initialize(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxWidth, DWORD dwMaxHeight)
{
	m_dwMaxMemSize = sizeof(IMAGE_HEADER) - sizeof(DWORD) + dwMaxWidth * dwMaxHeight * 4;
	m_hMemMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_dwMaxMemSize, wchMemoryMappedFileName);

	m_hMutex = CreateMutex(nullptr, FALSE, wchMutexName);

	m_pWriteBuffer = (char*)MapViewOfFile(m_hMemMap, FILE_MAP_WRITE, 0, 0, m_dwMaxMemSize);
	if (!m_pWriteBuffer)
	{
		__debugbreak();
	}

	return TRUE;
}

BOOL __stdcall CSharedMemoryWriter::Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch)
{
	BOOL bResult = FALSE;

	DWORD dwRequiredMemSize = dwWidth * dwHeight * 4;

	if (dwRequiredMemSize > m_dwMaxMemSize)
	{
	#ifdef _DEBUG
		__debugbreak();
	#endif
		goto lb_return;
	}
	// lock buffer access
	WaitForSingleObject(m_hMutex, INFINITE);

	IMAGE_HEADER* pImage = (IMAGE_HEADER*)m_pWriteBuffer;
	
	const char* pSrc = pImageData;
	DWORD dwDestPitch = dwWidth * 4;
	char* pDest = (char*)pImage->pData;

	pImage->dwWidth = dwWidth;
	pImage->dwHeight = dwHeight;
	pImage->dwPitch = dwDestPitch;
	
	for (DWORD y = 0; y < dwHeight; y++)
	{
		memcpy(pDest, pSrc, dwWidth * 4);
		pSrc += dwPitch;
		pDest += dwDestPitch;
	}

	// unlock buffer access
	ReleaseMutex(m_hMutex);
	
lb_return:
	return bResult;
}

void CSharedMemoryWriter::Cleanup()
{
	if (m_pWriteBuffer)
	{
		UnmapViewOfFile(m_pWriteBuffer);
		m_pWriteBuffer = nullptr;
	}
	if (m_hMemMap)
	{		
		CloseHandle(m_hMemMap);
		m_hMemMap = nullptr;
	}
	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = nullptr;
	}
}
CSharedMemoryWriter::~CSharedMemoryWriter()
{
	Cleanup();
}