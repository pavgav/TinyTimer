#include <windows.h>
#include <cwchar>

//updateShowBoxAsValue updates ShowBox to valueMin
void updateShowBoxAsValue(int valueSec, HWND hwndShowBox)
{
    wchar_t newShowBoxText[] {L"00:00:00"};
    int hours {};
    int minutes {};
    int seconds {};

    hours = valueSec / 3600;
    minutes = (valueSec - hours*3600)/60;
    seconds = valueSec - hours*3600 - minutes*60;

    {                                                                           // updating hours value in "newShowBoxText"
    wchar_t hoursCString[3]{};
    _itow_s(hours,hoursCString, sizeof(hoursCString)/2,10);                     //converts int to wchar_t

    if (hours >= 10) 
        {
            newShowBoxText[0] = hoursCString[0];
            newShowBoxText[1] = hoursCString[1];
        }
    else 
        {
            newShowBoxText[1] = hoursCString[0];
        }
    }
    {                                                                           // updating minutes value in "newShowBoxText"
    wchar_t minutesCString[3]{};
    _itow_s(minutes,minutesCString, sizeof(minutesCString)/2,10);               //converts int to wchar_t

    if (minutes >= 10) 
        {
            newShowBoxText[3] = minutesCString[0];
            newShowBoxText[4] = minutesCString[1];
        }
    else 
        {
            newShowBoxText[4] = minutesCString[0];
        }
    }
    {                                                                           // updating seconds value in "newShowBoxText"
    wchar_t secondsCString[3]{};
    _itow_s(seconds, secondsCString, sizeof(secondsCString)/2,10);               //converts int to wchar_t

    if (seconds >= 10) 
        {
            newShowBoxText[6] = secondsCString[0];
            newShowBoxText[7] = secondsCString[1];
        }
    else 
        {
            newShowBoxText[7] = secondsCString[0];
        }

    SetWindowTextW(hwndShowBox, newShowBoxText);
    }
    
}

//readInputBox gets the data from InputBox and returns it in int format
int readInputBox(HWND hwndInputBox)
{
    wchar_t timerInputWchar_t[] {L"000"};                   //variable for record information from InputBox
    GetWindowTextW(hwndInputBox, timerInputWchar_t, 4);
    
    //converting timer input to int an returning it
    wchar_t *end;                     
    return std::wcstol(timerInputWchar_t, &end, 10);
}