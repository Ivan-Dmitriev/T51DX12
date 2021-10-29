/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : timer.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Timer system.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __timer_h_
#define __timer_h_

#include "../def.h"

/* Project name space */
namespace bodx
{
  /* Timer class */
  class timer
  {
  private:
    UINT64
      StartTime,      /* Start program time */
      OldTime,        /* Previous frame time */
      OldTimeFPS,     /* Old time FPS measurement */
      PauseTime,      /* Time during pause period */
      TimePerSec,     /* Timer resolution */
      FrameCounter;   /* Frames counter */
    BOOL _IsPaused;   /* Is pause flag */
  public:
    const BOOL& IsPaused = _IsPaused; /* read-only list length */
    DBL
      Time,
      DeltaTime,
      GlobalTime,
      GlobalDeltaTime,
      FPS;

    /* Default constructor */
    timer( VOID ) :
      FrameCounter(0),
      PauseTime(0),
      _IsPaused(FALSE),
      Time(0),
      DeltaTime(0),
      GlobalTime(0),
      GlobalDeltaTime(0),
      FPS(0)
    {
      LARGE_INTEGER t;

      QueryPerformanceFrequency(&t);
      TimePerSec = t.QuadPart;
      QueryPerformanceCounter(&t);
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
    } /* End of constructor */

    VOID Reset( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceCounter(&t);
      StartTime = OldTime = OldTimeFPS = t.QuadPart;
    }

    /* Pause timer function */
    VOID Pause( VOID )
    {
      _IsPaused = TRUE;
    } /* End of 'Pause' function */

    /* Continue timer function */
    VOID Continue( VOID )
    {
      _IsPaused = FALSE;
    } /* End of 'Continue' function */

    /* Timer interframe response function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( VOID )
    {
      LARGE_INTEGER t;

      QueryPerformanceCounter(&t);
      /* Global time */
      GlobalTime = static_cast<DBL>(t.QuadPart - StartTime) / TimePerSec;
      GlobalDeltaTime = static_cast<DBL>(t.QuadPart - OldTime) / TimePerSec;
      /* Time with pause */
      if (_IsPaused)
      {
        DeltaTime = 0;
        PauseTime += t.QuadPart - OldTime;
      }
      else
      {
        DeltaTime = GlobalDeltaTime;
        Time = static_cast<DBL>(t.QuadPart - PauseTime - StartTime) / TimePerSec;
      }
      /* FPS */
      FrameCounter++;
      if (t.QuadPart - OldTimeFPS > TimePerSec)
      {
        FPS = FrameCounter * TimePerSec / static_cast<DBL>(t.QuadPart - OldTimeFPS);
        OldTimeFPS = t.QuadPart;
        FrameCounter = 0;
      }
      OldTime = t.QuadPart;
    }
  }; /* End of 'timer' class */
} /* end of 'bodx' name space */

#endif /* __timer_h_ */

/* END OF 'timer.h' FILE */