#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "FileWriterCommon.h"
#include "FileWriterAsync.h"
#include "../../../Util/QueryPerfCounter.h"

CFileWriterAsync::CFileWriterAsync()
{
}
BOOL __stdcall CFileWriterAsync::Initialize(UINT64 ui64MaxFileSize)
{
    m_ui64MaxFileSize = ui64MaxFileSize;


    return TRUE;
}
FILE_WRITER_TYPE __stdcall CFileWriterAsync::GetType()
{
    return FILE_WRITER_TYPE_ASYNC;
}
BOOL __stdcall CFileWriterAsync::Begin(const WCHAR* wchFileName)
{
    BOOL bResult = FALSE;
    // 파일 생성
    Close();
    
    
    //FILE_FLAG_WRITE_THROUGH
	m_hFile = CreateFile(wchFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH, nullptr);
    if (INVALID_HANDLE_VALUE == m_hFile)
    {
        __debugbreak();
        goto lb_return;
    }

    if (m_Overlapped.hEvent)
        __debugbreak();

    m_Overlapped.Offset = (DWORD)(-1);
    m_Overlapped.OffsetHigh = (DWORD)(-1);
    //m_Overlapped.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    m_ui64AccumWrittenBytes = 0;
    bResult = TRUE;
lb_return:
    return bResult;
}
void __stdcall CFileWriterAsync::Stop()
{
    Close();
}
void __stdcall CFileWriterAsync::Destroy()
{
    delete this;
}

BOOL __stdcall CFileWriterAsync::IsWriting() const 
{
    return m_bIsWriting; 
}
BOOL __stdcall CFileWriterAsync::Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch)
{
	BOOL bResult = FALSE;
    
    if (INVALID_HANDLE_VALUE == m_hFile)
        goto lb_return;

    // 이전에 요청한 WRITE작업이 끝날때까지 대기
    if (m_bIsPending)
    {
        DWORD dwTransferredBytes = 0;
        WaitForFlush(&dwTransferredBytes);
        m_ui64AccumWrittenBytes += (UINT64)dwTransferredBytes;
        m_bIsPending = FALSE;
    }
       
    // 파일에 써넣은 사이즈가 최대치에 도달하면 파일을 닫는다.
	if (m_ui64AccumWrittenBytes >= m_ui64MaxFileSize)
	{
		Close();
        goto lb_return;
	}

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
    
    

    // 파일에 써넣기 시도
    m_bIsWriting = TRUE;
    DWORD m_dwWriteBufferSize = 0;

    m_Overlapped.Offset = (DWORD)(-1);
    m_Overlapped.OffsetHigh = (DWORD)(-1);
    m_Overlapped.Internal = 0;
	m_Overlapped.InternalHigh = 0;
    m_dwPendingBytes = 0;
    DWORD dwWrittenBytes = 0;
	bResult = WriteFile(m_hFile, m_pWriteBuffer, dwDataSize, &dwWrittenBytes, &m_Overlapped);
    if (bResult)
    {
        // 즉시 write가 완료된 경우
        m_ui64AccumWrittenBytes += (UINT64)dwWrittenBytes;
        m_bIsPending = FALSE;
    }
    else
    {
        // write 작업이 pending된 경우
        DWORD dwErr = GetLastError();
        if (ERROR_IO_PENDING == dwErr)
        {
            m_dwPendingBytes = dwDataSize;
            m_bIsPending = TRUE;
            bResult = TRUE;
        }
        else
        {
            __debugbreak();
        }
    }
lb_return:
    return bResult;
}
BOOL CFileWriterAsync::WaitForFlush(DWORD* pdwOutTranferredBytes)
{
    BOOL bResult = FALSE;
    DWORD dwTransferredBytes = 0;

    if (GetOverlappedResult(m_hFile, &m_Overlapped, &dwTransferredBytes, TRUE))
    {
        if (m_dwPendingBytes != dwTransferredBytes)
            __debugbreak();

        bResult = TRUE;
    }
    else
    {
        
        DWORD dwErr = GetLastError();
        if (ERROR_IO_INCOMPLETE == dwErr)
        {
            int a = 0;
        }
        else
        {
            __debugbreak();
        }
    }
    *pdwOutTranferredBytes = dwTransferredBytes;

    return bResult;

}

void CFileWriterAsync::Close()
{
    if (INVALID_HANDLE_VALUE != m_hFile)
    {
        DWORD dwTransferredBytes = 0;
        if (m_bIsPending)
        {
            WaitForFlush(&dwTransferredBytes);
            m_bIsPending = FALSE;
        }
        if (m_Overlapped.hEvent)
        {
            CloseHandle(m_Overlapped.hEvent);
            m_Overlapped.hEvent = nullptr;
        }
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    m_bIsWriting = FALSE;
}
void CFileWriterAsync::Cleanup()
{
    Close();

    if (m_pWriteBuffer)
    {
        free(m_pWriteBuffer);
        m_pWriteBuffer = nullptr;
    }
}
CFileWriterAsync::~CFileWriterAsync()
{
    Cleanup();
}