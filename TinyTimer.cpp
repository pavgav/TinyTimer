#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <CommCtrl.h>
#include <chrono> 
#include <thread>

#define CTRL_PLAY_BUTTON 1001
#define CTRL_STOP_BUTTON 1002
#define CTRL_UPDOWN 1003
#define IDM_FILE_SETTINGS 2001
#define IDM_FILE_EXIT 2002
#define IDM_ABOUT_INFO 2003

// ***Begin of forward declarations***
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AddMenus(HWND);
void CreateInterfaceWnds(HWND);     //for creating interface and passing parent's window handler

HMENU hMenuBar;                     //for creating mainWindow menu bar
HMENU hMenuFile;
HMENU hMenuAbout;


HWND hwndShowBox;                   //contol windows
HWND hwndInputBox;
HWND hwndUpDownCtrl;
HWND hwndPlayPauseButton;
HWND hwndStopPauseButton;


// ***End of forward declarations***

// ***Begin of Main function***
int WINAPI wWinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    PWSTR pCmdLine, 
    int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"mainWindowClass";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = MainWindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClassW(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"TT",                          // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 190, 90,

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
// ***End of Main function***



void CreateInterfaceWnds(HWND hwnd)     //Creating all control windows
{
    HWND hwndShowBox = CreateWindowEx(
        0,                              // Optional window styles.
        L"EDIT",                        // Rectangular child window for timer digits
        NULL,                           // Window text
        WS_CHILD | WS_BORDER |          //
        ES_NUMBER | ES_CENTER,         //
        //ES_READONLY,                    // Child window
        10, 10, 60, 20,                 // Position ans sizes
        hwnd,                           // Parent window handler
        NULL,
        NULL,
        NULL
    );
    SendMessage(hwndShowBox, WM_SETTEXT, 0, (LPARAM) L"00:00:00");

    HWND hwndInputBox = CreateWindowEx(
        0,                              // Optional window styles.
        L"EDIT",                        // Rectangular child window for timer digits
        NULL,                           // Window text
        WS_CHILD | WS_BORDER |          //
        ES_NUMBER | ES_CENTER ,         // Child window
        75, 10, 50, 20,                 // Position ans sizes
        hwnd,                           // Parent window handler
        NULL,
        NULL,
        NULL
    );

    HWND hwndUpDownCtrl = CreateWindowEx(
        0,                              // Optional window styles.
        UPDOWN_CLASS,                   // Up-Down Control class
        NULL,                           // Window text
        WS_CHILD | WS_BORDER | UDS_ALIGNRIGHT | UDS_SETBUDDYINT, // Child window   
        105, 10, 20, 30,                // Position ans sizes
        hwnd,                           // Parent window handler
        (HMENU) CTRL_UPDOWN,
        NULL,
        NULL
    );
    SendMessage(hwndUpDownCtrl, UDM_SETBUDDY, (WPARAM) (HWND) hwndInputBox, 0); //set a buddy window to reflect timer setting
    SendMessage(hwndUpDownCtrl, UDM_SETRANGE, 0, MAKELPARAM(999, 0));           //setting a range for the timer setting 0-999min

    HWND hwndPlayPauseButton = CreateWindowEx(
        0,                              // Optional window styles.
        L"BUTTON",                      // Up-Down Control class
        NULL,                           // Window text
        WS_CHILD | WS_BORDER,           // Child window
        130, 10, 20, 20,                // Position ans sizes
        hwnd,                           // Parent window handler
        (HMENU) CTRL_PLAY_BUTTON,       //setting identifire for the button
        NULL,
        NULL
    );
    
    HWND hwndStopButton = CreateWindowEx(
        0,                              // Optional window styles.
        L"BUTTON",                      // Up-Down Control class
        NULL,                           // Window text
        WS_CHILD | WS_BORDER,           // Child window
        150, 10, 20, 20,                // Position ans sizes
        hwnd,                           // Parent window handler
        (HMENU) CTRL_STOP_BUTTON,       //setting identifire for the button
        NULL,
        NULL
    );        

    ShowWindow(hwndShowBox, SW_SHOWNORMAL);
    ShowWindow(hwndInputBox, SW_SHOWNORMAL);
    ShowWindow(hwndUpDownCtrl, SW_SHOWNORMAL);
    ShowWindow(hwndPlayPauseButton, SW_SHOWNORMAL);
    ShowWindow(hwndStopButton, SW_SHOWNORMAL);
}

void AddMenus(HWND hwnd)
{
    hMenuBar = CreateMenu();
    hMenuFile = CreateMenu();
    hMenuAbout = CreateMenu();

    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR) hMenuFile, L"File");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR) hMenuAbout, L"About");

    AppendMenuW(hMenuFile, MF_STRING, IDM_FILE_SETTINGS , L"Settings");
    AppendMenuW(hMenuFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuFile, MF_STRING, IDM_FILE_EXIT , L"Exit");

    AppendMenuW(hMenuAbout, MF_STRING, IDM_ABOUT_INFO, L"Info");

    SetMenu(hwnd, hMenuBar);
}

void updateText()
{
    SetWindowTextW(hwndShowBox, L"text");
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)   // Handle the mesages
    {
    case WM_CREATE:
        {
            CreateInterfaceWnds(hwnd);
            AddMenus(hwnd);
            return 0;
        }

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
            return 0;
        }
        
    case WM_COMMAND:
        {
            switch (wParam)
            {
                case CTRL_PLAY_BUTTON:
                    if ((HIWORD(wParam) == BN_CLICKED)) //PLAY_BUTTON clicked event
                    {
                        SetWindowTextW(hwndShowBox, L"text");
                        updateText();
                        MessageBox(hwnd, L"You pressed PLAY_BUTTON", L"Check message", MB_OK);
                    }
                    break;
                
                case CTRL_STOP_BUTTON:
                    if ((HIWORD(wParam) == BN_CLICKED)) //STOP_BUTTON clicked event
                    {
                        MessageBox(hwnd, L"You pressed STOP_BUTTON", L"Check message", MB_OK);
                    }
                    break;
                
                case CTRL_UPDOWN:
                    MessageBox(hwnd, L"You pressed CTRL_UPDOWN", L"Check message", MB_OK);
                    break;

                default:
                    break;
            }
            return 0;
        }
        
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}