/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : win.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               WinApi.
 *               Window class functions.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx::win'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>
#include "../../bodx.h"

/* Project name space */
namespace bodx
{
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
    win* Win;
    HDC hDC;
    PAINTSTRUCT ps;
    MINMAXINFO *minmax;
 
    switch (Msg)
    {
    case WM_GETMINMAXINFO:
      minmax = (MINMAXINFO *)lParam;
      minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
        GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
      return 0;
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
          Win->OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
          return 0;
        case WM_ERASEBKGND:
          return (LRESULT)Win->OnEraseBackground((HDC)wParam);
        case WM_PAINT:
          hDC = BeginPaint(hWnd, &ps);
          Win->OnPaint(hDC, &ps);
          EndPaint(hWnd, &ps);
          return 0;
        case WM_TIMER:
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
          Win->OnButtonDown(FALSE,
                            (INT)(SHORT)LOWORD(lParam),
                            (INT)(SHORT)HIWORD(lParam),
                            (UINT)(SHORT)LOWORD(wParam));
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
          Win->OnButtonUp((INT)(SHORT)LOWORD(lParam),
                          (INT)(SHORT)HIWORD(lParam),
                          (UINT)(SHORT)LOWORD(wParam));
          return 0;
        case WM_DESTROY:
          Win->OnDestroy();
          PostQuitMessage(30);
          return 0;
        }
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
  }; /* End of 'win::WinFunc' function */

  /* Window class constructor.
   * ARGUMENTS:
   *   - handle of application instance:
   *       HINSTANCE hInstance;
   * - window name:
   *       const char* WndClassName;
   */
  win::win( HINSTANCE hInst ) : hInstance(hInst)
  {
    WNDCLASS wc;

    /* Create and register window class */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(win *);
    wc.hbrBackground = (HBRUSH)COLOR_MENU;
    wc.hCursor = LoadCursor(nullptr, IDC_HAND);
    wc.hIcon = LoadIcon(nullptr, IDI_SHIELD);
    wc.hInstance = hInstance;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = "T05ANIM window class";
    wc.lpfnWndProc = WinFunc;
    if (!RegisterClass(&wc))
      MessageBox(nullptr, "Error register window class",
                 "ERROR", MB_OK | MB_ICONERROR);

    /* Create window */
    hWnd = CreateWindow("T05ANIM window class", "My window",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr, nullptr, hInstance, reinterpret_cast<VOID *>(this));
  }; /* End of constructor */

  /* Start message loop.
   * ARGUMENTS: None.
   * RETURNS:
   *   (INT) Auit message.
   */
  INT win::Run( VOID )
  {
    MSG msg;

    /* Message loop */
    while (TRUE)
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      {
        if (msg.message == WM_QUIT)
          break;
        DispatchMessage(&msg);
      }
      else
        if (IsInit)
          Idle();

    return (INT)msg.wParam;
  }; /* End of 'win::Run' function */
} /* end of 'bodx' name space */

/* END OF 'win.cpp' FILE */