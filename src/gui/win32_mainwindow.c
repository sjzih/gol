#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "win32_mainwindow.h"

static BOOL mainwnd_oncreate(HWND hwnd, LPCREATESTRUCT lpcreate);
static void mainwnd_onsize(HWND hwnd, int type, int width, int height);
static LRESULT WINAPI mainwnd_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern ATOM
mainwnd_register(HINSTANCE hinst)
{
	WNDCLASSEX wc;
	LPCTSTR MainWndClass = TEXT("Win32 Test application");
	HWND hWnd;
	HACCEL hAccelerators;
	HMENU hSysMenu;

	wc.cbSize        = sizeof(wc);
	wc.style         = 0;
	wc.lpfnWndProc   = mainwnd_proc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinst;
	wc.hIcon         = NULL;
	wc.hCursor       = LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = MAINWND_CLASS;
	wc.hIconSm       = NULL;

	return RegisterClassEx(&wc);
}

static LRESULT WINAPI
mainwnd_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg){
        HANDLE_MSG(hWnd, WM_CREATE, mainwnd_oncreate);
        HANDLE_MSG(hWnd,   WM_SIZE, mainwnd_onsize);
	}
	return DefWindowProc(hWnd,Msg,wParam,lParam);
}

static BOOL 
mainwnd_oncreate(HWND hwnd, LPCREATESTRUCT lpcreate)
{
    HWND hwndButton = CreateWindow( 
            _T("BUTTON"),
            _T("Go"),
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            10,         // x position 
            10,         // y position 
            64,        // Button width
            32,        // Button height
            hwnd,
            NULL,       // No menu.
            NULL,
            NULL
    );
    return TRUE;
}

static void
mainwnd_onsize(HWND hwnd, int type, int width, int height)
{
}
