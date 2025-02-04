// DDrawGame.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include <crtdbg.h>
#include "DDrawGame.h"
#include "../DDrawLib/DDrawDevice.h"
#include "../DDrawUtil/DDrawUtil.h"
#include "../DDrawUtil/TGAImage.h"
#include "../../../../Util/QueryPerfCounter.h"
#include "../ImageData/ImageData.h"
#include "Game.h"

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


BOOL StartDDrawViewProcess(HANDLE* phOutProcess, HANDLE* phOutThread);

BOOL	g_bCanUseWndProcBackup = FALSE;
BOOL	g_bCanUseWndProc = FALSE;
HWND	g_hMainWindow = nullptr;

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
	LoadStringW(hInstance, IDC_DDrawGame, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DDrawGame));

	MSG msg;

	QCInit();

	g_pGame = new CGame;
	g_pGame->Initialize(g_hMainWindow, lpCmdLine);

	HANDLE hDDrawViewerProcess = nullptr;
	HANDLE hDDrawViewerThread = nullptr;
	StartDDrawViewProcess(&hDDrawViewerProcess, &hDDrawViewerThread);

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
			g_pGame->Process();

		}
		g_bCanUseWndProc = FALSE;
	}
	if (g_pGame)
	{
		delete g_pGame;
		g_pGame = nullptr;
	}

	
	if (hDDrawViewerProcess)
	{
		// TerminateProcess(hDDrawViewerProcess, -1); // 프로세스 강제종료. 권장하고 싶지 않지만 사용 가능함. hDDrawViewerProcess에 대해 종료되었는지 WaitFor..()함수로 대기 가능.
		
		// DDrawViewer.exe 프로세스에 종료 지시를 내린다.
		HANDLE hDestroyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"DDrawViewer-Destroy-Event");
		if (hDestroyEvent)
		{
			SetEvent(hDestroyEvent);
			CloseHandle(hDestroyEvent);
		}
		
		WaitForSingleObject(hDDrawViewerProcess, INFINITE);	// DDrawViewer.exe 프로세스가 종료될때까지 대기
		CloseHandle(hDDrawViewerProcess);
		hDDrawViewerProcess = nullptr;

		if (hDDrawViewerThread)
		{
			CloseHandle(hDDrawViewerThread);
			hDDrawViewerThread = nullptr;
		}
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DDrawGame));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DDrawGame);
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
			if (g_pGame)
			{
				g_pGame->OnUpdateWindowSize();
			}
			break;
		case WM_MOVE:
			if (g_pGame)
			{
				g_pGame->OnUpdateWindowPos();
			}
			break;
		case WM_KEYDOWN:
			{

				UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
				UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
				if (!(lParam & 0x40000000))
				{
					if (g_pGame)
					{
						g_pGame->OnKeyDown(vkCode, uiScanCode);
					}

				}
			}
			break;
		case WM_KEYUP:
			{
				UINT	uiScanCode = (0x00ff0000 & lParam) >> 16;
				UINT	vkCode = MapVirtualKey(uiScanCode, MAPVK_VSC_TO_VK);
				if (g_pGame)
				{
					g_pGame->OnKeyUp(vkCode, uiScanCode);
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
				if (!g_pGame->OnSysKeyDown(vkCode, uiScanCode, bAltKeyDown))
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

BOOL StartDDrawViewProcess(HANDLE* phOutProcess, HANDLE* phOutThread)
{
	BOOL bResult = FALSE;

	const WCHAR* wchExeName = L"";
#if defined(_M_ARM64EC) || defined(_M_ARM64)
	wchExeName = L"\\DDrawViewer_arm_debug.exe";
#elif defined(_M_AMD64)
	wchExeName = L"\\DDrawViewer_x64_debug.exe";
#elif defined(_M_IX86)
	wchExeName = L"\\DDrawViewer_x86_debug.exe";
#endif

	WCHAR wchFullPath[_MAX_PATH] = {};

	// get full path
	SetCurrentDirectory(L"../../DDrawViewer/Contents/");
	GetCurrentDirectory(_MAX_PATH, wchFullPath);
	wcscat_s(wchFullPath, wchExeName);


	// create DDrawViewer process
	STARTUPINFO si = {};
	PROCESS_INFORMATION pi = {};

	si.cb = sizeof(si);
	DWORD	dwCreationFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
	if (CreateProcess(nullptr, wchFullPath, nullptr, nullptr, FALSE, dwCreationFlag, nullptr, nullptr, &si, &pi))
	{
		if (pi.hProcess)
		{
			*phOutProcess = pi.hProcess;
		}
		if (pi.hThread)
		{
			*phOutThread = pi.hThread;
		}
		bResult = TRUE;
	}
	return bResult;
}