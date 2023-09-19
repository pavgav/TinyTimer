#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <CommCtrl.h>
#include <iostream>
#include <string>
#include "timer.h"

#define CTRL_PLAY_BUTTON 1001
#define CTRL_STOP_BUTTON 1002
#define CTRL_UPDOWN 1003
#define IDM_FILE_SETTINGS 2001
#define IDM_FILE_EXIT 2002
#define IDM_ABOUT_INFO 2003

#define TIMER_NOT_ACTIVE 101
#define TIMER_ACTIVE 102
#define TIMER_ON_PAUSE 103

#define ID_WINTIMER 201

// ***Begin of forward declarations***
HWND hwnd;                                                                      //main window handle
HWND hwndShowBox;                                                               //contol windows handles
HWND hwndInputBox;                                                              //
HWND hwndUpDownCtrl;                                                            //
HWND hwndPlayPauseButton;                                                       //
HWND hwndStopButton;                                                            //

HMENU hMenuBar;                                                                 //mainWindow menu handles
HMENU hMenuFile;
HMENU hMenuAbout;

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

short timerStatus;                                                              // 0 - not active; 1 -
int timerSetting;
Timer tinyTimer;

void AddMenus(HWND);                                                            //for creating menus and passing parent's window handler
void CreateInterfaceWnds(HWND);                                                 //for creating interface and passing parent's window handler

void updateShowBoxAsValue(int, HWND);                                           //updateShowBoxAsValue updates ShowBox to valueMin
int readInputBox(HWND); 

// ***End of forward declarations***