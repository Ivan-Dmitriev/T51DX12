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
#define MAIN_WINDOW_CLASS_NAME "My WindowClass"

namespace ivdx
{
  /* window handle class */
  class win
  {
  protected:
    HWND hWnd;           // window handle
    INT W, H;            // window size
    HINSTANCE hInstance; // application handle
    INT MouseWheel; /* mouse wheel state */
 
  private:
    static LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg,
                                     WPARAM wParam, LPARAM lParam );
 
    BOOL IsFullScreen = FALSE;
    RECT FullScreenSaveRect;
    BOOL IsInit = FALSE;
  public:
    BOOL IsActive;
 
    win( HINSTANCE hInst = GetModuleHandle(nullptr) );

    //~win( VOID );
 
    INT Run( VOID );
    VOID FlipFullScreen( VOID );
    /* Initialization function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
  private:  
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

  private:
    //message cracks
    VOID OnGetMinMaxInfo( MINMAXINFO *MinMax );
    BOOL OnCreate( CREATESTRUCT *CS );
    VOID OnSize( UINT State, INT W, INT H );
    VOID OnTimer( INT Id );
    VOID OnMouseWheel( INT X, INT Y, INT Z, UINT Keys );
    VOID OnButtonDown( BOOL IsDblClick, INT X, INT Y, UINT Keys );
    VOID OnButtonUp( INT X, INT Y, UINT Keys );
    BOOL OnEraseBackground( HDC hDC );
    VOID OnPaint( HDC hDC, PAINTSTRUCT *PS );
    VOID OnClose( VOID );
    VOID OnDestroy( VOID );
    VOID OnActivate( UINT Reason, HWND hWndActDeact, BOOL IsMinimized );
  }; /* End of 'win' class */
} /* end of 'ivdx' namespace */

#endif /* __win_h_ */

/* END OF 'win.h' FILE */