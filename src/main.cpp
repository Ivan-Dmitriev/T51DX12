/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : main.cpp
 * PURPOSE      : Animation project.
 *                Entry point.
 * PROGRAMMER   : Ivan Dmitriev.
 * LAST UPDATE  : 25.10.2021.
 * NOTE         : Module namespace 'ivdx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <ivdx.h>

#include <commctrl.h>
#include <windowsx.h>

#include "ivdx.h"

#include <iostream>
//#include <commondf.h>

#include "samples/winmain.h"

/*
#include "mth/mth.h"
#include "win/win.h"
#include "win/win.h"
#include "anim/anim.h"
#include "def.h"
*/

 /* The main program function.
  * ARGUMENTS:
  *   - handle of application instance:
  *       HINSTANCE hInstance;
  *   - dummy handle of previous application instance (not used):
  *       HINSTANCE hPrevInstance;
  *   - command line string:
  *       CHAR *CmdLine;
  *   - show window command parameter (see SW_***):
  *       INT CmdShow;
  * RETURNS:
  *   (INT) Error level for operation system (0 for success).
  */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  if (hPrevInstance != NULL || CmdLine == NULL || CmdShow == -1)
      return -1;
  InitCommonControls();
  
  //ivdx::win_main MyWin(hInstance);

  ivdx::anim &myw = ivdx::anim::Get();
  //ivdx::win_main MyWin;

  //MyWin.Run();  
  myw.Run();

  return 0;

  //return myw.Run();
} /* End of 'WinMain' function */

  /* END OF 'main.cpp' FILE */