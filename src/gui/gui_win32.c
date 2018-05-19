#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "gui.h"
#include "win32_mainwindow.h"

#define WIN_STYLE (WS_OVERLAPPEDWINDOW)

static HINSTANCE hinst_g;

extern int
gui_init(void)
{
    hinst_g = GetModuleHandle(NULL);
    InitCommonControls();

    if (!mainwnd_register(hinst_g))
        return -1;

    HWND hwnd = CreateWindowEx(
            0,
            MAINWND_CLASS,
            TEXT("Game of Life"),
            WIN_STYLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            DEFAULT_WINW,
            DEFAULT_WINH,
            NULL,
            NULL,
            hinst_g,
            NULL
    );
    if (!hwnd)
        return -1;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return 0;
}

extern int
gui_handle_events(void)
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
