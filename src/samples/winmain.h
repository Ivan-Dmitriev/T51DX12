/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : winmain.h
 * PURPOSE     : CGSG'Sr'2021 WinApi resourse sample project.
 *               Main window handle declaration module.
 * PROGRAMMER  : CGSG'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 27.10.2021
 * NOTE        : Project namespace 'ttt'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __winmain_h_
#define __winmain_h_

#include <commctrl.h>
#include <windowsx.h>

#include "../res/resource.h"
#include "../win/win.h"
#include "dlgabout.h"

#pragma comment(lib, "comctl32")

/* Project namespace */
namespace ivdx
{
  /* Window class name */
  const CHAR MainWindowClassName[] = "CGSG'Sr'2021 WinApi Resourses Main Window Class";

  static const INT InitTimer = 130;
  static const INT RefreshTimer = 30;

  /* Window handle class */
  class win_main : public win
  {
  private:
    HACCEL hAccel; // Accelerator table

    dlg_about MyTool; // Sample tool window

    /***
     * Cracked message handle functions
     ***/

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    BOOL OnCreate( CREATESTRUCT *CS ) override
    {
      SetTimer(hWnd,             // handle to main window 
               InitTimer,        // timer identifier 
               0,                // 0-second interval 
               (TIMERPROC)nullptr);
      //MyTool.Create();
      return TRUE;
    } /* End of 'OnCreate' function */

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnDestroy( VOID ) override
    {
      PostQuitMessage(0);
      if (IsInit)
        {
          Close();
          KillTimer(hWnd, RefreshTimer);
        }
        else
          KillTimer(hWnd, InitTimer);
    } /* End of 'OnDestroy' function */

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID OnPaint( HDC hDC ) override
    {
    } /* End of 'OnPaint' function */

    /* WM_DROPFILES window message handle function.
     * ARGUMENTS:
     *   - handle to an internal structure describing the dropped files:
     *       HDROP hDrop;
     * RETURNS: None.
     */
    VOID OnDropFiles( HDROP hDrop ) override
    {
      CHAR Buf[1000];
      MSGBOXPARAMS mbp {};

      mbp.cbSize = sizeof(MSGBOXPARAMS);
      mbp.hwndOwner = hWnd;
      mbp.hInstance = hInstance;
      mbp.lpszCaption = "About";
      mbp.lpszIcon = (CHAR *)IDI_LOGO;
      mbp.dwStyle = MB_OK | MB_USERICON;
      mbp.lpszText = Buf;
      LoadString(hInstance, IDS_ABOUT, Buf,sizeof(Buf));
      MessageBoxIndirect(&mbp);
    } /* End of 'OnDropFiles' function */

    /* WM_DRAWITEM window message handle function.
     * ARGUMENTS:
     *   - control identifier (for menu - 0):
     *       INT Id;
     *   - draw item information structure pointer:
     *       DRAWITEMSTRUCT *DrawItem;
     * RETURNS: None.
     *   (LRESULT) return value depended to notification.
     */
    VOID OnDrawItem( INT Id, DRAWITEMSTRUCT *DrawItem ) override
    {
    } /* End of 'OnDrawItem' function */

    /* WM_NOTIFY window message handle function.
     * ARGUMENTS:
     *   - control identifier:
     *       INT Id;
     *   - notification header depended to control type:
     *       NMHDR *NoteHead;
     * RETURNS:
     *   (LRESULT) return value depended to notification.
     */
    LRESULT OnNotify( INT Id, NMHDR *NoteHead ) override
    {
      return 0;
    } /* End of 'OnNotify' function */

    /* WM_COMMAND window message handle function.
     * ARGUMENTS:
     *   - item (menu or accelerator) or control window identifier:
     *       INT Id;
     *   - control window handle:
     *       HWND hWndCtl;
     *   - notification: 0 - menu, 1 - accelerator, otherwise -
     *     depended to control (like BM_CLICKED):
     *       UINT CodeNotify;
     * RETURNS: None.
     */
    VOID OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify ) override
    {
      switch (Id)
      {
      case ID_HELP_ABOUT:
        dlg_about(this).Run();
        return;
      case 30:
        OnDropFiles(nullptr);
        return;
      case ID_FILES_EXIT:
      case 31:
        DestroyWindow(hWnd);
        return;
      }
    } /* End of 'OnCommand' function */
    
    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    VOID Erase( HDC hDC ) override
    {
      if (IsInit)
        Erase(hDC);
    } /* End of 'Erase' function */

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - application instance handle:
     *       HINSTANCE hInstanceNew;
     */
    win_main( HINSTANCE hInstanceNew = GetModuleHandle(nullptr) ) : win(hInstanceNew), MyTool(this)
    {
      /* Register window class */
      WNDCLASSEX wc;

      wc.cbSize = sizeof(WNDCLASSEX);
      wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = sizeof(VOID *);
      wc.hbrBackground = (HBRUSH)COLOR_MENU;
      wc.hCursor = LoadCursor(hInstance, (CHAR *)IDC_MAIN_CURSOR);
      //wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
      //wc.hIconSm = LoadIcon(nullptr, IDI_SHIELD);
      wc.hIcon = (HICON)LoadImage(hInstance, (CHAR *)IDI_LOGO, IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
      wc.hIconSm = (HICON)LoadImage(hInstance, (CHAR *)IDI_LOGO, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);;
      wc.lpszMenuName = nullptr;
      wc.hInstance = hInstance;
      wc.lpfnWndProc = WindowFunc;
      wc.lpszClassName = MainWindowClassName;

      if (!RegisterClassEx(&wc))
      {
        CHAR Buf[100];
        sprintf(Buf, "%d", GetLastError());
        OutputDebugString(Buf);
        MessageBox(nullptr, "Error register window class", "ERROR", MB_OK);
        return;
      }

      /* Create window */
      hWnd =
        CreateWindowEx(WS_EX_ACCEPTFILES,
          MainWindowClassName,
          "CGSG'Sr'2021 Resourses Sample. 30 forever!!!",
          WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT, CW_USEDEFAULT,
          CW_USEDEFAULT, CW_USEDEFAULT,
          nullptr, LoadMenu(hInstance, (CHAR *)IDR_MENU1), hInstance, (VOID *)this);


      hAccel = LoadAccelerators(hInstance, (CHAR *)IDR_ACCELERATOR1);

      /* Show window */
      ShowWindow(hWnd, SW_SHOWNORMAL);
      UpdateWindow(hWnd);
      CreateWindow("BUTTON",
        "About",
        WS_CHILD | WS_CLIPSIBLINGS,
        /*
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        */
        30, 30, 300, 100,
        hWnd, (HMENU)30, hInstance, nullptr);

    } /* End of 'win_main' function */

    /* Class destructor */
    ~win_main( VOID ) override
    {
    } /* End of '~win_main' function */
    
    /* Application main loop start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Run( VOID ) override
    {
      MSG msg;

      while (TRUE)
        /* Check message at window message queue */
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
          if (msg.message == WM_QUIT)
            break;
          else
          {
            /* Dispatch message to window */
            if (!TranslateAccelerator(hWnd, hAccel, &msg))
            {
              TranslateMessage(&msg);
              DispatchMessage(&msg);
            }
          }
        else
          /* Call idle virtual function */
          Idle();
    } /* End of 'Run' function */

  }; /* End of 'win_main' class */
} /* end of 'ttt' namespace */

#endif /* __winmain_h_*/

/* END OF 'winmain.h' FILE */
