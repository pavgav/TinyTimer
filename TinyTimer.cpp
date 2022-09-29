#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <CommCtrl.h>

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND timerTextBox; 

int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    PWSTR pCmdLine, 
    int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = MainWindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Simple timer",                // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 100,

        NULL,       // Parent window    
        NULL,       // No Menu
        hInstance,  // Instance handle
        NULL        // Additional application data not transfered
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)   // Handle the mesages
    {
    case WM_CLOSE: // User tries to close the timer, check whether it's intentionally
        if (MessageBox(hwnd, L"Close timer?", L"Timer", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }
        return 0;
    
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    
    case WM_CREATE:
        HWND hwndShowBox = CreateWindowEx(
            0,          // Optional window styles.
            L"EDIT",    // Rectangular child window for timer digits
            NULL,       // Window text
            WS_CHILD | WS_BORDER,   // Child window
            10, 10, 60, 20, // Position ans sizes
            hwnd, // Parent window handler
            NULL,
            NULL,
            NULL
        );

        HWND hwndInputBox = CreateWindowEx(
            0,          // Optional window styles.
            L"EDIT",    // Rectangular child window for timer digits
            NULL,       // Window text
            WS_CHILD | WS_BORDER,   // Child window
            75, 10, 30, 20, // Position ans sizes
            hwnd, // Parent window handler
            NULL,
            NULL,
            NULL
        );

        HWND hwndUpDownCtrl = CreateWindowEx(
            0,          // Optional window styles.
            UPDOWN_CLASS,    // Up-Down Control class
            NULL,       // Window text
            WS_CHILD | WS_BORDER,   // Child window
            105, 10, 20, 20, // Position ans sizes
            hwnd, // Parent window handler
            NULL,
            NULL,
            NULL
        );
        ShowWindow(hwndShowBox, SW_SHOWNORMAL);
        ShowWindow(hwndInputBox, SW_SHOWNORMAL);
        ShowWindow(hwndUpDownCtrl, SW_SHOWNORMAL);

        return 0;


    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}