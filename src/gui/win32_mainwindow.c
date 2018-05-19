#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "gui.h"
#include "win32_mainwindow.h"

#define BTN_GO_WIDTH  64
#define BTN_GO_HEIGHT 32
#define TEXT_NITER_WIDTH  90
#define TEXT_NITER_HEIGHT 32

#define PADDING 8

static HWND hbutton_g;
static HWND htext_g;
static HWND hedit_g;
static HWND hvideo_g;

GUI_Window VIDEO_WINDOW;

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
    hbutton_g = CreateWindow( 
            _T("BUTTON"), _T("Go"), 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            0, 0, BTN_GO_WIDTH, BTN_GO_HEIGHT,
            hwnd, NULL, NULL, NULL
    );
    htext_g = CreateWindow(
            _T("STATIC"), _T("Number of iterations"),
            WS_VISIBLE | WS_CHILD,  // Styles 
            0, 0, TEXT_NITER_WIDTH, TEXT_NITER_HEIGHT,
            hwnd, NULL, NULL, NULL
    );
    hvideo_g = CreateWindow(
            _T("STATIC"), _T(""),
            WS_BORDER | WS_VISIBLE | WS_CHILD,  // Styles 
            0, 0, 0, 0, hwnd, NULL, NULL, NULL
    );
    VIDEO_WINDOW = hvideo_g;
    return TRUE;
}

static void
mainwnd_onsize(HWND hwnd, int type, int width, int height)
{
    int x, y, w, h;
    x = width - BTN_GO_WIDTH - PADDING, y = height - BTN_GO_HEIGHT - PADDING, h = 0, w = 0;
    SetWindowPos(hbutton_g, HWND_TOP, x, y, w, h, SWP_NOSIZE);

    x -= TEXT_NITER_WIDTH, y -= BTN_GO_HEIGHT;
    SetWindowPos(htext_g, HWND_TOP, x, y, w, h, SWP_NOSIZE);

    w = x - 2*PADDING;
    x = PADDING, y = PADDING, h = height - 2*PADDING;
    SetWindowPos(hvideo_g, HWND_TOP, x, y, w, h, 0);
}
