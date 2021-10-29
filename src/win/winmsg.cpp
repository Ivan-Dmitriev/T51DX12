/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : winmsg.cpp
 * PROGRAMMER  : Ivan Dmitriev.
 * LAST UPDATE : 21.07.2021
 * PURPOSE     : WinAPI.
 * PROGRAMMER  : Ivan Dmitriev.
 * LAST UPDATE : 23.07.2021
 * NOTE         : Module namespace 'ivdx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <ivdx.h>
#include "../ivdx.h"

namespace ivdx
{

}
 //#include <commondf.h>
//
//
//#define InitTimer 30
//#define RefreshTimer 47
//
//namespace ivdx
//{
//  /* WM_CREATE window message handle function.
//   * ARGUMENTS:
//   *   - structure with creation data:
//   *       CREATESTRUCT *CS;
//   * RETURNS:
//   *   (BOOL) TRUE to continue creation window, FALSE to terminate.
//   */
//  BOOL win::OnCreate( CREATESTRUCT *CS )
//  {
//    /* SetTimer for 10 msec */
//    SetTimer(hWnd, InitTimer, 10, NULL);
//    return TRUE;
//  } /* End of 'win::OnCreate' function */
// 
//  /* WM_DESTROY window message handle function.
//   * ARGUMENTS: None.
//   * RETURNS: None.
//   */
//  VOID win::OnDestroy( VOID )
//  {
//    if (IsInit)
//      {
//        Close();
//        KillTimer(hWnd, RefreshTimer);
//      }
//      else
//        KillTimer(hWnd, InitTimer);
//  } /* End of 'win::OnDestroy' function */
// 
//  /* WM_SIZE window message handle function.
//   * ARGUMENTS:
//   *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
//   *       UINT State;
//   *   - new width and height of client area:
//   *       INT W, H;
//   * RETURNS: None.
//   */
//  VOID win::OnSize( UINT State, INT W, INT H )
//  {
//    this->W = W;
//    this->H = H;
//    if (IsInit)
//      Resize();
//  } /* End of 'win::OnSize' function */
// 
//  /* WM_ERASEBKGND window message handle function.
//   * ARGUMENTS:
//   *   - device context of client area:
//   *       HDC hDC;
//   * RETURNS:
//   *   (BOOL) TRUE if background is erased, FALSE otherwise.
//   */
//  BOOL win::OnEraseBackground( HDC hDC )
//  {
//    if (IsInit)
//      Erase(hDC);
//    return TRUE;
//  } /* End of 'win::OnEraseBkgnd' function */
// 
//  /* WM_PAINT window message handle function.
//   * ARGUMENTS:
//   *   - window device context:
//   *       HDC hDC;
//   *   - paint message structure pointer:
//   *       PAINTSTRUCT *PS;
//   * RETURNS: None.
//   */
//  VOID win::OnPaint( HDC hDC, PAINTSTRUCT *PS )
//  {
//    if (IsInit)
//      Paint(hDC);
//    /*
//    SelectObject(hDC, GetStockObject(DC_PEN));
//    SetDCPenColor(hDC, RGB(255, 0, 0));
//    Rectangle(hDC, 100, 100, 1000, 100);
//    */
//  } /* End of 'win::OnPaint' function */ 
//
//  /* WM_ACTIVATE window message handle function.
//   * ARGUMENTS:
//   *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
//   *       UINT Reason;
//   *   - handle of active window:
//   *       HWND hWndActDeact;
//   *   - minimized flag:
//   *       BOOL IsMinimized;
//   * RETURNS: None.
//   */
//  VOID win::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
//  {
//    if (IsInit)
//      Activate();
//    // IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
//  } /* End of 'win::OnActivate' function */
// 
//  /* WM_TIMER window message handle function.
//   * ARGUMENTS:
//   *   - specified the timer identifier.
//   *       INT Id;
//   * RETURNS: None.
//   */
//  VOID win::OnTimer( INT Id ) 
//  {
//    if (!IsInit)
//    {
//      KillTimer(hWnd, InitTimer);
//      SetTimer(hWnd, RefreshTimer, 10, NULL);
//      IsInit = TRUE;
//      // call callbacks
//      Init();
//      Resize();
//      Timer();
//    }
//    else
//      Timer();
//  } /* End of 'win::OnTimer' function */
// 
//  /* WM_MOUSEWHEEL window message handle function.
//   * ARGUMENTS:
//   *   - mouse window position:
//   *       INT X, Y;
//   *   - mouse wheel relative delta value:
//   *       INT Z;
//   *   - mouse keys bits (see MK_*** bits constants):
//   *       UINT Keys;
//   * RETURNS: None.
//   */
//  VOID win::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
//  {
//    MouseWheel += Z;
//  } /* End of 'win::OnMouseWheel' function */
//
//  /* WM_CLOSE window message handle function.
//   * ARGUMENTS:
//   *   - window device context:
//   *       HDC hDC;
//   *   - paint message structure pointer:
//   *       PAINTSTRUCT *PS;
//   * RETURNS: None.
//   */
//  VOID win::OnClose( VOID )
//  {
//    /*
//    if (MessageBox(NULL, L"Do you want to quit?", L"Quit", MB_YESNO | MB_ICONINFORMATION) == IDNO)
//      return;
//    else
//    */
//    //PostQuitMessage(30);
//  } /* End of 'win::OnPaint' function */ 
//  VOID win::OnGetMinMaxInfo( MINMAXINFO *MinMax )
//  {
//    MinMax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
//    GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
//  } /* End of 'win::OnGetMinMaxInfo' function */ 
//}
//
/* END OF 'winmsg.cpp' FILE */
