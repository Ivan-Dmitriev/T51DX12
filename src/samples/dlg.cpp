/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : dlg.cpp
 * PURPOSE     : CGSG'Sr'2021 WinApi resourse sample project.
 *               Dialog handle implementation module.
 * PROGRAMMER  : CGSG'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 28.10.2021
 * NOTE        : Project namespace 'ivdx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <ivdx.h>
#include "../ivdx.h"

#include "dlg.h"

// Map for storing all dialog windows
std::map<HWND, ivdx::win *> ivdx::dlg::Windows;

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
INT_PTR CALLBACK ivdx::dlg::DlgFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  dlg *Win = nullptr;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.y =
      GetSystemMetrics(SM_CYCAPTION) * 8 +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.x += 300 + 102 * 5 + 47;
    return 0;
  case WM_INITDIALOG:
    Windows[hWnd] = reinterpret_cast<dlg *>(lParam);
    Windows[hWnd]->hWnd = hWnd;
  default:
    if (Windows.find(hWnd) == Windows.end())
      break;
    Win = dynamic_cast<dlg *>(Windows[hWnd]);
    switch (Msg)
    {
    case WM_INITDIALOG:
      return Win->OnInitDialog((HWND)(wParam), lParam);
    case WM_SIZE:
      Win->OnSize((UINT)wParam, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam));
      return 0;
    case WM_TIMER:
      Win->OnTimer((UINT)wParam);
      return 0;
    case WM_DRAWITEM:
      Win->OnDrawItem((INT)wParam, (DRAWITEMSTRUCT *)lParam);
      return 0;
    case WM_NOTIFY:
      return Win->OnNotify((INT)wParam, (NMHDR *)lParam);
    case WM_DROPFILES:
      Win->OnDropFiles((HDROP)wParam);
      return 0;
    case WM_COMMAND:
      Win->OnCommand((INT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
      return 0;
    }
  }
  return FALSE;
} /* End of 'sxfh::dlg_main::DlgFunc' function */

/* END OF 'dlg.cpp' FILE */