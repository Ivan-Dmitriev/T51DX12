/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_vec4.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               4-element vector.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include "mthdef.h"

/* Math name space */
namespace mth {
  template<typename type>
  /* 3D vector representation type */
  class vec4
  {
  private:
    /* Delegate acces rights to other class */
    friend class matr<type>;
    type X, Y, Z, W;
  public:
    /* One value constructor.
     * ARGUMENTS: 
     *   - initial value:
     *       type val;
     * RETURNS: None.
     */
    explicit vec4( type val = 0 ) : X(val), Y(val), Z(val), W(1)
    {
    } /* End of one value constructor */

    /* Constructor.
     * ARGUMENTS: 
     *   - initial values:
     *       type xval, yval, zval;
     * RETURNS: None.
     */
    vec4( type xval, type yval, type zval, type wval ) : X(xval), Y(yval), Z(zval), W(wval)
    {
    } /* End of constructor */

    /* Constructor.
     * ARGUMENTS: 
     *   - initial values:
     *       vec3<type> vec, type num;
     * RETURNS: None.
     */
    vec4( vec3<type> vec, type num = 0 ) : X(vec[0]), Y(vec[1]), Z(vec[2]), W(num)
    {
    } /* End of constructor */

    /* Set null-vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec4 &) Self-reference.
     */
    vec4 & Zero( void )
    {
      X = 0;
      Y = 0;
      Z = 0;
      W = 1;
      return *this;
    } /* End of 'Zero' function */

    /* Addition operator.
     * ARGUMENTS:
     *   - term:
     *       const vec4 &val;
     * RETURNS:
     *   (vec4) result.
     */
    vec4 operator+( const vec4 &val ) const
    {
      return vec4(X + val.X, Y + val.Y, Z + val.Z, W + val.W);
    } /* End of 'operator+' function */

    /* Addition operator.
     * ARGUMENTS: 
     *   - term:
     *       const vec4 &val;
     * RETURNS: 
     *   (vec4 &) Self-reference.
     */
    vec4 & operator+=( const vec4 &val )
    {
      X += val.X;
      Y += val.Y;
      Z += val.Z;
      W += val.W;

      return *this;
    } /* End of 'operator+=' function */

    /* Subtraction operator.
     * ARGUMENTS: 
     *   - subtrahend:
     *       const vec4 &val;
     * RETURNS:
     *   (vec4) result.
     */
    vec4 operator-( const vec4 &val ) const
    {
      return vec4(X - val.X, Y - val.Y, Z - val.Z, W - val.W);
    } /* End of 'operator-' function */

    /* Subtraction operator.
     * ARGUMENTS: 
     *   - subtrahend:
     *       const vec4 &val;
     * RETURNS: 
     *   (vec4 &) Self-reference.
     */
    vec4 & operator-=( const vec4 &val )
    {
      X -= val.X;
      Y -= val.Y;
      Z -= val.Z;
      W -= val.W;

      return *this;
    } /* End of 'operator-=' function */

    /* Multiplication by coordinates operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec4 &val;
     * RETURNS:
     *   (vec4) result.
     */
    vec4 operator*( const vec4 &val ) const
    {
      return vec4(X * val.X, Y * val.Y, Z * val.Z, W * val.W);
    } /* End of 'operator*' function */

    /* Multiplication by coordinates operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec4 &val;
     * RETURNS: 
     *   (vec4 &) Self-reference.
     */
    vec4 & operator*=( const vec4 &val )
    {
      X *= val.X;
      Y *= val.Y;
      Z *= val.Z;
      W *= val.W;

      return *this;
    } /* End of 'operator*=' function */

    /* Multiplication by number operator.
     * ARGUMENTS: 
     *   - multiplier number:
     *       n_type n;
     * RETURNS:
     *   (vec4) result.
     */
    template<typename n_type>
    vec4 operator*( n_type n ) const
    {
      return vec4(X * n, Y * n, Z * n, W * n);
    } /* End of 'operator* function */

    /* Multiplication by number operator.
     * ARGUMENTS:
     *   - multiplier number:
     *       n_type n;
     * RETURNS: 
     *   (vec4 &) Self-reference.
     */         
    template<typename n_type>
    vec4 & operator*=( n_type n )
    {
      X *= n;
      Y *= n;
      Z *= n;
      W *= n;

      return *this;
    } /* End of 'operator*=' function */

    /* Division by number operator.
     * ARGUMENTS: 
     *   - divisor number:
     *       n_type n;
     * RETURNS:
     *   (vec4) result.
     */
    template<typename n_type>
    vec4 operator/( n_type n ) const
    {
      return vec4(X / n, Y / n, Z / n, W / n);
    } /* End of 'operator/' function */

    /* Division by number operator.
     * ARGUMENTS:  
     *   - divisor number:
     *       n_type n;
     * RETURNS: 
     *   (vec4 &) Self-reference.
     */         
    template<typename n_type>
    vec4 & operator/=( n_type n )
    {
      X /= n;
      Y /= n;
      Z /= n;
      W /= n;

      return *this;
    } /* End of 'operator/=' function */

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
      if (Ind == 3)
        return W;
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
      if (Ind == 2)
        return Z;
      else
        return W;
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
  }; /* End of 'vec4' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec4_h_ */

/* END OF 'mth_vec4.h' FILE */