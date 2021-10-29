/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : default.cpp
 * PURPOSE     : Animation project.
 *               Units collection.
 *               Test unit file.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 27.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "../../animation/animation.h"

#include <bodx.h>
#include "../../bodx.h"

namespace bodx
{ 
  class test_unit : public unit
  {
  private:
    matr Transform;
  public:
    test_unit( animation *Ani )
    {
      char buf[30];
      sprintf(buf, "%lf",Ani->FPS);
      SetWindowText(Ani->hWnd, (LPCSTR)buf);
     }

    ~test_unit( VOID )
    {
      /* Do not look here, this is 'costyl' */
      //bodx::animation::GetPtr()->Free(Cow);
    }
    VOID Response( animation *Ani ) override
    {  
      char buf[30];
      sprintf(buf, "%lf",Ani->FPS);
      SetWindowText(Ani->hWnd, (LPCSTR)buf);
    }
    VOID Render( animation *Ani ) override
    {
      //Ani->Draw(Cow, Transform);
    }
  };
  static animation::unit_register<test_unit> _("Test");
}

/* END OF 'default.cpp' FILE */