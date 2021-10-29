/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : anim.h
 * PURPOSE     : Animation project.
 *               Mathematics library.
 *               Space camera handle module.
 * PROGRAMMER  : Ivan Dmitriev.
 * LAST UPDATE : 25.10.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __anim_h_
#define __anim_h_

#include <cstdio>
#include <map>

#include "render/dx/dx12.h"
//#include "../win/win.h"
#include "../samples/winmain.h"

#include "input.h"
#include "timer.h"
#include "render/render.h"

namespace ivdx 
{
  /* Forward declaraion of anim class */
  class anim;
  
  /* Animation system class */
  class anim : public win_main, public timer, public input, public render, public dx12::core 
  {
  private:
    static anim Instance;
  public:
    std::string Path;
    //tetris T;
   /* Get current directory function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID GetCurrentDir( VOID )
    {
      CHAR Buf[_MAX_PATH];
      //GetCurrentDirectory(sizeof(Buf), Buf);
      anim::Path = Buf;
    } /* End of 'GetCurrentDir' function */

  private:
    //stock<unit *> MyScene;
    anim( VOID ) : win_main(), input(win::MouseWheel, win::hWnd), render(win::hWnd, win::Width, win::Height), core(win::hWnd)
    {
      //T.Init();
    }
  public:
  
    //unit *Scene;
    static anim & Get( VOID )
    {
      return Instance;
    }
    static anim * GetPtr( VOID )
    {
      return &Instance;
    }
  public:
   /* Init function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Init( VOID ) override
    {
    } /* End of 'Init' function */
   /* Close function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Close( VOID ) override
    {
    } /* End of 'Close' function */
   /* Resize function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Resize( VOID ) override
    {
      //dx12::core::Resize(W, H);
      //render::Resize();
      //Render();
      //CopyFrame();
    } /* End of 'Resize' function */

    /* Paint function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID Paint( HDC hDC )
    {
      //Rectangle(hDC, -1, -1, W, H);
      //SelectObject(hDC, GetStockObject(DC_BRUSH));
      //SetDCBrushColor(hDC, RGB(255, 0, 0));
      //Ellipse(hDC, -1, -1, MouseX - 10, MouseY - 10);
      //Ellipse(hDC, -1, -1, W - 10, H - 10);
    } /* End of 'Paint' function */

    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Idle( VOID ) override
    {
      Render();
      //render::CopyFrame();
    } /* End of 'Idle' function */

    /* Timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Timer( VOID ) override
    {
      Render();
      //render::CopyFrame();
      //keyboard::Response();
      //timer::Response();
    } /* End of 'Timer' function */

    /* Render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID )
    {
      //static DBL global_delta_time  = 0;

      //global_delta_time += GlobalDeltaTime;

      //if (global_delta_time > 2.5)
      //{
      //  T.Step();
      //  global_delta_time = 0;
      //}

      input::Response();
      timer::Response();

      dx12::core::Render(dx12::core::Game.Tick(Keys, KeysClick));

      //dx12::core::T.Responce(Keys);

      dx12::core::Response(Keys, GlobalDeltaTime, MouseDX, MouseDY);

      CHAR Buf[100];
      //sprintf(Buf, "%.3f\n", FPS);

      sprintf(Buf, "Score - %d\n", dx12::core::Game.Score);
      SetWindowTextA(hWnd, Buf);
      if (KeysClick['F'])
        FlipFullScreen();

      //render::StartFrame();
      //render::EndFrame();
    } /* End of 'Render' function */
  }; /* End of 'anim' class */
} /* end of 'ivgl' namespace */

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
