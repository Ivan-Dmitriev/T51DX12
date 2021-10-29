/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : win_main.h
 * PURPOSE     : Animation DirectX 12 project.
 *               WinApi.
 *               Window class declaration.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx::win_main'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __win_main_h_ 
#define __win_main_h_

#include "../../def.h"
#include "win.h"
#include "dialog_about.h"

/* Project name space */
namespace bodx
{
  /* Window class name */
  const CHAR WindowClassName[] = "CGSG'Sr'2021 DS2";

  /* Window handle class */
  class win_main : public win
  {
  private:
    static constexpr INT InitTimer = 130;
    static constexpr INT RefreshTimer = 30;

    dialog_about MyTool; // Smaple tool window
    HACCEL hAccel;       // Accelerator table

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
    BOOL OnCreate( CREATESTRUCT *CS ) override;

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnDestroy( VOID ) override;

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT nW, nH;
     * RETURNS: None.
     */
    VOID OnSize( UINT State, INT nW, INT nH ) override;

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID OnPaint( HDC hDC ) override
    {
      //if (IsInit)
      //  Paint(hDC);
    } /* End of 'OnPaint' function */

    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    BOOL OnEraseBackground( HDC hDC ) override
    {
      //if (IsInit)
      //  Erase(hDC);
      return TRUE;
    }; /* End of 'OnEraseBackground' function */

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
    VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized ) override
    {
      IsActive = Reason == WA_CLICKACTIVE || Reason == WA_ACTIVE;
      //if (IsInit)
      //  Activate();
    }; /* End of 'OnActivate' function */

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    VOID OnTimer( INT Id ) override;
 
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
    VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys ) override
    {
      MouseWheel += Z;
    }; /* End of 'OnMouseWheel' function */

    /* WM_DROPFILES window message handle function.
     * ARGUMENTS:
     *   - handle to an internal structure describing the dropped files:
     *       HDROP hDrop;
     * RETURNS: None.
     */
    VOID OnDropFiles( HDROP hDrop ) override;

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
    VOID OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify ) override;

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - application instance handle:
     *       HINSTANCE hInstanceNew;
     */
    win_main( HINSTANCE hInstanceNew = GetModuleHandle(nullptr) );

    /* Class destructor */
    ~win_main( VOID ) override
    {
    } /* End of '~win_main' function */
  }; /* End of 'win_main' class */
} /* end of 'bodx' name space */

#endif /* __win_main_h_ */

/* END OF 'win_main.h' FILE */