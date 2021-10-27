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
#include "ivdx.h"

#include <iostream>

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
  ivdx::anim &myw = ivdx::anim::Get();

  return myw.Run();
} /* End of 'WinMain' function */

  /* END OF 'main.cpp' FILE */