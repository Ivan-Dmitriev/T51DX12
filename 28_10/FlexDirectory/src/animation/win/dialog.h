/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : dialog.h
 * PURPOSE     : CGSG'Sr'2021 WinApi resourse sample project.
 *               Dialog handle declaration module.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 28.10.2021
 * NOTE        : Project namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __dialog_h__
#define __dialog_h__

#include "win.h"
#include "../../def.h"

/* Project namespace */
namespace bodx
{
  /* Dialog window base class */
  class dialog : public win
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
    static INT_PTR CALLBACK DialogFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
    dialog( INT DialogId, win *ParentWin = nullptr, HINSTANCE hInst = GetModuleHandle(nullptr) ) :
      win(hInst), DialogResId(DialogId), ParentWnd(ParentWin)
    {
    } /* End of 'dialog' function */

    /* Class destructor */
    ~dialog( VOID ) override
    {
    } /* End of '~dialog' function */

    /* Dialog run function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Run( VOID )
    {
      DialogBoxParam(hInstance, (CHAR *)(INT_PTR)DialogResId, ParentWnd == nullptr ? nullptr : ParentWnd->hWnd, DialogFunc, reinterpret_cast<LPARAM>(this));
    } /* End of 'Run' function */

    /* Dialog create function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Create( VOID )
    {
      hWnd = CreateDialogParam(hInstance, (CHAR *)(INT_PTR)DialogResId, ParentWnd == nullptr ? 
        nullptr : ParentWnd->hWnd, DialogFunc, reinterpret_cast<LPARAM>(this));

      RECT rc;
      GetClientRect(hWnd, &rc);
      W = rc.right;
      H = rc.bottom;
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
  }; /* End of 'dialog' class */

} /* end of 'bodx' namespace */
#endif /* __dialog_h__ */

/* END OF 'dialog.h' FILE */