/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : anim.h
 * PURPOSE     : Animation project.
 *               WinAPI handle module.
 * PROGRAMMER  : .
 *               ID3
 * LAST UPDATE : 25.10.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __win_h_
#define __win_h_

#include "../def.h"

/* window class name */
//#define MAIN_WINDOW_CLASS_NAME "My WindowClass"

namespace ivdx
{
  /* Window class name */
  const CHAR WindowClassName[] = "CGSG'Sr'2021 WinApi Resourses Sample Window Class";

  /* window handle class */
  class win
  {
  public:
    /* Window handle */
    HWND hWnd {};

  protected:
    /* Window size */
    INT Width = 0, Height = 0;

    /* Application instance handle */
    HINSTANCE hInstance; 

    /* Mouse wheel delta value */
    INT MouseWheel; 
 
    static LRESULT CALLBACK WindowFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );
 
    BOOL IsFullScreen = FALSE;
    RECT FullScreenSaveRect;
    BOOL IsInit = FALSE;
 
    //INT Run( VOID );
    VOID FlipFullScreen( VOID );
    /* Initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Init( VOID )
    {
    } /* End of 'Init' function */
    /* Close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Close( VOID )
    {
    } /* End of 'Close' function */
    /* Resizing function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Resize( VOID )
    {
    } /* End of 'Resize' function */
    /* Timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Timer( VOID )
    {
    } /* End of 'Timer' function */
    /* Activisation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Activate( VOID )
    {
    } /* End of 'Activate' function */
    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Idle( VOID )
    {
    } /* End of 'Idle' function */
    /* Erase function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    virtual VOID Erase( HDC hDC )
    {
    } /* End of 'Erase' function */

    /* Paint function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    virtual VOID Paint( HDC hDC )
    {
    } /* End of 'Paint' function */

  public:

    /* Activity flag */
    BOOL IsActive;
    /* Class constructor.
     * ARGUMENTS:
     *   - application instance handle:
     *       HINSTANCE hInstanceNew;
     */
    win( HINSTANCE hInstanceNew = GetModuleHandle(nullptr) ) : hInstance(hInstanceNew)
    {
    } /* End of 'win' function */
    
    /* Class destructor */
    virtual ~win( VOID )
    {
    } /* End of '~win' function */
    /* Application main loop start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID Run( VOID )
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
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
        else
          /* Call idle virtual function */
          ;//Idle();
    } /* End of 'Run' function */


    /* Deleted operators for maximum safety */
    win & operator=( const win &Win ) = delete;
    win( const win &Win ) = delete;

  protected:
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
    virtual BOOL OnCreate( CREATESTRUCT *CS )
    {
      return TRUE;
    } /* End of 'OnCreate' function */

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnDestroy( VOID )
    {
    } /* End of 'OnDestroy' function */

    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    virtual BOOL OnEraseBkgnd( HDC hDC )
    {
      return FALSE;
    } /* End of 'OnEraseBkgnd' function */

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS: None.
     */
    virtual VOID OnPaint( HDC hDC )
    {
    } /* End of 'OnPaint' function */

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
    virtual VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized )
    {
    } /* End of 'OnActivate' function */

    /* WM_MOUSEWHEEL window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse wheel delta value (120 points per rotate):
     *       INT Z;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
    {
    } /* End of 'OnMouseWheel' function */

    /* WM_*BUTTONDOWN window message handle function.
     * ARGUMENTS:
     *   - double click flag:
     *       BOOL IsDoubleClick;
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnButtonDown( BOOL IsDoubleClick, INT X, INT Y, UINT Keys )
    {
    } /* End of 'OnButtonDown' function */

    /* WM_*BUTTONUP window message handle function.
     * ARGUMENTS:
     *   - mouse window position:
     *       INT X, Y;
     *   - mouse keys bits (see MK_*** bits constants):
     *       UINT Keys;
     * RETURNS: None.
     */
    virtual VOID OnButtonUp( INT X, INT Y, UINT Keys )
    {
    } /* End of 'OnButtonUp' function */

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    virtual VOID OnSize( UINT State, INT W, INT H )
    {
      Width = W;
      Height = H;
    } /* End of 'OnSize' function */

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */
    virtual VOID OnTimer( INT Id )
    {
    } /* End of 'OnTimer' function */

    /* WM_DROPFILES window message handle function.
     * ARGUMENTS:
     *   - handle to an internal structure describing the dropped files:
     *       HDROP hDrop;
     * RETURNS: None.
     */
    virtual VOID OnDropFiles( HDROP hDrop )
    {
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
    virtual VOID OnDrawItem( INT Id, DRAWITEMSTRUCT *DrawItem )
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
    virtual LRESULT OnNotify( INT Id, NMHDR *NoteHead )
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
    virtual VOID OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify )
    {
    } /* End of 'OnCommand' function */
  }; /* End of 'win' class */
} /* end of 'ivdx' namespace */

#endif /* __win_h_ */

/* END OF 'win.h' FILE */