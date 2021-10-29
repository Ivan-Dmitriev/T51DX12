/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : buffers.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               Resources.
 *               Uniform buffers.
 * PROGRAMMER  : Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "buffers.h"
#include "../../../bodx.h"

/* Apply buffers 
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID bodx::buffer::Apply( VOID )
{
  if (BindPoint != 0)
    glBindBufferBase(GL_UNIFORM_BUFFER, BindPoint, Id);
} // end of 'bodx::buffer::Apply' function

/* Free buffers 
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID bodx::buffer::Free( VOID )
{
  if (Id != 0)
  {
    glDeleteBuffers(1, &Id);
    Id = 0;
    BindPoint = 0;
    NumOfQuads = 0;
  }
} // end of 'bodx::buffer::Free' function

/* END OF 'buffers.cpp' FILE */