#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <process.h>
#include "FileWriterCommon.h"
#include "FileWriterSyncMT.h"

UINT WINAPI WriteThread(void* pArgs);

CFileWriterSyncMT::CFileWriterSyncMT()
{
}
BOOL __stdcall CFileWriterSyncMT::Initialize(UINT64 ui64MaxFileSize)
{
    m_ui64MaxFileSize = ui64MaxFileSize;

    m_hCompletedEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    for (DWORD i = 0; i < THREAD_EVENT_TYPE_COUNT; i++)
    {
        m_ThreadDesc.hEventList[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }
    m_ThreadDesc.pFileWriter = this;

    UINT uiThreadID = 0;
    m_ThreadDesc.hThread = (HANDLE)_beginthreadex(nullptr, 0, WriteThread, &m_ThreadDesc, 0, &uiThreadID);

    return TRUE;
}
FILE_WRITER_TYPE __stdcall CFileWriterSyncMT::GetType()
{
    return FILE_WRITER_TYPE_SYNC_MT;
}
BOOL __stdcall CFileWriterSyncMT::Begin(const WCHAR* wchFileName)
{
    BOOL bResult = FALSE;
    
    Close();

    // 파일 생성
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
void __stdcall CFileWriterSyncMT::Stop()
{
    Close();
}
void __stdcall CFileWriterSyncMT::Destroy()
{
    delete this;
}

BOOL __stdcall CFileWriterSyncMT::IsWriting() const 
{
    return m_bIsWriting; 
}
BOOL __stdcall CFileWriterSyncMT::Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch)
{
	BOOL bResult = FALSE;

    if (INVALID_HANDLE_VALUE == m_hFile)
        goto lb_return;

    // 앞선 write 스레드의 완료를 대기
    if (m_bIsPending)
    {
        WaitForSingleObject(m_hCompletedEvent, INFINITE);
        m_bIsPending = FALSE;
    }

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

    // write 스레드 깨움
    m_bIsWriting = TRUE;
    m_dwPendingBytes = dwDataSize;
    m_bIsPending = TRUE;
    SetEvent(m_ThreadDesc.hEventList[THREAD_EVENT_TYPE_PROCESS]);
    bResult = TRUE;

lb_return:
    return bResult;
}
void CFileWriterSyncMT::WriteByThread()
{
	DWORD dwWrittenBytes = 0;
    
    DWORD m_dwWriteBufferSize = 0;
    BOOL bResult = WriteFile(m_hFile, m_pWriteBuffer, m_dwPendingBytes, &dwWrittenBytes, nullptr);
    if (!bResult)
    {
        DWORD dwErr = GetLastError();
        __debugbreak();
    }
    m_ui64AccumWrittenBytes += (UINT64)dwWrittenBytes;
    m_dwPendingBytes = 0;

    SetEvent(m_hCompletedEvent);
}
void CFileWriterSyncMT::Close()
{
    // 앞선 write 스레드의 완료를 대기
    if (m_bIsPending)
    {
        WaitForSingleObject(m_hCompletedEvent, INFINITE);
        m_bIsPending = FALSE;
    }

    if (INVALID_HANDLE_VALUE != m_hFile)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    m_bIsWriting = FALSE;
}

void CFileWriterSyncMT::Cleanup()
{
    Close();

    if (m_ThreadDesc.hThread)
    {
        SetEvent(m_ThreadDesc.hEventList[THREAD_EVENT_TYPE_DESTROY]);
        WaitForSingleObject(m_ThreadDesc.hThread, INFINITE);
        CloseHandle(m_ThreadDesc.hThread);
        m_ThreadDesc.hThread = nullptr;
    }
    for (DWORD i = 0; i < THREAD_EVENT_TYPE_COUNT; i++)
    {
        CloseHandle(m_ThreadDesc.hEventList[i]);
        m_ThreadDesc.hEventList[i] = nullptr;
    }
    if (m_hCompletedEvent)
    {
        CloseHandle(m_hCompletedEvent);
        m_hCompletedEvent = nullptr;
    }
    if (m_pWriteBuffer)
    {
        free(m_pWriteBuffer);
        m_pWriteBuffer = nullptr;
    }
}
CFileWriterSyncMT::~CFileWriterSyncMT()
{
    Cleanup();
}

UINT WINAPI WriteThread(void* pArgs)
{
    THREAD_DESC* pThreadDesc = (THREAD_DESC*)pArgs;
    CFileWriterSyncMT* pFileWriter = pThreadDesc->pFileWriter;

    while (1)
    {
        DWORD dwEventIndex = WaitForMultipleObjects(THREAD_EVENT_TYPE_COUNT, pThreadDesc->hEventList, FALSE, INFINITE);
        switch (dwEventIndex)
        {
            case THREAD_EVENT_TYPE_PROCESS:
                pFileWriter->WriteByThread();
                break;
            case THREAD_EVENT_TYPE_DESTROY:
                goto lb_return;
        }
    }
lb_return:
    _endthreadex(0);
    return 0;
}