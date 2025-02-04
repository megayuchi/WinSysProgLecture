#pragma once

enum FILE_WRITER_TYPE
{
	FILE_WRITER_TYPE_SYNC,
	FILE_WRITER_TYPE_SYNC_MT,
	FILE_WRITER_TYPE_ASYNC,
	FILE_WRITER_TYPE_COUNT
};
interface IFileWriter
{
	virtual BOOL __stdcall Initialize(UINT64 ui64MaxFileSize) = 0;
	virtual BOOL __stdcall Write(const char* pImageData, DWORD dwWidth, DWORD dwHeight, DWORD dwPitch) = 0;
	
	virtual BOOL __stdcall IsWriting() const = 0;
	virtual BOOL __stdcall Begin(const WCHAR* wchFileName) = 0;
	virtual void __stdcall Stop() = 0;
	virtual void __stdcall Destroy() = 0;
	virtual FILE_WRITER_TYPE __stdcall GetType() = 0;
};