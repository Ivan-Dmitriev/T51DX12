/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : animation.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Animation interface.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>
#include "../bodx.h"

bodx::animation bodx::animation::Instance;

/* Render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Render( VOID )
{
  timer::Response();
  input::Response();
  render::Start(Game.Tick(Keys, KeysClick));
  render::End();
} /* End of 'bodx::animation::Render' function */

/* Resize window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::AnimResize( VOID )
{
  render::Resize();
  Render();
  render::CopyFrame();
}; /* End of 'bodx::animation::Resize' function */

/* Draw animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::AnimDraw( VOID )
{
  Render();
  render::CopyFrame();
}; /* End of 'bodx::animation::AnimDraw' function */

/* END OF 'animation.cpp' FILE */