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
    HBITMAP hAvatarAND; // Test image
    HBITMAP hAvatarXOR; // Test image

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
      //  BITMAP bm;
      //  GetObject(hAvatar, sizeof(bm), &bm);

      //  BITMAPINFOHEADER bmh {};
      //  bmh.biSize = sizeof(BITMAPINFOHEADER);
      //  bmh.biBitCount = bm.bmBitsPixel;
      //  bmh.biWidth = bm.bmWidth;
      //  bmh.biHeight = bm.bmHeight;
      //  bmh.biPlanes = 1;

      //  StretchDIBits(DrawItem->hDC, 0, 0, bm.bmWidth, bm.bmHeight, 0, 0, bm.bmWidth, bm.bmHeight, bm.bmBits, (BITMAPINFO *)&bmh, DIB_RGB_COLORS, SRCCOPY);
      //}
      if (Id == IDC_MY3D)
      {
        BITMAP bitmap;
        HDC hMemDC;
        HGDIOBJ oldBitmap;

        hMemDC = CreateCompatibleDC(DrawItem->hDC);
        oldBitmap = SelectObject(hMemDC, hAvatarAND);

        GetObject(hAvatarAND, sizeof(bitmap), &bitmap);
        BitBlt(DrawItem->hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCAND);

        oldBitmap = SelectObject(hMemDC, hAvatarXOR);

        GetObject(hAvatarXOR, sizeof(bitmap), &bitmap);
        BitBlt(DrawItem->hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCINVERT);
      }

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
      hAvatarAND = (HBITMAP)LoadImage(hInstance, (CHAR *)IDB_BITMAP_AND, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
      hAvatarXOR = (HBITMAP)LoadImage(hInstance, (CHAR *)IDB_BITMAP_XOR, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    } /* End of 'dlg_about' function */

    /* Class destructor */
    ~dlg_about( VOID ) override
    {
    } /* End of '~dlg_about' function */
  }; /* End of 'dlg_about' class */
} /* end of 'ivdx' namespace */

#endif /* __dlgabout_h_*/

/* END OF 'dlgabout.h' FILE */
