#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "../Common/typedef.h"
#include "../DDrawUtil/DDrawUtil.h"
#include "../../../Util/QueryPerfCounter.h"
#include "../DDrawLib/DDrawDevice.h"
#include "../../SharedMemoryReader/SharedMemoryReader.h"
#include "../../NamedPipe/NamedPipe.h"
#include "../../DDrawPacket/packet_typedef.h"
#include "Viewer.h"


CViewer* g_pViewer = nullptr;

CViewer::CViewer()
{
	m_dwCurFPS = 0;
	m_fTicksPerGameFrame = 1000.0f / (float)m_dwGameFPS;
	m_PrvGameFrameTick = GetTickCount64();
	m_PrvCounter = QCGetCounter();

}

BOOL CViewer::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	m_pDDraw = new CDDrawDevice;
	m_pDDraw->InitializeDDraw(hWnd);

	WCHAR wchCurPath[_MAX_PATH] = {};
	GetCurrentDirectory(_MAX_PATH, wchCurPath);


	m_pMemoryReader = new CSharedMemoryReader;
	m_pMemoryReader->Initialize(L"DDrawGame-MemoryMap", L"DDrawGame-Mutex", 4096, 4096);

	int iScreenWidth = (int)m_pDDraw->GetWidth();
	int iScreenHeight = (int)m_pDDraw->GetHeight();

	srand(GetTickCount());

	m_pNamedPipeClient = new CNamedPipe;
	m_pNamedPipeClient->InitializeAsClient(512, 512, 1024 * 1024);

	return TRUE;
}
void CViewer::Process()
{
	LARGE_INTEGER	CurCounter = QCGetCounter();
	float	fElpasedTick = QCMeasureElapsedTick(CurCounter, m_PrvCounter);
	ULONGLONG CurTick = GetTickCount64();

	if (fElpasedTick > m_fTicksPerGameFrame)
	{
		if (m_pNamedPipeClient)
		{
			BOOL bOnConnected = FALSE;
			BOOL bMsgExist = FALSE;
			m_pNamedPipeClient->Run(&bOnConnected, &bMsgExist);
		}
		OnGameFrame(CurTick);
		m_PrvGameFrameTick = CurTick;
		m_PrvCounter = CurCounter;
		
	}
	else
	{
		float fAlpha = fElpasedTick / m_fTicksPerGameFrame;
		//InterpolatePostion(fAlpha);
	}

	DrawScene();
}

void CViewer::OnKeyDown(UINT nChar, UINT uiScanCode)
{
	switch (nChar)
	{
		case VK_LEFT:
			{
				
			}
			break;
		case VK_RIGHT:
			{
				
			}
			break;
		case VK_UP:
			{
				
			}
			break;
		case VK_DOWN:
			{
				
			}
			break;
		case VK_SPACE:
			{
				
			}
			break;
		
	}

	// NamedPipe전송 무결성 테스트
	//TestSend();
	
	PACKET_ON_KEYDOWN packet;
	packet.Key = nChar;
	packet.ScanCode = uiScanCode;
	if (m_pNamedPipeClient)
	{
		m_pNamedPipeClient->Send((const char*)&packet, sizeof(packet));
	}
}
void CViewer::OnKeyUp(UINT nChar, UINT uiScanCode)
{
	switch (nChar)
	{
		case VK_LEFT:
			{
				
			}
			break;
		case VK_RIGHT:
			{
				
			}
			break;
		case VK_UP:
			{
				
			}
			break;
		case VK_DOWN:
			{
				
			}
			break;
	}
	PACKET_ON_KEYUP packet;
	packet.Key = nChar;
	packet.ScanCode = uiScanCode;
	if (m_pNamedPipeClient)
	{
		m_pNamedPipeClient->Send((const char*)&packet, sizeof(packet));
	}
}

void CViewer::TestSend()
{
	PACKET_STRING packet;

	const WCHAR* wchTxtArgList[] = { L"A", L"BB", L"CCC", L"HITOMI", L"KASUMI", L"AYANE", L"LEIFANG", L"TINA", L"LISA" };
	DWORD dwTextArgCount = (DWORD)_countof(wchTxtArgList);
	for (DWORD i = 0; i < 100; i++)
	{
		DWORD dwArgIndex = i %	dwTextArgCount;
		const WCHAR* wchTextArg = wchTxtArgList[dwArgIndex];
		DWORD dwLen = wcslen(wchTextArg);
		if (dwLen >= (DWORD)_countof(packet.wchTxt))
			__debugbreak();

		packet.SetText(wchTextArg);
		if (m_pNamedPipeClient->Send((const char*)&packet, packet.GetPacketSize()))
		{
			WCHAR wchTxt[512];
			swprintf_s(wchTxt, L"%s\n", packet.wchTxt);
			OutputDebugString(wchTxt);
		}
		else
		{
			break;
		}
	}

}
BOOL CViewer::OnSysKeyDown(UINT nChar, UINT uiScanCode, BOOL bAltKeyDown)
{
	BOOL	bResult = FALSE;
	switch (nChar)
	{
		case VK_F9:
			{
				if (bAltKeyDown)
				{
				
					bResult = TRUE;

				}

			}
			break;
			

	}
	return bResult;
}

void CViewer::DrawScene()
{
	// 메시지가 없으면 게임루프

	m_pDDraw->BeginDraw();


	m_pDDraw->Clear();

	char* pBits = nullptr;
	DWORD dwWidth = 0;
	DWORD dwHeight = 0;

	if (m_pMemoryReader->Read(&pBits, &dwWidth, &dwHeight))
	{
		m_pDDraw->DrawBitmap(0, 0, dwWidth, dwHeight, pBits);
	}
	
	// mid scroll image
//	m_pDDraw->DrawImageData(m_iMidScrollImagePosX, m_iMidScrollImagePosY, m_pMidScrollImageData);
	

	//m_pDDraw->DrawRect(sx, sy, iBoxWidth, iBoxHeight, 0xff00ff00);

	// Draw loaded image
	
	m_pDDraw->EndDraw();

	HDC	hDC = nullptr;
	if (m_pDDraw->BeginGDI(&hDC))
	{
		m_pDDraw->DrawInfo(hDC);
		m_pDDraw->EndGDI(hDC);
	}
	m_pDDraw->OnDraw();
	m_pDDraw->CheckFPS();
	/*




	// Draw Enemies


	// Draw ammos

	//DrawFPS(1, 1);
	//DrawScore(m_iScreenWidth - 16, 1);

	Blt();
	*/
}
void CViewer::OnUpdateWindowSize()
{
	if (m_pDDraw)
	{
		m_pDDraw->OnUpdateWindowSize();
		//UpdateBackImagePos();
	}
}
void CViewer::OnUpdateWindowPos()
{
	if (m_pDDraw)
	{
		m_pDDraw->OnUpdateWindowPos();
		//		UpdateBackImagePos();
	}
}
void CViewer::OnGameFrame(ULONGLONG CurTick)
{

	int iScreenWidth = (int)m_pDDraw->GetWidth();
	int iScreenHeight = (int)m_pDDraw->GetHeight();
}
void CViewer::Cleanup()
{
	if (m_pNamedPipeClient)
	{
		delete m_pNamedPipeClient;
		m_pNamedPipeClient = nullptr;
	}
	if (m_pMemoryReader)
	{
		delete m_pMemoryReader;
		m_pMemoryReader = nullptr;
	}
	if (m_pDDraw)
	{
		delete m_pDDraw;
		m_pDDraw = nullptr;
	}
}

CViewer::~CViewer()
{
	Cleanup();
}