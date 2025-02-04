#pragma once

struct FILE_IMAGE_HEADER_READ
{
	DWORD dwWidth;
	DWORD dwHeight;
};
class CFileReader
{
	HANDLE m_hFile = INVALID_HANDLE_VALUE;
	char* m_pReadBuffer = nullptr;
	DWORD m_dwReadBufferSize = 0;
	

public:
	BOOL	Initialize(const WCHAR* wchFileName);
	BOOL	Read(char** ppOutImageBuffer, DWORD* pdwOutWidth, DWORD* pdwOutHeight, BOOL* pbOutEOF);
	void	SetFilePosBegin();
	void	Close();
	CFileReader();
	~CFileReader();
};
