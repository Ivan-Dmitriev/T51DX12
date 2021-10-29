/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : win.h
 * PURPOSE     : Animation DirectX 12 project.
 *               WinApi.
 *               Window class declaration.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx::win'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __win_h_ 
#define __win_h_

#include "../../def.h"

/* Project name space */
namespace bodx
{
  class dialog;

  /* Window handler class */
  class win
  {
    friend dialog;
  protected:
    HWND hWnd;           // window handle
    INT W = 0, H = 0;    // window size
    HINSTANCE hInstance; // application handle
    INT MouseWheel = 0;  //mouse wheel state/change
 
    /* Draw animation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID AnimDraw( VOID )
    {
    } /* End of 'AnimDraw' function */

    /* Resize animation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID AnimResize( VOID )
    {
    } /* End of 'AnimResize' function */

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
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );
 
    RECT FullScreenSaveRect = RECT(); //full screen save rectangle
 
  public:
    BOOL IsActive = FALSE; //is window active
    BOOL IsInit = FALSE;   //initialize flag
 
    /* Constructor declaration */
    win( HINSTANCE hInstanceNew = GetModuleHandle(nullptr) ) : hInstance(hInstanceNew)
    {
    } /* End of 'win' function */

    /* Class destructor */
    virtual ~win( VOID )
    {
    } /* End of '~win' function */
 
    /* Run window function declaration */
    INT Run( VOID );     
    
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
      return FALSE;
    } /* End of 'OnCreate' function */

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT nW, nH;
     * RETURNS: None.
     */
    virtual VOID OnSize( UINT State, INT nW, INT nH )
    {
      W = nW;
      H = nH;
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
    virtual VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys )
    {
    } /* End of 'OnMouseWheel' function */

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
    virtual VOID OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys )
    {
    } /* End of 'OnButtonDown' function */

    /* WM_MBUTTONUP window messages handle function.
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
 
    /* WM_ERASEBKGND window message handle function.
     * ARGUMENTS:
     *   - device context of client area:
     *       HDC hDC;
     * RETURNS:
     *   (BOOL) TRUE if background is erased, FALSE otherwise.
     */
    virtual BOOL OnEraseBackground( HDC hDC )
    {
      return FALSE;
    } /* End of 'OnEraseBackground' function */
 
    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    virtual VOID OnPaint( HDC hDC )
    {
    } /* End of 'OnPaint' function */

    virtual VOID OnClose( VOID )
    {
    } /* End of 'OnClose' function */

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    virtual VOID OnDestroy( VOID )
    {
    } /* End of 'OnDestroy' function */

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
} /* end of 'bodx' name space */

#endif /* __win_h_ */

/* END OF 'win.h' FILE */