/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : joysick.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Input system.
 *               Joystick handler class.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __joystick_h_
#define __joystick_h_

#include <mmsystem.h>
#pragma comment(lib, "winmm") /* winmm.lib */
#include <cstring>

#include "../../def.h"

#define GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

/* Project name space */
namespace bodx
{
  /* Mouse handler class */
  class joystick
  {
  public: 
    /* joystick responsing */
    BYTE JBut[32], JButOld[32], JButClick[32]; /* joystick button states */
    INT JPov;                                  /* joystick point-of-view control [-1, 0..7] */
    DBL JoyX, JoyY, JoyZ, JoyR;                /* joystick axes */
    /* Constructor */
    joystick( VOID ) :
      JoyX(0), JoyY(0), JoyZ(0), JoyR(0), JPov(0)
    {
      memset(JBut, 0, 32);
      memset(JButOld, 0, 32); 
      memset(JButClick, 0, 32);
    } /* End of constructor */

    /* Animation joystick response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      INT i;

      if (joyGetNumDevs() > 0)
      {
        JOYCAPS jc;

        /* get joysick info */
        if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
        {
          JOYINFOEX ji;

          ji.dwSize = sizeof(JOYINFOEX);
          ji.dwFlags = JOY_RETURNALL;
          if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
          {
            /* buttons */
            for (i = 0; i < 32; i++)
            {
              JBut[i] = (ji.dwButtons >> i) & 1;
              JButClick[i] = JBut[i] && !JButOld[i];
              JButOld[i] = JBut[i];
            }
            /* axes */
            JoyX = GET_JOYSTIC_AXIS(X);
            JoyY = GET_JOYSTIC_AXIS(Y);
            JoyZ = GET_JOYSTIC_AXIS(Z);
            JoyR = GET_JOYSTIC_AXIS(R);
          }
        }
      }
    } /* End of 'DG5_AnimKeyboardResponse' function */
  }; /* End of 'joystick' class */
} /* end of 'bodx' name space */

#endif /* __joystick_h_ */

/* END OF 'joystick.h' FILE */