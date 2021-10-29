/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : win.cpp
 * PROGRAMMER  : Ivan Dmitriev.
 * LAST UPDATE : 21.07.2021
 * PURPOSE     : WinAPI.
 */


#include <ivdx.h>
#include "../ivdx.h"

#include <Windows.h>
#include <iostream>

/* 
#include "../def.h"
#include "win.h"
#include "../anim/anim.h"
*/
/* window class name */
#define MAIN_WINDOW_CLASS_NAME "My WindowClass"

namespace ivdx
{
  /* Default win constructor */
  //win::win( HINSTANCE hInst ) : hInstance(hInst), MouseWheel(0)
  //{
  //  IsActive = TRUE;
  //  IsFullScreen = FALSE;
  //  W = 0;
  //  H = 0;
  //  WNDCLASS wc;
  //  //HWND hWnd;
  //  //MSG msg;

  //  /* Create and register window class */
  //  wc.style = CS_HREDRAW | CS_VREDRAW;
  //  wc.cbClsExtra = 0;
  //  wc.cbWndExtra = sizeof(win *);
  //  wc.hbrBackground = (HBRUSH)COLOR_MENU;
  //  wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
  //  wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
  //  wc.hInstance = hInstance;
  //  wc.lpszMenuName = nullptr;
  //  wc.lpszClassName = "My WindowClass";
  //  wc.lpfnWndProc = WinFunc;
  //  if (!RegisterClass(&wc))
  //    MessageBox(nullptr, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);

  //  /* Create window */
  //  hWnd = CreateWindow("My WindowClass", "3D animation", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
  //    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
  //    nullptr, nullptr, hInstance, reinterpret_cast<VOID *>(this));

  //  /* Show window */
  //  ShowWindow(hWnd, SW_NORMAL); /* SW_MAXIMIZE); */
  //} /* End of 'win constructor' function */

   /* Window message handle function (CALLBACK version).
    *   - window handle:
    *       HWND hWnd;
    *   - message identifier (see WM_***):
    *       UINT Msg;
    *   - message first parameter ('word' param):
    *       WPARAM wParam;
    *   - message second parameter ('long' param):
    *       LPARAM lParam;
    * RETURNS:
    *   (LRESULT) message return code (depended to Msg type).
    */
  LRESULT CALLBACK win::WindowFunc( HWND hWnd, UINT Msg,
                                         WPARAM wParam, LPARAM lParam )
  {
    win *Win;
    HDC hDC;
    PAINTSTRUCT ps;

    switch (Msg)
    {
    case WM_GETMINMAXINFO:
      ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
        GetSystemMetrics(SM_CYMAXTRACK) +
        GetSystemMetrics(SM_CYCAPTION) +
        GetSystemMetrics(SM_CYMENU) +
        GetSystemMetrics(SM_CYBORDER) * 2;
      return 0;
    case WM_CREATE:
      /* Attach 'this' pointer to window class to window */
      SetWindowLongPtr(hWnd, 0, (INT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
    default:
      Win = (win *)GetWindowLongPtr(hWnd, 0);
      if (Win != nullptr)
        switch (Msg)
        {
        case WM_CREATE:
          Win->hWnd = hWnd;
          return Win->OnCreate((CREATESTRUCT *)lParam) ? 0 : -1;
        case WM_DESTROY:
          Win->OnDestroy();
          return 0;
        case WM_SIZE:
          Win->OnSize((UINT)wParam, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam));
          return 0;
        case WM_ACTIVATE:
          Win->OnActivate((UINT)LOWORD(wParam), (HWND)(lParam), (BOOL)HIWORD(wParam));
          return 0;
        case WM_ERASEBKGND:
          return (LRESULT)Win->OnEraseBkgnd((HDC)wParam);
        case WM_PAINT:
          hDC = BeginPaint(hWnd, &ps);
          Win->OnPaint(hDC);
          EndPaint(hWnd, &ps);
          return 0;
        case WM_TIMER:
          Win->OnTimer((UINT)wParam);
          return 0;
        case WM_MOUSEWHEEL:
          Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam),
            (INT)(SHORT)HIWORD(wParam), (UINT)(SHORT)LOWORD(wParam));
          return 0;
        case WM_LBUTTONDBLCLK:
          Win->OnButtonDown(TRUE, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
          return 0;
        case WM_LBUTTONDOWN:
          Win->OnButtonDown(FALSE, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
          return 0;
        case WM_LBUTTONUP:
          Win->OnButtonUp((INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (UINT)(SHORT)LOWORD(wParam));
          return 0;
        case WM_DRAWITEM:
          Win->OnDrawItem((INT)wParam, (DRAWITEMSTRUCT *)lParam);
          return 0;
        case WM_NOTIFY:
          return Win->OnNotify((INT)wParam, (NMHDR *)lParam);
        case WM_DROPFILES:
          Win->OnDropFiles((HDROP)wParam);
          return 0;
        case WM_COMMAND:
          Win->OnCommand((INT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
          return 0;
        }
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
  } /* End of 'win::WindowFunc' function */
  /* Flip full screen function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID win::FlipFullScreen( VOID )
  {
    static BOOL IsFullScreen = FALSE; /* store current mode */
    static RECT SaveRC;               /* save old window size */

    if (!IsFullScreen)
    {
      HMONITOR hMon;
      MONITORINFOEX moninfo;
      RECT rc;

      IsFullScreen = TRUE;

      /* Save old window size and position */
      GetWindowRect(hWnd, &SaveRC);

      /* Get closest monitor */
      hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

      /* Get monitor information */
      moninfo.cbSize = sizeof(moninfo);
      GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

      rc = moninfo.rcMonitor;
      AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

      /* Expand window to full screen */
      SetWindowPos(hWnd, HWND_TOP,
        rc.left, rc.top,
        rc.right - rc.left, rc.bottom - rc.top,
        SWP_NOOWNERZORDER);
    }
    else
    {
      IsFullScreen = FALSE;

      /* Restore window size and position */
      SetWindowPos(hWnd, HWND_NOTOPMOST,
        SaveRC.left, SaveRC.top,
        SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
        SWP_NOOWNERZORDER);
    }
  } /* End of 'FlipFullScreen' function */

  /* Run function.
   * ARGUMENTS: None.
   * RETURNS: (INT) code of result message.
   */
  //INT win::Run( VOID )
  //{
  //  MSG msg;

  //  while (TRUE)
  //    /* Check message at window message queue */
  //    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
  //      if (msg.message == WM_QUIT)
  //        break;
  //      else
  //      {
  //        /* Displatch message to window */
  //        TranslateMessage(&msg);
  //        DispatchMessage(&msg);
  //      }
  //    else
  //    {
  //      Idle();
  //    }
  //   return msg.wParam;
  //} /* End of 'Run' function */
} /* end of 'ivdx' namespace */

/* END OF 'win.cpp' FILE */

