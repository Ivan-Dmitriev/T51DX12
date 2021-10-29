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

/* Initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Init( VOID )
{
}; /* End of 'bodx::animation::Init' function */

/* Deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Close( VOID )
{
}; /* End of 'bodx::animation::Close' function */

/* Resize window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Resize( VOID )
{
  render::Resize();
  Render();
  render::CopyFrame();
}; /* End of 'bodx::animation::Resize' function */

/* Update timer function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Timer( VOID )
{
  Render();
  render::CopyFrame();
}; /* End of 'bodx::animation::Timer' function */

/* Activation window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Activate( VOID )
{
}; /* End of 'bodx::animation::Activate' function */

/* Idle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Idle( VOID )
{
  Render();
  render::CopyFrame();
}; /* End of 'bodx::animation::Idle' function */

/* Erase background function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Erase( HDC hDC )
{
}; /* End of 'bodx::animation::Erase' function */

/* Paint window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::animation::Paint( HDC hDC )
{
}; /* End of 'bodx::animation::Paint' function */

/* END OF 'animation.cpp' FILE */