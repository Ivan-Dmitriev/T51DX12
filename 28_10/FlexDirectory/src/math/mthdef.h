/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mthdef.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               Math types, constans and standard functions.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mthdef_h_
#define __mthdef_h_

#include <cmath>
#include <cstdlib>

#include <windows.h>

/* Default types */
typedef double DBL;
typedef float FLT;

/* Math name space */
namespace mth
{
  /* Forward declaration */
  template<typename type> class vec2;
  template<typename type> class vec3;
  template<typename type> class vec4;
  template<typename type> class matr;
  template<typename type> class ray;

  /* math consts */
  static constexpr DBL PI = 3.14159265358979323846;

  /* Translate degrees to radians.
   * ARGUMENTS:
   *   - degrees:
   *       type A;
   * RETURNS: 
   *   (type) radians.
   */
  template<typename type>
  static type D2R( type A )
  {
    return (type)(A * (PI / 180.0));
  } /* End of 'D2R' function */

  /* Translate radians to degrees.
   * ARGUMENTS:
   *   - radians:
   *       type A;
   * RETURNS: 
   *   (type) radians.
   */
  template<typename type>
  static type R2D( type A )
  {
    return A * (180.0 / PI);
  } /* End of 'R2D' function */

  /* Minimum of 2 values.
   * ARGUMENTS:
   *   - values for comparing:
   *       type A, B;
   * RETURNS:
   *   (type) minimal value.
   */
  template<typename type>
  static type Min( type A, type B )
  {
    return A < B ? A : B;
  } /* End of 'Min' function */

  /* Minimum of 2 vectors.
   * ARGUMENTS:
   *   - vectors for comparing:
   *       vec3<type> A, B;
   * RETURNS:
   *   (type) minimal value.
   */
  template<typename type>
  static vec3<type> MinBB( vec3<type> A, vec3<type> B )
  {
    return vec3<type>(Min(A.X, B.X), Min(A.Y, B.Y), Min(A.Z, B.Z));
  } /* End of 'MinBB' function */

  /* Maximum of 2 values.
   * ARGUMENTS:
   *   - values for comparing:
   *       type A, B;
   * RETURNS:
   *   (type) maximal value.
   */
  template<typename type>
  static type Max( type A, type B )
  {
    return A > B ? A : B;
  } /* End of 'Max' function */

  /* Maximum of 2 vectors.
   * ARGUMENTS:
   *   - vectors for comparing:
   *       vec3<type> A, B;
   * RETURNS:
   *   (type) minimal value.
   */
  template<typename type>
  static vec3<type> MaxBB( vec3<type> A, vec3<type> B )
  {
    return vec3<type>(Max(A.X, B.X), Max(A.Y, B.Y), Max(A.Z, B.Z));
  } /* End of 'MaxBB' function */

  /* Clamp by 2 borders.
   * ARGUMENTS:
   *   - value for clamp:
   *       type Value;
   *   - min border:
   *       type Value;
   *   - max border:
   *       type Value;
   * RETURNS:
   *   (type) clamped value.
   */
  template<typename type>
  static type Clamp( type Value, type A = type(0), type B = type(1) )
  {
    return Value < A ? A : Value > B ? B : Value;
  } /* End of 'Clamp' function */

  /* Leaner interpolation.
   * ARGUMENTS:
   *   - first componet:
   *       type1 A;
   *   - second component:
   *       type1 B;
   *   - interpolation coefficen:
   *       type2 T;
   * RETURNS:
   *   (type1) result value.
   */
  template<typename type1, typename type2>
  static type1 Lerp( type1 A, type1 B, type2 T )
  {
    return A * (1 - T) + B * T;
  } /* End of 'Lerp' function */

  /* Swap function.
   * ARGUMENTS:
   *   - pointer to first componet:
   *       type *A;
   *   - pointer to second component:
   *       type *B;
   * RETURNS: None.
   */
  template<typename type>
  static void Swap( type *A, type *B )
  {
    type C = *A;
    *A = *B;
    *B = C;
  } /* End of 'Swap' function */
} /* end of 'mth' namespace */

#endif /* __mthdef_h_ */

/* END OF 'mthdef.h' FILE */