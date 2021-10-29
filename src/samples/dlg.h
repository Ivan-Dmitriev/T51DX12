/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : dlg.h
 * PURPOSE     : CGSG'Sr'2021 WinApi resourse sample project.
 *               Dialog handle declaration module.
 * PROGRAMMER  : CGSG'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 28.10.2021
 * NOTE        : Project namespace 'ivdx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __dlg_h__
#define __dlg_h__

#include <map>

#include "../win/win.h"

/* Project namespace */
namespace ivdx
{
  /* Dialog window base class */
  class dlg : public win
  {
  protected:
    // Dialog resource Id
    INT DialogResId;

    // Parent window
    win *ParentWnd;

    // Map for storing all dialog windows
    static std::map<HWND, win *> Windows;

    /* Dialog window message handle function.
     * ARGUMENTS:
     *   - handle of window:
     *       HWND hWnd;
     *   - message identifier (WM_*** and so on):
     *       UINT Msg;
     *   - first message parameter ('word param'):
     *       WPARAM wParam;
     *   - second message parameter ('long param'):
     *       LPARAM lParam;
     * RETURNS:
     *   (INT_PTR) Value according message type.
     */
    static INT_PTR CALLBACK DlgFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - dialog box resource Id:
     *       INT DialogId;
     *   - parent dialog window pointer:
     *       win *ParentWin;
     *   - handle of application instance:
     *       HINSTANCE hInst;
     */
    dlg( INT DialogId, win *ParentWin = nullptr, HINSTANCE hInst = GetModuleHandle(nullptr) ) :
      win(hInst), DialogResId(DialogId), ParentWnd(ParentWin)
    {
    } /* End of 'dlg' function */

    /* Class destructor */
    ~dlg( VOID ) override
    {
    } /* End of '~dlg' function */

    /* Dialog run function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Run( VOID )
    {
      DialogBoxParam(hInstance, (CHAR *)(INT_PTR)DialogResId, ParentWnd == nullptr ? nullptr : ParentWnd->hWnd, DlgFunc, reinterpret_cast<LPARAM>(this));
    } /* End of 'Run' function */

    /* Dialog create function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Create( VOID )
    {
      hWnd = CreateDialogParam(hInstance, (CHAR *)(INT_PTR)DialogResId, ParentWnd == nullptr ? nullptr : ParentWnd->hWnd, DlgFunc, reinterpret_cast<LPARAM>(this));

      RECT rc;
      GetClientRect(hWnd, &rc);
      Width = rc.right;
      Height = rc.bottom;
    } /* End of 'Create' function */

    /***
     * Message crackers
     ***/

    /* WM_INITDIALOG window message handle function.
     * ARGUMENTS:
     *   - control handle to be set focus:
     *       HWND hWndFocus;
     *   - additional dialog creation parameter:
     *       LPARAM lParam;
     * RETURNS:
     *   (BOOL) TRUE for use hWndFocus, FALSE for user focus window set.
     */
    virtual BOOL OnInitDialog( HWND hWndFocus, LPARAM lParam )
    {
      return TRUE;
    } /* End of 'OnInitDialog' function */
  }; /* End of 'dlg' class */

} /* end of 'ivdx' namespace */
#endif /* __dlg_h__ */

/* END OF 'dlg.h' FILE */