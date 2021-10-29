/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_vec3.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               3D space vector.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include "mthdef.h"

/* Math name space */
namespace mth {
  template<typename type>
  /* 3D vector representation type */
  class vec3
  {
  private:
    /* Delegate acces rights to other class */
    friend class matr<type>;
    type X, Y, Z;
  public:
    /* One value constructor.
     * ARGUMENTS: 
     *   - initial value:
     *       type val;
     * RETURNS: None.
     */
    explicit vec3( type val = 0 ) : X(val), Y(val), Z(val)
    {
    } /* End of one value constructor */

    /* Constructor.
     * ARGUMENTS: 
     *   - initial values:
     *       type xval, yval, zval;
     * RETURNS: None.
     */
    vec3( type xval, type yval, type zval ) : X(xval), Y(yval), Z(zval)
    {
    } /* End of constructor */

    /* Self-normalize vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3 &) normalized vector.
     */
    vec3 & Normalize( void )
    {
      vec3 v = vec3(X, Y, Z);
      v /= !v;
      X = v.X;
      Y = v.Y;
      Z = v.Z;

      return *this;
    } /* End of 'Normalize' function */

    /* Get normalized vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) normalized vector.
     */
    vec3 Normalized( void ) const
    {
      vec3 v = vec3(X, Y, Z);
      return v /= !v;
    } /* End of 'Normalized' function */

    /* Get vector square length method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (type) vector square length.
     */
    type Length2( void ) const
    {
      return X * X + Y * Y + Z * Z;
    } /* End of 'Length2' function */

    /* Get vector length method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (type) vector length.
     */
    type 
      Length( void ) const
    {
      return sqrt(X * X + Y * Y + Z * Z);
    } /* End of 'Length' function */

    /* Get distance to vector method.
     * ARGUMENTS:
     *   - vector:
     *       const vec3 &val;
     * RETURNS:
     *   (type) distance to vector.
     */
    type Distance( const vec3 &val ) const
    {
      return !vec3(X - val.X, Y - val.Y, Z - val.Z);
    } /* End of 'Distance' function */

    /* Set null-vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3 &) Self-reference.
     */
    vec3 & Zero( void )
    {
      X = 0;
      Y = 0;
      Z = 0;
      return *this;
    } /* End of 'Zero' function */

    /* Addition operator.
     * ARGUMENTS: 
     *   - term:
     *       const vec3 &val;
     * RETURNS:
     *   (vec3) result.
     */
    vec3 operator+( const vec3 &val ) const
    {
      return vec3(X + val.X, Y + val.Y, Z + val.Z);
    } /* End of 'operator+' function */

    /* Addition operator.
     * ARGUMENTS: 
     *   - term:
     *       const vec3 &val;
     * RETURNS: 
     *   (vec3 &) Self-reference.
     */
    vec3 & operator+=( const vec3 &val )
    {
      X += val.X;
      Y += val.Y;
      Z += val.Z;

      return *this;
    } /* End of 'operator+=' function */

    /* Subtraction operator.
     * ARGUMENTS: 
     *   - subtrahend:
     *       const vec3 &val;
     * RETURNS:
     *   (vec3) result.
     */
    vec3 operator-( const vec3 &val ) const
    {
      return vec3(X - val.X, Y - val.Y, Z - val.Z);
    } /* End of 'operator-' function */

    /* Subtraction operator.
     * ARGUMENTS: 
     *   - subtrahend:
     *       const vec3 &val;
     * RETURNS: 
     *   (vec3 &) Self-reference.
     */
    vec3 & operator-=( const vec3 &val )
    {
      X -= val.X;
      Y -= val.Y;
      Z -= val.Z;

      return *this;
    } /* End of 'operator-=' function */

    /* Negate operator.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec3) result.
     */
    vec3 operator-( VOID ) 
    {
      return vec3(-X, -Y, -Z);
    } /* End of 'operator-' function */

    /* Multiplication by coordinates operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec3 &val;
     * RETURNS:
     *   (vec3) result.
     */
    vec3 operator*( const vec3 &val ) const
    {
      return vec3(X * val.X, Y * val.Y, Z * val.Z);
    } /* End of 'operator*' function */

    /* Multiplication by coordinates operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec3 &val;
     * RETURNS: 
     *   (vec3 &) Self-reference.
     */
    vec3 & operator*=( const vec3 &val )
    {
      X *= val.X;
      Y *= val.Y;
      Z *= val.Z;

      return *this;
    } /* End of 'operator*=' function */

    /* Multiplication by number operator.
     * ARGUMENTS: 
     *   - multiplier number:
     *       n_type n;
     * RETURNS:
     *   (vec3) result.
     */
    template<typename n_type>
    vec3 operator*( n_type n ) const
    {
      return vec3(X * n, Y * n, Z * n);
    } /* End of 'operator* function */

    /* Multiplication by number operator.
     * ARGUMENTS:  
     *   - multiplier number:
     *       n_type n;
     * RETURNS: 
     *   (vec3 &) Self-reference.
     */         
    template<typename n_type>
    vec3 & operator*=( n_type n )
    {
      X *= n;
      Y *= n;
      Z *= n;

      return *this;
    } /* End of 'operator*=' function */

    /* Division by number operator.
     * ARGUMENTS: 
     *   - divisor number:
     *       n_type n;
     * RETURNS:
     *   (vec3) result.
     */
    template<typename n_type>
    vec3 operator/( n_type n ) const
    {
      return vec3(X / n, Y / n, Z / n);
    } /* End of 'operator/' function */

    /* Division by number operator.
     * ARGUMENTS:  
     *   - divisor number:
     *       n_type n;
     * RETURNS: 
     *   (vec3 &) Self-reference.
     */         
    template<typename n_type>
    vec3 & operator/=( n_type n )
    {
      X /= n;
      Y /= n;
      Z /= n;

      return *this;
    } /* End of 'operator/=' function */

    /* Dot multiplication operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec3 &val;
     * RETURNS:
     *   (type) result.
     */
    type operator&( const vec3 &val ) const
    {
      return X * val.X + Y * val.Y + Z * val.Z;
    } /* End of 'operator&' function */

    /* Cross multiplication operator.
     * ARGUMENTS:
     *   - multiplier vector:
     *       const vec3 &val;
     * RETURNS:
     *   (vec3) result.
     */
    vec3 operator%( const vec3 &val ) const
    {
      return vec3(Y * val.Z - val.Y * Z,
                        Z * val.X - val.Z * X,
                        X * val.Y - val.X * Y);
    } /* End of 'operator%' function */

    /* Cross multiplication operator.
     * ARGUMENTS:
     *   - multiplier vector:
     *       const vec3 &val;
     * RETURNS: 
     *   (vec3 &) Self-reference.
     */
    vec3 & operator%=( const vec3 &val )
    {
      type tx = Y * val.Z - val.Y * Z, ty = Z * val.X - val.Z * X, tz = X * val.Y - val.X * Y;
      X = tx;
      Y = ty;
      Z = tz;

      return *this;
    } /* End of 'operator%=' function */

    /* Vector length operator  */
    type operator!( void ) const
    {
      return Length();
    } /* End of 'operator!' function */

    /* Index operator.
     * ARGUMENTS:
     *   - index:
     *       int Ind;
     * RETURNS: 
     *   (type) result.
     */
    type operator[]( int Ind ) const
    {
      if (Ind == 0)
        return X;
      if (Ind == 1)
        return Y;
      if (Ind == 2)
        return Z;
    } /* End of 'operator[]' function */

    /* Index operator.
     * ARGUMENTS:
     *   - index:
     *       int Ind;
     * RETURNS: 
     *   (type &) reference to result.
     */
    type & operator[]( int Ind ) 
    {
      if (Ind <= 0)
        return X;
      if (Ind == 1)
        return Y;
      else
        return Z;
    } /* End of 'operator[]' function */

    /* Obtain vector first element pointer operator.
     * ARGUMENTS: None;
     * RETURNS: 
     *   (const type*) pointer to X coordinate.
     */
    operator const type *( void ) 
    {
      return &X;
    } /* End of 'operator const type *' function */

    /* Obtain vector first element pointer operator.
     * ARGUMENTS: None;
     * RETURNS: 
     *   (const type*) pointer to X coordinate.
     */
    operator const type *( void ) const
    {
      return &X;
    } /* End of 'operator const type *' function */
  }; /* End of 'vec3' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec3_h_ */

/* END OF 'mth_vec3.h' FILE */