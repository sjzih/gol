#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "gui.h"
#include "win32_mainwindow.h"

#define WIN_STYLE (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME)

static HINSTANCE hinst_g;

extern GUI_Window
gui_init(void)
{
    hinst_g = GetModuleHandle(NULL);

    if (!mainwnd_register(hinst_g))
        return NULL;

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
        return NULL;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return hwnd;
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
