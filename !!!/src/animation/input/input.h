/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : input.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Input system.
 *               Input system interface.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __input_h_
#define __input_h_

#include "../../def.h"
#include "keyboard.h"
#include "mouse.h"
#include "joystick.h"

/* Project name space */
namespace bodx
{
  /* Input system class */
  class input : public keyboard, public mouse, public joystick
  { 
  protected:
    /* Constructor */
    input( INT &WheelRef, HWND hWnd ) : keyboard(), mouse(WheelRef, hWnd), joystick()
    {
    } /* End of constructor */

    /* Input inter frame events handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response(VOID)
    {
      keyboard::Response();
      mouse::Response();
      joystick::Response();
    } /* End of 'Response' function */
  }; /* End of 'input' class */
} /* end of 'bodx' name space */

#endif /* __input_h_ */

/* END OF 'input.h' FILE */