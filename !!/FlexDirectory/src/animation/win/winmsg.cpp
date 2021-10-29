/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : winmsg.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               WinApi.
 *               Window message crackers functions.
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

static const INT InitTimer = 130;
static const INT RefreshTimer = 30;
/* WM_CREATE window message handle function.
  * ARGUMENTS:
  *   - structure with creation data:
  *       CREATESTRUCT *CS;
  * RETURNS:
  *   (BOOL) TRUE to continue creation window, FALSE to terminate.
  */
BOOL bodx::win::OnCreate( CREATESTRUCT *CS )
{
  SetTimer(hWnd,             // handle to main window 
            InitTimer,        // timer identifier 
            0,                // 0-second interval 
            (TIMERPROC)nullptr);
  return TRUE;
}; /* End of 'bodx::win::OnCreate' function */
 
/* WM_DESTROY window message handle function.
  * ARGUMENTS: None.
  * RETURNS: None.
  */
VOID bodx::win::OnDestroy( VOID )
{
  if (IsInit)
    {
      Close();
      KillTimer(hWnd, RefreshTimer);
    }
    else
      KillTimer(hWnd, InitTimer);
}; /* End of 'bodx::win::OnDestroy' function */
 
/* WM_SIZE window message handle function.
  * ARGUMENTS:
  *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
  *       UINT State;
  *   - new width and height of client area:
  *       INT W, H;
  * RETURNS: None.
  */
VOID bodx::win::OnSize( UINT State, INT W, INT H )
{
  this->W = W;
  this->H = H;
  if (IsInit)
    Resize();
}; /* End of 'bodx::win::OnSize' function */
 
/* WM_ERASEBKGND window message handle function.
  * ARGUMENTS:
  *   - device context of client area:
  *       HDC hDC;
  * RETURNS:
  *   (BOOL) TRUE if background is erased, FALSE otherwise.
  */
BOOL bodx::win::OnEraseBackground( HDC hDC )
{
  if (IsInit)
      Erase(hDC);
  return TRUE;
}; /* End of 'bodx::win::OnEraseBackground' function */
 
/* WM_PAINT window message handle function.
  * ARGUMENTS:
  *   - window device context:
  *       HDC hDC;
  *   - paint message structure pointer:
  *       PAINTSTRUCT *PS;
  * RETURNS: None.
  */
VOID bodx::win::OnPaint( HDC hDC, PAINTSTRUCT *PS )
{
  if (IsInit)
      Paint(hDC);
}; /* End of 'bodx::win::OnPaint' function */

/* WM_ACTIVATE window message handle function.
  * ARGUMENTS:
  *   - reason (WA_CLICKACTIVE, WA_ACTIVE or WA_INACTIVE):
  *       UINT Reason;
  *   - handle of active window:
  *       HWND hWndActDeact;
  *   - minimized flag:
  *       BOOL IsMinimized;
  * RETURNS: None.
  */
VOID bodx::win::OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
{
  IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
  if (IsInit)
      Activate();
}; /* End of 'bodx::win::OnActivate' function */
 
/* WM_TIMER window message handle function.
  * ARGUMENTS:
  *   - specified the timer identifier.
  *       INT Id;
  * RETURNS: None.
  */
VOID bodx::win::OnTimer( INT Id ) 
{
  if (!IsInit)
    {
      KillTimer(hWnd, InitTimer);
      SetTimer(hWnd,             // handle to main window 
                RefreshTimer,     // timer identifier 
                0,                // 0-second interval 
                (TIMERPROC)nullptr);
      IsInit = TRUE;

      // call callbacks
      Init();
      Resize();
      Timer();
    }
    else
      Timer();
}; /* End of 'bodx::win::OnTimer' function */
 
/* WM_MOUSEWHEEL window message handle function.
  * ARGUMENTS:
  *   - mouse window position:
  *       INT X, Y;
  *   - mouse wheel relative delta value:
  *       INT Z;
  *   - mouse keys bits (see MK_*** bits constants):
  *       UINT Keys;
  * RETURNS: None.
  */
VOID bodx::win::OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
{
  MouseWheel += Z;
}; /* End of 'bodx::win::OnMouseWheel' function */
  
/* WM_MBUTTONDOWN window messages handle function.
  * ARGUMENTS:
  *   - mouse is doble clicked:
  *       BOOL IsDblClick;
  *   - mouse window position:
  *       INT X, Y;
  *   - mouse keys bits (see MK_*** bits constants):
  *       UINT Keys;
  * RETURNS: None.
  */
  VOID bodx::win::OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys )
  {
  }; /* End of 'bodx::win::OnButtonDown' function */

/* WM_MBUTTONUP window messages handle function.
  * ARGUMENTS:
  *   - mouse window position:
  *       INT X, Y;
  *   - mouse keys bits (see MK_*** bits constants):
  *       UINT Keys;
  * RETURNS: None.
  */
VOID bodx::win::OnButtonUp( INT X, INT Y, UINT Keys )
{
}; /* End of 'bodx::win::OnButtonUp' function */

/* END OF 'winmsg.cpp' FILE */