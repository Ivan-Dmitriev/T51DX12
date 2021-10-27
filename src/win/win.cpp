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
  win::win( HINSTANCE hInst ) : hInstance(hInst), MouseWheel(0)
  {
    IsActive = TRUE;
    IsFullScreen = FALSE;
    W = 0;
    H = 0;
    WNDCLASS wc;
    //HWND hWnd;
    //MSG msg;

    /* Create and register window class */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(win *);
    wc.hbrBackground = (HBRUSH)COLOR_MENU;
    wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
    wc.hInstance = hInstance;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "My WindowClass";
    wc.lpfnWndProc = WinFunc;
    if (!RegisterClass(&wc))
      MessageBox(nullptr, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);

    /* Create window */
    hWnd = CreateWindow("My WindowClass", "3D animation", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr, nullptr, hInstance, reinterpret_cast<VOID *>(this));

    /* Show window */
    ShowWindow(hWnd, SW_NORMAL); /* SW_MAXIMIZE); */
  } /* End of 'win constructor' function */

  /* Window handle function.
   * ARGUMENTS:
   *   - window handle:
   *      HWND hWnd;
   *   - message type (see WM_***):
   *      UINT Msg;
   *   - message 'word' parameter:
   *      WPARAM wParam;
   *   - message 'long' parameter:
   *      LPARAM lParam;
   * RETURNS:
   *   (LRESULT) message depende return value.
   */
  LRESULT CALLBACK win::WinFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
  {
    win *Win;
    HDC hDC;
    PAINTSTRUCT ps;
    //MINMAXINFO *minmax;
 
    switch (Msg)
    {
    case WM_CREATE:
      SetWindowLongPtr(hWnd, 0, (UINT_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
    default:
      Win = reinterpret_cast<win *>(GetWindowLongPtr(hWnd, 0));
      if (Win != nullptr)
        switch (Msg)
        {
        case WM_CREATE:
          Win->hWnd = hWnd;
          return Win->OnCreate((CREATESTRUCT *)lParam) ? 0 : -1;
        case WM_SIZE:
          Win->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
          return 0;
        case WM_ERASEBKGND:
          //return 0;
          return (LRESULT)Win->OnEraseBackground((HDC)wParam);
        case WM_PAINT:
          hDC = BeginPaint(hWnd, &ps);
          Win->OnPaint(hDC, &ps);
          EndPaint(hWnd, &ps);
          return 0;
        case WM_TIMER:
          //InvalidateRect(hWnd, NULL, FALSE);
          Win->OnTimer((UINT)wParam);
          return 0;
        case WM_MOUSEWHEEL:
          Win->OnMouseWheel((INT)(SHORT)LOWORD(lParam),
                            (INT)(SHORT)HIWORD(lParam),
                            (INT)(SHORT)HIWORD(wParam),
                            (UINT)(SHORT)LOWORD(wParam));
          return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
          /*
          Win->OnButtonDown(FALSE,
                            (INT)(SHORT)LOWORD(lParam),
                            (INT)(SHORT)HIWORD(lParam),
                            (UINT)(SHORT)LOWORD(wParam));
          */
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
          /*
          Win->OnButtonUp((INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),
                          (UINT)(SHORT)LOWORD(wParam));
          */

          return 0;
        case WM_GETMINMAXINFO:
          Win->OnGetMinMaxInfo((MINMAXINFO *)lParam);
          return 0;
        case WM_DESTROY:
          Win->OnDestroy();
          PostQuitMessage(30);
          //return 0;
        case WM_CLOSE:
          Win->OnClose();
          //return 0;
        }
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
  } /* End of 'WinFunc' function */

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
  INT win::Run( VOID )
  {
    MSG msg;

    while (TRUE)
      /* Check message at window message queue */
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        if (msg.message == WM_QUIT)
          break;
        else
        {
          /* Displatch message to window */
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      else
      {
        Idle();
      }
     return msg.wParam;
  } /* End of 'Run' function */
} /* end of 'ivdx' namespace */

/* END OF 'win.cpp' FILE */

