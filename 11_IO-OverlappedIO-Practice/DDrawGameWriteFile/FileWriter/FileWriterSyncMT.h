#pragma once

#include "IFileWriter.h"

class CFileWriterSyncMT;

enum THREAD_EVENT_TYPE
{
	THREAD_EVENT_TYPE_PROCESS,
	THREAD_EVENT_TYPE_DESTROY,
	THREAD_EVENT_TYPE_COUNT
};
struct THREAD_DESC
{
	CFileWriterSyncMT* pFileWriter;
	HANDLE hThread;
	HANDLE hEventList[THREAD_EVENT_TYPE_COUNT];	
};
class CFileWriterSyncMT : public IFileWriter
{
	HANDLE m_hFile = INVALID_HANDLE_VALUE;
	char* m_pWriteBuffer = nullptr;
	DWORD m_dwWriteBufferSize = 0;
	UINT64 m_ui64AccumWrittenBytes = 0;
	UINT64 m_ui64MaxFileSize = 0;
	BOOL	m_bIsWriting = FALSE;

	THREAD_DESC	m_ThreadDesc = {};
	HANDLE	m_hCompletedEvent = nullptr;
	DWORD	m_dwPendingBytes = 0;
	BOOL	m_bIsPending = FALSE;
	void	Close();

	void	Cleanup();

public:
	BOOL	__stdcall Initialize(UINT64 ui64MaxFileSize);
	BOOL	__stdcall Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch);
	BOOL	__stdcall IsWriting() const;
	BOOL	__stdcall Begin(const WCHAR* wchFileName);
	void	__stdcall Stop();
	void	__stdcall Destroy();
	FILE_WRITER_TYPE __stdcall GetType();

	void WriteByThread();
	CFileWriterSyncMT();
	~CFileWriterSyncMT();
};
