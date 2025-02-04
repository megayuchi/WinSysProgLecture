#pragma once

class CDDrawDevice;
class CSharedMemoryReader;
interface INamedPipeLib;
class CViewer
{
	CDDrawDevice*	m_pDDraw = nullptr;
	CSharedMemoryReader*	m_pMemoryReader = nullptr;
	HMODULE		m_hNamedPipeLibDLL = nullptr;
	INamedPipeLib* m_pNamedPipeClient = nullptr;
	HWND	m_hWnd = nullptr;
	DWORD m_dwGameFPS = 60;
	DWORD m_dwCurFPS = 0;
	float m_fTicksPerGameFrame = 16.6f;
	ULONGLONG m_PrvGameFrameTick = 0;
	LARGE_INTEGER	m_PrvCounter = {};
	BOOL m_bUsePostEffect = FALSE;
	BOOL m_bWriteToFile = FALSE;
	

	char* m_pRecvBuffer = nullptr;
	DWORD m_dwRecvBufferSize = 0;

	
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
	
	BOOL	InitNamedPipeLib();
	void	CleanupNamedPipeLib();

	void	TestSend();
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
