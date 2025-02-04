#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "SharedMemoryReader.h"

CSharedMemoryReader::CSharedMemoryReader()
{
}
BOOL CSharedMemoryReader::Initialize(const WCHAR* wchMemoryMappedFileName, const WCHAR* wchMutexName, DWORD dwMaxWidth, DWORD dwMaxHeight)
{
    m_dwMaxWidth = dwMaxWidth;
    m_dwMaxHeight = dwMaxHeight;
    m_dwMaxBufferSize = m_dwMaxWidth * m_dwMaxHeight * 4;
    
    m_hMemMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, wchMemoryMappedFileName);
    if (!m_hMemMap)
    {
        __debugbreak();
    }

    m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, wchMutexName);
    if (!m_hMutex)
    {
        __debugbreak();
    }
    m_pBuffer = (char*)malloc(m_dwMaxBufferSize);
    memset(m_pBuffer, 0, m_dwMaxBufferSize);

    m_pSharedMemory = (char*)MapViewOfFile(m_hMemMap, FILE_MAP_ALL_ACCESS, 0, 0, m_dwMaxBufferSize);
    return TRUE;
}
BOOL CSharedMemoryReader::Read(char** ppOutImageBuffer, DWORD* pdwOutWidth, DWORD* pdwOutHeight)
{
    BOOL bResult = FALSE;

    if (!m_hMemMap)
        goto lb_return;

    if (!m_pSharedMemory)
        goto lb_return;


    WaitForSingleObject(m_hMutex, INFINITE);

    IMAGE_HEADER* pImage = (IMAGE_HEADER*)m_pSharedMemory;
    DWORD dwRequiredMemSize = pImage->dwWidth * pImage->dwHeight * 4;
    if (dwRequiredMemSize > m_dwMaxBufferSize)
        goto lb_release_mutex;

    if (pImage->dwWidth > m_dwMaxWidth)
        __debugbreak();

    if (pImage->dwHeight > m_dwMaxHeight)
        __debugbreak();

    char* pDest = m_pBuffer;
    const char* pSrc = (const char*)pImage->pData;
    DWORD dwDestPitch = pImage->dwWidth * 4;
    for (DWORD y = 0; y < pImage->dwHeight; y++)
    {
        memcpy(pDest, pSrc, pImage->dwWidth * 4);
        pSrc += pImage->dwPitch;
        pDest += dwDestPitch;
    }

    *ppOutImageBuffer = m_pBuffer;
    *pdwOutWidth = pImage->dwWidth;
    *pdwOutHeight = pImage->dwHeight;
    bResult = TRUE;

lb_release_mutex:
    ReleaseMutex(m_hMutex);
lb_return:
    return bResult;
    
}
void CSharedMemoryReader::Cleanup()
{
    if (m_pBuffer)
    {
        free(m_pBuffer);
        m_pBuffer = nullptr;
    }

    if (m_pSharedMemory)
    {
        UnmapViewOfFile(m_pSharedMemory);
        m_pSharedMemory = nullptr;
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
CSharedMemoryReader::~CSharedMemoryReader()
{
    Cleanup();
}