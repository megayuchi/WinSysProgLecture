#pragma once

class CDDrawDevice;
class CFileReader;
class CViewer
{
	CDDrawDevice*	m_pDDraw = nullptr;
	CFileReader*	m_pFileReader = nullptr;
	HWND	m_hWnd = nullptr;
	DWORD m_dwGameFPS = 60;
	DWORD m_dwCurFPS = 0;
	float m_fTicksPerGameFrame = 16.6f;
	ULONGLONG m_PrvGameFrameTick = 0;
	LARGE_INTEGER	m_PrvCounter = {};
	BOOL m_bUsePostEffect = FALSE;
	BOOL m_bWriteToFile = FALSE;
	

	char* m_pReadBuffer = nullptr;
	DWORD m_dwReadBufferSize = 0;

	//CTGAImage*	m_pCircleImage = nullptr;
	//CImageData*	m_pCircleImgData = nullptr;


	
	



	int	m_iCursorPosX = 0;
	int m_iCursorPosY = 0;

//	int m_iPlayerPosX = 0;
//	int m_iPlayerPosY = 0;
	int m_iBackImagePosX = 0;
	int m_iBackImagePosY = 0;

	int m_iMidScrollImagePosX = 0;
	int m_iMidScrollImagePosY = 0;

	BOOL m_bKeyDown_Up = FALSE;
	BOOL m_bKeyDown_Down = FALSE;
	BOOL m_bKeyDown_Left = FALSE;
	BOOL m_bKeyDown_Right = FALSE;

	// control game status
	void OnGameFrame(ULONGLONG CurTick);
	
	// display
	void DrawScene();
	

	// keyboard input
	

public:

	BOOL	Initialize(HWND hWnd);
	void	OnKeyDown(UINT nChar, UINT uiScanCode);
	void	OnKeyUp(UINT nChar, UINT uiScanCode);
	BOOL	OnSysKeyDown(UINT nChar, UINT uiScanCode, BOOL bAltKeyDown);
	void	OnUpdateWindowSize();
	void	OnUpdateWindowPos();
	void Process();
	void Cleanup();

	CViewer();
	~CViewer();
};

extern CViewer* g_pViewer;
