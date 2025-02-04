#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "../Common/typedef.h"
#include "../DDrawUtil/DDrawUtil.h"
#include "../../../Util/QueryPerfCounter.h"
#include "../DDrawLib/DDrawDevice.h"
#include "../FileReader/FileReader.h"
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

	const WCHAR* wchFileName = L"../../Data/test.mp0";
	//const WCHAR* wchFileName = L"d:\\Data\\test.mp0";
	//WCHAR wchFullPath[_MAX_PATH] = {};
	//swprintf_s(wchFullPath, L"%s\\%s", wchCurPath, wchFileName);

	m_pFileReader = new CFileReader;
	m_pFileReader->Initialize(wchFileName);

	int iScreenWidth = (int)m_pDDraw->GetWidth();
	int iScreenHeight = (int)m_pDDraw->GetHeight();

	srand(GetTickCount());

	return TRUE;
}
void CViewer::Process()
{
	LARGE_INTEGER	CurCounter = QCGetCounter();
	float	fElpasedTick = QCMeasureElapsedTick(CurCounter, m_PrvCounter);
	ULONGLONG CurTick = GetTickCount64();

	if (fElpasedTick > m_fTicksPerGameFrame)
	{
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
				m_bKeyDown_Left = TRUE;
			}
			break;
		case VK_RIGHT:
			{
				m_bKeyDown_Right = TRUE;
			}
			break;
		case VK_UP:
			{
				m_bKeyDown_Up = TRUE;
			}
			break;
		case VK_DOWN:
			{
				m_bKeyDown_Down = TRUE;
			}
			break;
		case VK_SPACE:
			{
				
			}
			break;
		case VK_RETURN:
			{
				//CaptureBackBuffer("backbuffer.tga");
			}
			break;
	}
}
void CViewer::OnKeyUp(UINT nChar, UINT uiScanCode)
{
	switch (nChar)
	{
		case VK_LEFT:
			{
				m_bKeyDown_Left = FALSE;
			}
			break;
		case VK_RIGHT:
			{
				m_bKeyDown_Right = FALSE;
			}
			break;
		case VK_UP:
			{
				m_bKeyDown_Up = FALSE;
			}
			break;
		case VK_DOWN:
			{
				m_bKeyDown_Down = FALSE;
			}
			break;
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

	BOOL bEOF = FALSE;
	if (m_pFileReader->Read(&pBits, &dwWidth, &dwHeight, &bEOF))
	{
		m_pDDraw->DrawBitmap(0, 0, dwWidth, dwHeight, pBits);
	}
	if (bEOF)
	{
		m_pFileReader->SetFilePosBegin();
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
	if (m_pFileReader)
	{
		delete m_pFileReader;
		m_pFileReader = nullptr;
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