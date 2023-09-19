#include "header.h"

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
        CW_USEDEFAULT, CW_USEDEFAULT, 210, 90,

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

    // Setting the flag for timer status, creating timer object and timer for window refresh
    short timerStatus {TIMER_NOT_ACTIVE};
    int timerSetting {0};
    Timer tinyTimer;
    
    

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

//Main window function
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)                                                               // Handle the mesages
    {
    case WM_CREATE:
        {
            CreateInterfaceWnds(hwnd);
            AddMenus(hwnd);
            timerStatus = TIMER_NOT_ACTIVE;
            return 0;
        }

    case WM_CLOSE:                                                              // User tries to close the timer, check whether it's intentionally
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
                    if ((HIWORD(wParam) == BN_CLICKED))                         //PLAY_BUTTON clicked event
                    {
                        switch (timerStatus)
                        {
                        case TIMER_ACTIVE:
                            //timer already active, let's pause it
                            tinyTimer.pause();
                            timerStatus = TIMER_ON_PAUSE;
                            SetWindowText(hwndPlayPauseButton, L"");
                            SetWindowText(hwndPlayPauseButton, L"\u25B7"); 

                            std::cout << "Time paused. Elapsed: " << tinyTimer.elapsed() << " seconds \n"; //#debug
                            break;
                        
                        case TIMER_NOT_ACTIVE:
                            {
                            int timerInputInt {readInputBox(hwndInputBox)};     //how many minutes in InputBox                             
                            updateShowBoxAsValue(timerInputInt*60, hwndShowBox);   //setting ShowBox according to the timer input
                            timerSetting = timerInputInt*60;                       
                            SendMessage(hwndInputBox, EM_SETREADONLY, true, 0); //blocks input for "hwndInputBox" while timer is active
                            SetWindowText(hwndPlayPauseButton, L"||");      //PlayPause button text as "II"

                            SetTimer (hwnd, ID_WINTIMER, 1000, (TIMERPROC) NULL);
                            tinyTimer.reset();
                            timerStatus = TIMER_ACTIVE;
                            }
                            
                            std::cout << "Timer reset P/P. Elapsed: " << tinyTimer.elapsed() << " seconds \n";  //#debug
                            break;
                        
                        case TIMER_ON_PAUSE:
                            tinyTimer.pause();
                            timerStatus = TIMER_ACTIVE;
                            SetWindowText(hwndPlayPauseButton, L"||");      //PlayPause button text as "II"

                            std::cout << "Timer continued. Elapsd: " << tinyTimer.elapsed() << " seconds \n"; //#debug
                            break;

                        default:
                            MessageBox(hwnd, L"default", L"Check message", MB_OK);
                            break;
                        }

                    }
                    break;
                
                case CTRL_STOP_BUTTON:
                    if ((HIWORD(wParam) == BN_CLICKED))                         //STOP_BUTTON clicked event
                    {
                        KillTimer(hwnd, ID_WINTIMER);
                        tinyTimer.reset(); 
                        timerStatus = TIMER_NOT_ACTIVE;
                        SendMessage(hwndInputBox, EM_SETREADONLY, false, 0); //allows input for "hwndInputBox" while timer is not active
                        SetWindowText(hwndPlayPauseButton, L"\u2BC8");

                        std::cout << "Timer reset STOP. Elapsd: " << tinyTimer.elapsed() << " seconds \n";  //#debug
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

    case WM_TIMER:                                                              //ID_WINTIMER messages for refreshing "hwndShowBox" value and checking "tinyTimer" expiration
        if (wParam == ID_WINTIMER)
        {
            std::cout << "MSG tick \n";                                         //#debug
            updateShowBoxAsValue((timerSetting - tinyTimer.elapsed()), hwndShowBox);

            if ((timerSetting - tinyTimer.elapsed()) == 0)
            {
                KillTimer(hwnd, ID_WINTIMER);
                tinyTimer.reset(); 
                timerStatus = TIMER_NOT_ACTIVE;
                SendMessage(hwndInputBox, EM_SETREADONLY, false, 0); //allows input for "hwndInputBox" while timer is not active
                SetWindowText(hwndPlayPauseButton, L"\u2BC8");
                
                std::wstring myString{};
                myString = std::to_wstring(timerSetting/60) + L" min elapsed.";
                LPCTSTR str = myString.c_str();
                MessageBox(hwnd, str, L"Timer is elapsed", MB_OK 
                    | MB_SETFOREGROUND | MB_ICONINFORMATION | MB_TOPMOST);
            }
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//Creating all control windows
void CreateInterfaceWnds(HWND hwnd)     
{
    hwndShowBox = CreateWindowEx(
        0,                              // Optional window styles.
        L"EDIT",                        // Rectangular child window for timer digits
        L"00:00:00",                    // Window text
        WS_CHILD | WS_BORDER |          //
        ES_NUMBER | ES_CENTER |         //
        ES_READONLY,                    // Child window
        10, 10, 60, 20,                 // Position ans sizes
        hwnd,                           // Parent window handler
        NULL,
        NULL,
        NULL
    );
    
    hwndInputBox = CreateWindowEx(
        0,                              // Optional window styles.
        L"EDIT",                        // Rectangular child window for timer digits
        L"45",                         // Window text
        WS_CHILD | WS_BORDER |          //
        ES_NUMBER | ES_CENTER ,         // Child window
        75, 10, 50, 20,                 // Position ans sizes
        hwnd,                           // Parent window handler
        NULL,
        NULL,
        NULL
    );

    hwndUpDownCtrl = CreateWindowEx(
        0,                              // Optional window styles.
        UPDOWN_CLASS,                   // Up-Down Control class
        NULL,                           // Window text
        WS_CHILD | WS_BORDER | UDS_ALIGNRIGHT | UDS_SETBUDDYINT, // Child window   
        105, 12, 20, 30,                // Position ans sizes
        hwnd,                           // Parent window handler
        (HMENU) CTRL_UPDOWN,
        NULL,
        NULL
    );
    SendMessage(hwndUpDownCtrl, UDM_SETBUDDY, (WPARAM) (HWND) hwndInputBox, 0); //set a buddy window to reflect timer setting
    SendMessage(hwndUpDownCtrl, UDM_SETRANGE, 0, MAKELPARAM(999, 0));           //setting a range for the timer setting 0-999min

    hwndPlayPauseButton = CreateWindowEx(
        0,                              // Optional window styles.
        L"BUTTON",                      // Up-Down Control class
        L"\u2BC8",                      // Window text: unicode escape sequence
        WS_CHILD | WS_BORDER,           // Child window
        130, 10, 30, 20,                // Position ans sizes
        hwnd,                           // Parent window handler
        (HMENU) CTRL_PLAY_BUTTON,       //setting identifire for the button
        NULL,
        NULL
    );
    
    hwndStopButton = CreateWindowEx(
        0,                              // Optional window styles.
        L"BUTTON",                      // Up-Down Control class
        L"\u2BC0",                      // Window text: unicode escape sequence
        WS_CHILD | WS_BORDER,           // Child window
        160, 10, 30, 20,                // Position ans sizes
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

//Creating main window menus
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

