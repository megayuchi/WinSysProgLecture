#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "FileReader.h"

CFileReader::CFileReader()
{
}
BOOL CFileReader::Initialize(const WCHAR* wchFileName)
{
    BOOL bResult = FALSE;
    // 파일 생성
    m_hFile = CreateFile(wchFileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (INVALID_HANDLE_VALUE == m_hFile)
    {
        goto lb_return;
    }
    bResult = TRUE;
lb_return:
    return bResult;
}
void CFileReader::SetFilePosBegin()
{
    LARGE_INTEGER DistanceToMove = {};
    SetFilePointerEx(m_hFile, DistanceToMove, nullptr, FILE_BEGIN);
}
BOOL CFileReader::Read(char** ppOutImageBuffer, DWORD* pdwOutWidth, DWORD* pdwOutHeight, BOOL* pbOutEOF)
{
	BOOL bResult = FALSE;

    FILE_IMAGE_HEADER_READ header = {};
    DWORD dwReadSize = 0;

    if (INVALID_HANDLE_VALUE == m_hFile)
        goto lb_return;

    if (ReadFile(m_hFile, &header, sizeof(header), &dwReadSize, nullptr))
    {
        if (!dwReadSize)
        {
            *pbOutEOF = TRUE;
            goto lb_return;
        }
    }
    if (header.dwWidth > 4096)
        __debugbreak();
    if (header.dwHeight > 4096)
        __debugbreak();

    DWORD dwDataSize = header.dwWidth * header.dwHeight * 4;
	if (dwDataSize)
	{
		if (m_dwReadBufferSize < dwDataSize)
		{
			if (m_pReadBuffer)
			{
				free(m_pReadBuffer);
				m_pReadBuffer = nullptr;
			}
			m_pReadBuffer = (char*)malloc(dwDataSize);
			m_dwReadBufferSize = dwDataSize;
		}
        if (ReadFile(m_hFile, m_pReadBuffer, dwDataSize, &dwReadSize, nullptr))
        {
            if (!dwReadSize)
            {
                *pbOutEOF = TRUE;
                goto lb_return;
            }
        }
	}
    *ppOutImageBuffer = m_pReadBuffer;
    *pdwOutWidth = header.dwWidth;
    *pdwOutHeight = header.dwHeight;
    bResult = TRUE;
lb_return:
    return bResult;
    
}
void CFileReader::Close()
{
    if (INVALID_HANDLE_VALUE != m_hFile)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}
CFileReader::~CFileReader()
{
    Close();

    if (m_pReadBuffer)
    {
        free(m_pReadBuffer);
        m_pReadBuffer = nullptr;
    }
}