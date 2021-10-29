/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : win_main.cpp
 * PURPOSE     : Animation project.
 *               WinApi.
 *               Window class functions.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 28.10.2021.
 * NOTE        : Module namespace 'bodx::win_main'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>
#include "../../bodx.h"

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
BOOL bodx::win_main::OnCreate( CREATESTRUCT *CS )
{
  SetTimer(hWnd,             // handle to main window 
           InitTimer,        // timer identifier 
           0,                // 0-second interval 
           (TIMERPROC)nullptr);
  return TRUE;
} /* End of 'bodx::win_main::OnCreate' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::win_main::OnDestroy( VOID )
{
  if (IsInit)
  {
    KillTimer(hWnd, RefreshTimer);
  }
  else
    KillTimer(hWnd, InitTimer);
  PostQuitMessage(0);
} /* End of 'bodx::win_main::OnDestroy' function */

/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT nW, nH;
 * RETURNS: None.
 */
VOID bodx::win_main::OnSize( UINT State, INT nW, INT nH )
{
  this->W = nW;
  this->H = nH;
  if (IsInit)
    AnimResize();
}; /* End of 'bodx::win_main::OnSize' function */


/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
VOID bodx::win_main::OnTimer( INT Id )
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
      //Init();
      AnimResize();
      AnimDraw();
    }
    else
      AnimDraw();
}; /* End of 'bodx::win_main::OnTimer' function */

/* WM_DROPFILES window message handle function.
 * ARGUMENTS:
 *   - handle to an internal structure describing the dropped files:
 *       HDROP hDrop;
 * RETURNS: None.
 */
VOID bodx::win_main::OnDropFiles( HDROP hDrop )
{
  CHAR Buf[1000];
  MSGBOXPARAMS mbp {};
  
  mbp.cbSize = sizeof(MSGBOXPARAMS);
  mbp.hwndOwner = hWnd;
  mbp.hInstance = hInstance;
  mbp.lpszCaption = "About";
  mbp.lpszIcon = (CHAR *)IDI_DS2_ICON;
  mbp.dwStyle = MB_OK | MB_USERICON;
  mbp.lpszText = Buf;
  LoadString(hInstance, IDS_ABOUT, Buf, sizeof(Buf));
  MessageBoxIndirect(&mbp);
} /* End of 'bodx::win_main::OnDropFiles' function */

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
VOID bodx::win_main::OnCommand( INT Id, HWND hWndCtl, UINT CodeNotify )
{
  switch (Id)
  {
  case ID_MENU_FLEX:
    PostQuitMessage(30);
    return;
  case ID_MENU_CGSGFOREVER:
    MyTool.Run();
    return;
  default:
    return;
  }
} /* End of 'bodx::win_main::OnCommand' function */

/* Class constructor.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInstanceNew;
 */
bodx::win_main::win_main( HINSTANCE hInstanceNew ) : win(hInstanceNew), MyTool(this)
{
  /* Register window class */
  WNDCLASSEX wc;

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = sizeof(VOID *);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(hInstance, (CHAR *)IDC_DS2_CURSOR);
  wc.hIcon = (HICON)LoadImage(hInstance, (CHAR *)IDI_DS2_ICON, IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);;
  wc.hIconSm = (HICON)LoadImage(hInstance, (CHAR *)IDI_DS2_ICON, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
  wc.lpszMenuName = (CHAR *)IDR_DS2_MENU;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WinFunc;
  wc.lpszClassName = WindowClassName;

  if (!RegisterClassEx(&wc))
  {
    MessageBox(nullptr, "Error register window class", "ERROR", MB_OK);
    return;
  }

  /* Create window */
  hWnd =
    CreateWindowEx(WS_EX_ACCEPTFILES,
      WindowClassName,
      "CGSG'Sr'2021 Resourses Sample. 30 forever!!!",
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr, nullptr, hInstance, (VOID *)this);

  hAccel = LoadAccelerators(hInstance, (CHAR *)IDR_DS2_ACCELERATOR);

  /* Show window */
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);
} /* End of 'bodx::win_main::win_main' function */

/* END OF 'win_main.cpp' FILE */