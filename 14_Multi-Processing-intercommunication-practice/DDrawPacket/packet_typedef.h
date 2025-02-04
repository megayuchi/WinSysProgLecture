#pragma once

enum PIPE_PACKET_PROTOCOL
{
	PIPE_PACKET_PROTOCOL_ON_KEYDOWN,
	PIPE_PACKET_PROTOCOL_ON_KEYUP,
	PIPE_PACKET_PROTOCOL_STRING,
	PIPE_PACKET_PROTOCOL_COUNT
};

struct PACKET_ON_KEYDOWN
{
	DWORD dwPTCL;
	UINT Key;
	UINT ScanCode;
	PACKET_ON_KEYDOWN() { dwPTCL = PIPE_PACKET_PROTOCOL_ON_KEYDOWN; }
};
struct PACKET_ON_KEYUP
{
	DWORD dwPTCL;
	UINT Key;
	UINT ScanCode;
	PACKET_ON_KEYUP() { dwPTCL = PIPE_PACKET_PROTOCOL_ON_KEYUP; }
};

struct PACKET_STRING
{
	DWORD dwPTCL;
	DWORD dwTxtLen;
	WCHAR wchTxt[512];
	DWORD GetPacketSize()
	{
		return (DWORD)((DWORD_PTR)(void*)wchTxt - (DWORD_PTR)(void*)this) + (DWORD)((dwTxtLen+1) * sizeof(WCHAR));	// 터미널 문자까지 포함한다.
	}
	void SetText(const WCHAR* wchInTxt)
	{
		DWORD len = (DWORD)wcslen(wchInTxt);
		if (len >= (DWORD)_countof(wchTxt))
			__debugbreak();

		memcpy(wchTxt, wchInTxt, sizeof(WCHAR) * len);
		wchTxt[len] = 0;
		dwTxtLen = len;
	}

	PACKET_STRING() { dwPTCL = PIPE_PACKET_PROTOCOL_STRING; }
};