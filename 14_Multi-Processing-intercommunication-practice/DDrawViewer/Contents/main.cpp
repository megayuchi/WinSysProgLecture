// DDRAWVIEWER.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include <crtdbg.h>
#include "DDrawViewer.h"
#include "../DDrawLib/DDrawDevice.h"
#include "../DDrawUtil/DDrawUtil.h"
#include "../../../Util/QueryPerfCounter.h"
#include "Viewer.h"

#include <intrin.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL	g_bCanUseWndProcBackup = FALSE;
BOOL	g_bCanUseWndProc = FALSE;
HWND	g_hMainWindow = nullptr;



HANDLE g_hDestroyEvent = nullptr;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DDRAWVIEWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DDRAWVIEWER));

	MSG msg;

	QCInit();

	// DDrawGame.exe로부터 종료 지시를 받기 위한 이벤트 생성
	g_hDestroyEvent = CreateEvent(nullptr, FALSE, FALSE, L"DDrawViewer-Destroy-Event");

	g_pViewer = new CViewer;
	g_pViewer->Initialize(g_hMainWindow);

	// Main message loop:
	while (TRUE)
	{
		// call WndProc
		//g_bCanUseWndProc == FALSE이면 DefWndProc호출
		g_bCanUseWndProc = TRUE;

		BOOL	bHasMsg = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

		if (bHasMsg)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else
		{
			DWORD dwWaitResult = WaitForSingleObject(g_hDestroyEvent, 0);
			if (WAIT_OBJECT_0 == dwWaitResult)
			{
				// 종료 이벤트
				break;
			}
			else if (WAIT_TIMEOUT == dwWaitResult)
			{
				int a = 0;
			}
			
			g_pViewer->Process();

		}
		g_bCanUseWndProc = FALSE;
	}
	if (g_pViewer)
	{
		delete g_pViewer;
		g_pViewer = nullptr;
	}
	if (g_hDestroyEvent)
	{
		CloseHandle(g_hDestroyEvent);
		g_hDestroyEvent = nullptr;
	}
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DDRAWVIEWER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DDRAWVIEWER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	g_hMainWindow = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
			{
				int wmId = LOWORD(wParam);
				// Parse the menu selections:
				switch (wmId)
				{
					case IDM_ABOUT:
						DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
						break;
					case IDM_EXIT:
						DestroyWindow(hWnd);
						break;
					default:
						return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			break;
		case WM_SIZE:
			if (g_pViewer)
			{
				g_pViewer->OnUpdateWindowSize();
			}
			break;
		case WM_MOVE:
			if (g_pViewer)
			{
				g_pViewer->OnUpdateWindowPos();
			}
			break;
		case WM_KEYDOWN:
			{
				
				UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
				UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
				if (!(lParam & 0x40000000))
				{
					if (g_pViewer)
					{
						g_pViewer->OnKeyDown(vkCode, uiScanCode);
					}

				}
			}
			break;
		case WM_KEYUP:
			{
				UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
				UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
				if (g_pViewer)
				{
					g_pViewer->OnKeyUp(vkCode, uiScanCode);
				}
			}
			break;
		case WM_SYSKEYDOWN:
			{
				UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
				UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
				BOOL	bAltKeyDown = FALSE;
				if ((HIWORD(lParam) & KF_ALTDOWN))
				{
					bAltKeyDown = TRUE;
				}
				if (!g_pViewer->OnSysKeyDown(vkCode, uiScanCode, bAltKeyDown))
				{
					DefWindowProc(hWnd, message, wParam, lParam);
				}

			}
			break;
			/*case WM_KEYDOWN:
	  case WM_SYSKEYDOWN:
		  if (wParam == VK_RETURN)
			  if ((HIWORD(lParam) & KF_ALTDOWN))
				  ToggleFullscreen();
		  break;*/

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
