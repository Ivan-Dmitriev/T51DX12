/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : core_refference.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core_refference class defenition.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 25.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __core_refferenceh_
#define __core_refference_h_

#include "../../../def.h"

/* Project name space */
namespace bodx
{
  /* DirectX 12 namecpace */
  namespace dx12
  { 
    class core; // forward declaration
    
    /* Core refference class */
    class core_ref
    {
      friend core;
    protected:
      static core *D3D;
    }; /* End of 'core_refference' class */
  } /* end of 'dx12' namespace */
} /* end of 'bodx' namespace */

#endif // __core_refference_h_


/* END OF 'core_refference.h' FILE */