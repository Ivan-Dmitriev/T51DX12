/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : timer.h
 * PURPOSE      : Animation project.
 *                Timer handle module.
 * PROGRAMMER   : Ivan Dmitriev.
 * LAST UPDATE  : 25.10.2021
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __timer_h_
#define __timer_h_

#include <commondf.h>

#include "../mth/mthdef.h"

namespace ivdx 
{
  class timer
  {
  private:
    UINT64 StartTime,           /* Start program time */
                  OldTime,      /* Previous frame time */
                  OldTimeFPS,   /* Old time FPS measurement */
                  PauseTime,    /* Time during pause period */
                  TimePerSec,   /* Timer resolution */
                  FrameCounter; /* Frames counter */
  public:
    DBL
      GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
      Time, DeltaTime,             /* Time with pause and interframe interval */
      FPS;                         /* Frame per second */
    BOOL IsPause;                  /* Pause flag */
  public:
    timer( VOID )
    {
      TimerInit();
    }
    //VOID Timer( VOID );
   
    /* Get FPS function.
     * ARGUMENTS: None.
     * RETURNS: (DBL) Result number.
     */
    DBL GetFPS( VOID )
    {
      return FPS;
    } /* End of 'GetFPS' function */

    /* Initialization timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID TimerInit( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceFrequency(&t);
      TimePerSec = t.QuadPart;
      QueryPerformanceCounter(&t);
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
      FrameCounter = 0;
      IsPause = FALSE;
      FPS = 30.0;
      PauseTime = 0;
    } /* End of 'Init' function */

    /* Timer response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceCounter(&t);
      /* Global time */
      GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
      /* Time with pause */
      if (IsPause)
      {
        DeltaTime = 0;
        PauseTime += t.QuadPart - OldTime;
      }
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
      }
      /* FPS */
      FrameCounter++;
      if (t.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
        OldTimeFPS = t.QuadPart;
        FrameCounter = 0;
      }
      OldTime = t.QuadPart;
    } /* End of 'Response' function */
  }; /* End of 'timer' class */
} /* end of 'ivdx' namespace */

#endif /* __timer_h_ */

/* END OF 'timer.h' FILE */