#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "FileWriterCommon.h"
#include "FileWriterSync.h"

CFileWriterSync::CFileWriterSync()
{
}
BOOL __stdcall CFileWriterSync::Initialize(UINT64 ui64MaxFileSize)
{
    m_ui64MaxFileSize = ui64MaxFileSize;
    return TRUE;
}
FILE_WRITER_TYPE __stdcall CFileWriterSync::GetType()
{
    return FILE_WRITER_TYPE_SYNC;
}
BOOL __stdcall CFileWriterSync::Begin(const WCHAR* wchFileName)
{
    BOOL bResult = FALSE;
    // 파일 생성
    Close();
    m_hFile = CreateFile(wchFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, nullptr);

    if (INVALID_HANDLE_VALUE == m_hFile)
    {
        __debugbreak();
        goto lb_return;
    }
    m_ui64AccumWrittenBytes = 0;
    bResult = TRUE;
lb_return:
    return bResult;
}
void __stdcall CFileWriterSync::Stop()
{
    Close();
}
void __stdcall CFileWriterSync::Destroy()
{
    delete this;
}

BOOL __stdcall CFileWriterSync::IsWriting() const 
{
    return m_bIsWriting; 
}
BOOL __stdcall CFileWriterSync::Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch)
{
	BOOL bResult = FALSE;
	DWORD dwWrittenBytes = 0;

	DWORD dwDataSize = sizeof(FILE_IMAGE_HEADER_WRITE) - sizeof(DWORD) + (dwWidth * dwHeight * 4);
    if (m_dwWriteBufferSize < dwDataSize)
    {
        if (m_pWriteBuffer)
        {
            free(m_pWriteBuffer);
            m_pWriteBuffer = nullptr;
        }
        m_pWriteBuffer = (char*)malloc(dwDataSize);
        m_dwWriteBufferSize = dwDataSize;
    }
    FILE_IMAGE_HEADER_WRITE* pHeader = (FILE_IMAGE_HEADER_WRITE*)m_pWriteBuffer;
    pHeader->dwWidth = dwWidth;
    pHeader->dwHeight = dwHeight;
    const char* pSrc = pImageData;
    char* pDest = (char*)pHeader->pData;
    DWORD dwDestPitch = dwWidth * 4;
    for (DWORD y = 0; y < dwHeight; y++)
    {
        memcpy(pDest, pSrc, dwWidth * 4);
        pSrc += dwPitch;
        pDest += dwDestPitch;
    }
    
    if (INVALID_HANDLE_VALUE != m_hFile)
    {
        m_bIsWriting = TRUE;
        DWORD m_dwWriteBufferSize = 0;
        bResult = WriteFile(m_hFile, m_pWriteBuffer, dwDataSize, &dwWrittenBytes, nullptr);
        if (!bResult)
        {
            DWORD dwErr = GetLastError();
            __debugbreak();
        }
        m_ui64AccumWrittenBytes += (UINT64)dwWrittenBytes;
        if (m_ui64AccumWrittenBytes >= m_ui64MaxFileSize)
        {
            Close();
        }
    }
    
lb_return:
    return bResult;
}
void CFileWriterSync::Close()
{
    if (INVALID_HANDLE_VALUE != m_hFile)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    m_bIsWriting = FALSE;
}
void CFileWriterSync::Cleanup()
{
    Close();

    if (m_pWriteBuffer)
    {
        free(m_pWriteBuffer);
        m_pWriteBuffer = nullptr;
    }
}
CFileWriterSync::~CFileWriterSync()
{
    Cleanup();
}