/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : input.h
 * PURPOSE      : Animation project.
 *                Input system handle module.
 * PROGRAMMER   : Ivan Dmitriev.
 * LAST UPDATE  : 25.10.2021
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __input_h_
#define __input_h_

#include <cstdlib>
#include <mmsystem.h>

#include "../def.h"

#pragma comment(lib, "winmm") /* winmm.lib */

/* Useful macro for getting joystick axis */
#define GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

namespace ivdx
{
  /* Mouse input system class */
  class mouse
  {
  public:
    INT &MouseWheel;
    /* mouse coordinates */
    INT MouseX, MouseY, MouseZ;
    /* mouse coordinates changing */
    INT MouseDX, MouseDY, MouseDZ;
    //INT MouseWheel_1;
    HWND hWnd2;
  public:
    INT Dz;
    mouse( INT &WheelRef, HWND NewhWnd ) : MouseWheel(WheelRef), Dz(0), hWnd2(NewhWnd)
    {
      MouseX = MouseY = MouseZ = MouseDX = MouseDY = MouseDZ = 0;
    }
    VOID Response( VOID )
    {
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(hWnd2, &pt);
      /* Transform coordinates on the screen */
      MouseDX = pt.x - MouseX;
      MouseDY = pt.y - MouseY;
      MouseDZ = MouseWheel;
      /* absolute val */
      MouseX = pt.x;
      MouseY = pt.y;
      MouseZ += MouseWheel;

      MouseWheel = 0;
    }
  }; /* End of 'mouse' class */

  /* Keyboard input system class */
  class keyboard
  {
  public:
    /* keyboard responsing */
    BYTE Keys[256];                /* state of keys on the current frame */
    BYTE KeysClick[256];           /* signs of a single click of the keyboard */

  protected:
    BYTE KeysOld[256];             /* state of keys on the current frame */
  public:
    keyboard( VOID ) 
    {
      //ZeroMemory(Keys, 256);
      memset(Keys, 0, 256);
      memset(KeysOld, 0, 256);
      memset(KeysClick, 0, 256);

      //ZeroMemory(KeysOld, 256);
      //ZeroMemory(KeysClick, 256);
    }
    VOID Response( VOID )
    {
      (VOID)GetKeyboardState(Keys);
      for (INT i = 0; i < 256; i++)
      {
        Keys[i] >>= 7;
        KeysClick[i] = Keys[i] && !KeysOld[i];

        /*
        if (Keys[i] == TRUE)
        {
          Keys[i] >>= 7;
          KeysClick[i] = Keys[i] && !KeysOld[i];
        } 
        */

      }
      memcpy(KeysOld, Keys, 256);
    } /* End of 'KeyboardResponse' function */

  }; /* End of 'keyboard' class */
  class joystick
  {
  protected:
    /* Joystick responsing */
    BYTE JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
    INT JPov;                                  /* Joystick point-of-view control [-1, 0..7] */
    DBL JoyX, JoyY, JoyZ, JoyR;                /* Joystick axes */
  public:
    joystick( VOID ) 
    {
      JPov = 0;
      JoyX = 0;
      JoyY = 0;
      JoyZ = 0;
      JoyR = 0;

      memset(JBut, 0, 32);
      memset(JButClick, 0, 32);
      memset(JButOld, 0, 32);
    }

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
    } /* End of 'JoystickResponse' function */
  };
  /* General input system class */
  class input : public mouse, public keyboard, public joystick
  {
  public:
    input( INT &WheelRef, HWND NewhWnd ) : mouse(WheelRef, NewhWnd)
    {
    }
    VOID Response( VOID )
    {
      mouse::Response();
      keyboard::Response();
      joystick::Response();
    }
  }; /* End of 'input' class */
} /* end of 'ivdx' namespace */

#endif /* __input_h_ */

/* END OF 'input.h' FILE */