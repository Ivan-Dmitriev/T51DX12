/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12_shader.h
  * PURPOSE     : Animation project.
  *               Direct X12.
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __dx12_shader_h_
#define __dx12_shader_h_

#include <fstream>

#include "dx12.h"


/* Beginning of 'ivdx' namespace */
namespace ivdx
{
  /* Beginning of 'dx12' namespace */
  namespace dx12
  {
    /* Beginning of 'texture' class */
    class texture
    {
    private:
      std::string Filename;
    public:
      /* default constructor */
      texture( std::string NewFilename );
      VOID Load( VOID )
      {
      }

    }; /* End of 'texture' class */
  } /* end of 'dx12' namespace */
} /* end of 'ivdx' namespace */

#endif /* __dx12_shader_h_ */

/* END OF 'dx12_shader.h' FILE */
