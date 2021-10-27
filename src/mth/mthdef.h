/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mthdef.h
 * PURPOSE     : Animation project.
 *               Mathematics library.
 *               Common definitions handle module.
 * PROGRAMMER  : Ivan Dmitriev.
 * LAST UPDATE : 08.01.2021
 * NOTE        : Module namespace 'mth'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mthdef_h_
#define __mthdef_h_

#include <cmath>
#include <cassert>
#include <cstdlib>
#include <commondf.h>

#define PI 3.14159265358979323846
#define Degree2Radian(a) D2R(a)
#define Degree2RadianConst 0.01745329251994329576923690768489
#define Radian2DegreeConst 57.295779513082320876798154814105

/* Useful macro type */
typedef DOUBLE DBL;
typedef FLOAT FLT;
typedef unsigned int UINT;

template<class Type> class vec2;
template<class Type> class vec3;
template<class Type> class vec4;
template<class Type> class matr;

namespace math
{
  template<class Type>
   /* Clamp value between two variables function.
    * ARGUMENTS: 
    *   - Variable needs to be clamped:
    *       Type Value;
    *   - Borders of clamping:
    *       Type min, max; 
    * RETURNS: (Type) result value.  
    */
    static Type Clamp( Type Value, Type min = Type(0), Type max = Type(1) )
    {
      return Value < min ? min : Value > max ? max : Value;
    } /* End of 'Clamp' function */
  template<class Type>
   /* Get one random number between -1 and 1 function.
    * ARGUMENTS: None; 
    * RETURNS: (Type) result value.
    */
    static Type Rnd0( VOID )
    {
      return 2.0 * rand() / RAND_MAX - 1;
    } /* End of 'Rnd0' function */
   /* Get one float random number between -1 and 1 function.
    * ARGUMENTS: None; 
    * RETURNS: (FLT) result value.
    */
    static FLT Rnd0F( VOID )
    {
      return 2.0 * rand() / RAND_MAX - 1;
    } /* End of 'Rnd0F' function */
   /* Get one float random number between 0 and 1 function.
    * ARGUMENTS: None; 
    * RETURNS: (FLT) result value.
    */
    static FLT Rnd1F( VOID )
    {
      return 1.0 * rand() / RAND_MAX;
    } /* End of 'Rnd1F' function */

  template<class Type, class Type1>
   /* Linear interpolation between A and B values function.
    * ARGUMENTS:  
    *   - Borders of interpolation:
    *       Type A, B;
    *   - Value to be interpolated:
    *       Type1 T;
    * RETURNS: (Type1) result value.
    */
    Type1 Lerp( Type A, Type B, Type1 T )
    {
      return A * (1 - T) + B * T;
    } /* End of 'Lerp' function */

  template<class Type>
   /* Get squared number function.
    * ARGUMENTS:
    *   - Value to be interpolated:
    *       Type x;
    * RETURNS: (Type) result value.
    */
    static Type Sqr( Type x )
    {
      return x * x;
    } /* End of 'Sqr' function */

  template<class Type>

   /* Convert from degrees to radians function.
    * ARGUMENTS:
    *   - Value to be converted:
    *       Type A;
    * RETURNS: (Type) result value.
    */
    static Type D2R( Type A )
    {
      return (A) * (PI / 180.0);
    } /* End of 'Sqr' function */

  template<class Type>
   /* Convert from radians to degrees function.
    * ARGUMENTS:
    *   - Value to be converted:
    *       Type A;
    * RETURNS: (Type) result value.
    */
    static Type R2D( Type A )
    {
      return (A) * (180.0 / PI);
    } /* End of 'R2D' function */
  template<class Type>
   /* Get minimum value function.
    * ARGUMENTS:
    *   - Input values:
    *       Type A, B;
    * RETURNS: (Type) result value.
    */
    static Type Min( Type A, Type B )
    {
      return A < B ? A : B;
    } /* End of 'Min' function */
  template<class Type>
   /* Get maximum value function.
    * ARGUMENTS:
    *   - Input values:
    *       Type A, B;
    * RETURNS: (Type) result value.
    */
    static Type Max( Type A, Type B )
    {
      return A > B ? A : B;
    } /* End of 'Max' function */
  template<class Type>
   /* Swap two values function.
    * ARGUMENTS:
    *   - Input values:
    *       Type A, B;
    * RETURNS: (Type) result value.
    */
    VOID Swap( Type *A, Type *B )
    {
      Type tmp = *A;
       *A = *B;
       *B = tmp;
    } /* End of 'Swap' function */

} /* end of 'math' namespace */

#endif /* __mthdef_h_ */

/* END OF 'mthdef.h' FILE */