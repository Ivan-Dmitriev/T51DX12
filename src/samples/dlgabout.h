/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : dlgabout.h
 * PURPOSE     : CGSG'Sr'2021 WinApi resourse sample project.
 *               About dialog window handle declaration module.
 * PROGRAMMER  : CGSG'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 28.10.2021
 * NOTE        : Project namespace 'ivdx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __dlgabout_h_
#define __dlgabout_h_

#include "dlg.h"
#include "../res/resource.h"

/* Project namespace */
namespace ivdx
{
  /* Window handle class */
  class dlg_about : public dlg
  {
  private:

    /***
     * Cracked message handle functions
     ***/

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
    BOOL OnInitDialog( HWND hWndFocus, LPARAM lParam ) override
    {
      //SendDlgItemMessage(hWnd, IDC_PROGRESS1, PBM_SETRANGE, 0, MAKELONG(0, 100));
      //SendDlgItemMessage(hWnd, IDC_PROGRESS1, PBM_SETPOS, 30, 0);
      return TRUE;
    } /* End of 'OnInitDialog' function */

    /* WM_DROPFILES window message handle function.
     * ARGUMENTS:
     *   - handle to an internal structure describing the dropped files:
     *       HDROP hDrop;
     * RETURNS: None.
     */
    VOID OnDropFiles( HDROP hDrop ) override
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
    VOID OnDrawItem( INT Id, DRAWITEMSTRUCT *DrawItem ) override
    {
      //if (Id == IDC_MY3D)
      //{
      //  // DrawShadowText(DrawItem->hDC, L"CGSG", 4, &DrawItem->rcItem, 0, RGB(255, 255, 0), RGB(0, 0, 0), 3, 5);
      //  Ellipse(DrawItem->hDC, DrawItem->rcItem.left, DrawItem->rcItem.top,
      //    DrawItem->rcItem.right, DrawItem->rcItem.bottom);
      //}
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
    VOID OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify ) override
    {
      switch (Id)
      {
      case IDOK:
      case IDCANCEL:
        EndDialog(hWnd, 30);
        return;
      }
    } /* End of 'OnCommand' function */

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - parent dialog window pointer:
     *       win *ParentWin;
     *   - application instance handle:
     *       HINSTANCE hInstanceNew;
     */
    dlg_about( win *ParentWin ) :
      dlg(IDD_ABOUT, ParentWin)
    {
    } /* End of 'dlg_about' function */

    /* Class destructor */
    ~dlg_about( VOID ) override
    {
    } /* End of '~dlg_about' function */
  }; /* End of 'dlg_about' class */
} /* end of 'ivdx' namespace */

#endif /* __dlgabout_h_*/

/* END OF 'dlgabout.h' FILE */
