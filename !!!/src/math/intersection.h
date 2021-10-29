 /*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : intersection.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __intersection_h_
#define __intersection_h_

#include "mthdef.h"

/* Math support namespace */
namespace mth
{
  /* Vec3 handle class */
  template <typename type>
  class ray
  {
  private:
    vec3<type> Org, Dir;
  public:
    ray( VOID ) : Org(0), Dir(0, 1, 1)
    {
    }
    ray( vec3<type> O, vec3<type> D ) : Org(O), Dir(D)
    {
    }

    BOOL IntersectSphere( vec3<type> C, type R )
    {
      vec3<type> a = C - Org;
      FLT OC2, OK, OK2, R2, h2;

      OC2 = a & a;
      OK = a & Dir;
      OK2 = (a & Dir) * (a & Dir);
      R2 = R * R;
      h2 = R2 - (OC2 - OK2);
      if (OC2 < R2)
        return TRUE;
      if (OK < 0 || h2 < 0)
        return FALSE;
      return TRUE;
    }

  }; /* End of 'ray' class */
} /* End of 'mth' namespace */

#endif /* __intersection_h_ */

/* END OF 'intersection.h' FILE */