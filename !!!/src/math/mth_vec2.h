/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_vec2.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               2D space vector.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include "mthdef.h"

/* Math name space */
namespace mth {
  template<typename type>
  /* 2D vector representation type */
  class vec2
  {
  private:
    type X, Y;
  public:
    /* One value constructor.
     * ARGUMENTS: 
     *   - initial value:
     *       type val;
     * RETURNS: None.
     */
    explicit vec2( type val = 0 ) : X(val), Y(val)
    {
    } /* End of one value constructor */

    /* Constructor.
     * ARGUMENTS: 
     *   - initial values:
     *       type xval, yval;
     * RETURNS: None.
     */
    vec2( type xval, type yval ) : X(xval), Y(yval)
    {
    } /* End of constructor */

    /* Self-normalize vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2 &) normalized vector.
     */
    vec2 & Normalize( void )
    {
      vec2 v = vec2(X, Y);
      v /= !v;
      X = v.X;
      Y = v.Y;

      return *this;
    } /* End of 'Normalize' function */

    /* Get normalized vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2) normalized vector.
     */
    vec2 Normalizing( void ) const
    {
      vec2 v = vec2(X, Y);
      return v /= !v;
    } /* End of 'Normalizing' function */

    /* Get vector square length method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (type) vector square length.
     */
    type Length2( void ) const
    {
      return X * X + Y * Y;
    } /* End of 'Length2' function */

    /* Get vector length method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (type) vector length.
     */
    type Length( void ) const
    {
      return sqrt(X * X + Y * Y);
    } /* End of 'Length' function */

    /* Get distance to vector method.
     * ARGUMENTS:
     *   - vector:
     *       const vec2 &val;
     * RETURNS:
     *   (type) distance to vector.
     */
    type Distance( const vec2 &val ) const
    {
      return !vec2(X - val.X, Y - val.Y);
    } /* End of 'Distance' function */

    /* Set null-vector method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (vec2 &) Self-reference.
     */
    vec2 & Zero( void )
    {
      X = 0;
      Y = 0;
      return *this;
    } /* End of 'Zero' function */

      /* Addition operator.
     * ARGUMENTS: 
     *   - term:
     *       const vec2 &val;
     * RETURNS:
     *   (vec2) result.
     */
    vec2 operator+( const vec2 &val ) const
    {
      return vec2(X + val.X, Y + val.Y);
    } /* End of 'operator+' function */

    /* Addition operator.
     * ARGUMENTS: 
     *   - term:
     *       const vec2 &val;
     * RETURNS: 
     *   (vec2 &) Self-reference.
     */
    vec2 & operator+=( const vec2 &val )
    {
      X += val.X;
      Y += val.Y;

      return *this;
    } /* End of 'operator+=' function */

    /* Subtraction operator.
     * ARGUMENTS: 
     *   - subtrahend:
     *       const vec2 &val;
     * RETURNS:
     *   (vec2) result.
     */
    vec2 operator-( const vec2 &val ) const
    {
      return vec2(X - val.X, Y - val.Y);
    } /* End of 'operator- function */

    /* Subtraction operator.
     * ARGUMENTS: 
     *   - subtrahend:
     *       const vec2 &val;
     * RETURNS: 
     *   (vec2 &) Self-reference.
     */
    vec2 & operator-=( const vec2 &val )
    {
      X -= val.X;
      Y -= val.Y;

      return *this;
    } /* End of 'operator-=' function */

    /* Multiplication by coordinates operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec2 &val;
     * RETURNS:
     *   (vec2) result.
     */
    vec2 operator*( const vec2 &val ) const
    {
      return vec2(X * val.X, Y * val.Y);
    } /* End of 'operator*' function */

    /* Multiplication by coordinates operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec2 &val;
     * RETURNS: 
     *   (vec2 &) Self-reference.
     */
    vec2 & operator*=( const vec2 &val )
    {
      X *= val.X;
      Y *= val.Y;

      return *this;
    } /* End of 'operator*=' function */

    /* Multiplication by number operator.
     * ARGUMENTS: 
     *   - multiplier number:
     *       n_type n;
     * RETURNS:
     *   (vec2) result.
     */
    template<typename n_type>
    vec2 operator*( n_type n ) const
    {
      return vec2(X * n, Y * n);
    } /* End of 'operator*' function */

    /* Multiplication by number operator.
     * ARGUMENTS:  
     *   - multiplier number:
     *       n_type n;
     * RETURNS: 
     *   (vec2 &) Self-reference.
     */         
    template<typename n_type>
    vec2 & operator*=( n_type n )
    {
      X *= n;
      Y *= n;

      return *this;
    } /* End of 'operator*=' function */

    /* Division by number operator.
     * ARGUMENTS: 
     *   - divisor number:
     *       n_type n;
     * RETURNS:
     *   (vec2) result.
     */
    template<typename n_type>
    vec2 operator/( n_type n ) const
    {
      return vec2(X / n, Y / n);
    } /* End of 'operator/' function */

    /* Division by number operator.
     * ARGUMENTS:  
     *   - divisor number:
     *       n_type n;
     * RETURNS: 
     *   (vec2 &) Self-reference.
     */         
    template<typename n_type>
    vec2 & operator/=( n_type n )
    {
      X /= n;
      Y /= n;

      return *this;
    } /* End of 'operator/=' function */

    /* Dot multiplication operator.
     * ARGUMENTS: 
     *   - multiplier vector:
     *       const vec2 &val;
     * RETURNS:
     *   (vec2) result.
     */
    type operator&( const vec2 &val ) const
    {
      return X * val.X + Y * val.Y;
    } /* End of 'operator&' function */

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
      else
        return X;
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
      if (Ind == 0)
        return X;
      if (Ind == 1)
        return Y;
      return X;
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
  }; /* End of 'vec2' class */
} /* end of 'mth' namespace */

#endif /* __mth_vec2_h_ */

/* END OF 'mth_vec2.h' FILE */