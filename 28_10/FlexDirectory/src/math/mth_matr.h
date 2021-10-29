/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_matr.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               Transform 3D space mitrix.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mthdef.h"
#include "mth_vec3.h"

/* Math name space */
namespace mth {
  template<class type>
    class matr_data
    {
    public:
      type A[4][4]; /* Matrix data */

      /* Matrix constructor. 
       * ARGUMENTS: None.
       */
      matr_data( void )
      {
      } /* End of 'matr' function */
    };

  /* Matrix representation type */
  template<typename type>
  class matr : public matr_data<type>
  {
  private:
    mutable type InvA[4][4];
    mutable bool IsInverseEvaluated;

    /* Evaluate determinant of matrix 3x3 method.
     * ARGUMENTS:
     *   - matix:
     *       type A11, A12, A13,
     *            A21, A22, A23,
     *            A31, A32, A33;
     * RETURNS:
     *   (type) Determinant.
     */
    type Determinant3x3( type A11, type A12, type A13,
                         type A21, type A22, type A23,
                         type A31, type A32, type A33 )
    {
      return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
             A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
    } /* End of 'Determinant3x3' function */

    /* Evaluate determinant of this matrix method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (type) Determinant.
     */
    type Determinant( void )
    {
      return
        +matr_data<type>::A[0][0] * Determinant3x3(matr_data<type>::A[1][1], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                                  matr_data<type>::A[2][1], matr_data<type>::A[2][2], matr_data<type>::A[2][3],
                                  matr_data<type>::A[3][1], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) +
        -matr_data<type>::A[0][1] * Determinant3x3(matr_data<type>::A[1][0], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                                  matr_data<type>::A[2][0], matr_data<type>::A[2][2], matr_data<type>::A[2][3],
                                  matr_data<type>::A[3][0], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) +
        +matr_data<type>::A[0][2] * Determinant3x3(matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][3],
                                  matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][3],
                                  matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][3]) +
        -matr_data<type>::A[0][3] * Determinant3x3(matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][2],
                                  matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][2],
                                  matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][2]);
    } /* End of 'Determinant' function */

    /* Evaluate inverse matrix method.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    void EvaluateInverseMatrix( void )
    {
      if (IsInverseEvaluated)
        return;
      IsInverseEvaluated = true;

      type det = !*this;
 
      if (det == 0)
      {
        InvA[0][0] = 1;
        InvA[0][1] = 0;
        InvA[0][2] = 0;
        InvA[0][3] = 0;
        InvA[1][0] = 0;
        InvA[1][1] = 1;
        InvA[1][2] = 0;
        InvA[1][3] = 0;
        InvA[2][0] = 0;
        InvA[2][1] = 0;
        InvA[2][2] = 1;
        InvA[2][3] = 0;
        InvA[3][0] = 0;
        InvA[3][1] = 0;
        InvA[3][2] = 0;
        InvA[3][3] = 1;
      }

      InvA[0][0] =
        +Determinant3x3(matr_data<type>::A[1][1], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                        matr_data<type>::A[2][1], matr_data<type>::A[2][2], matr_data<type>::A[2][3],
                        matr_data<type>::A[3][1], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) / det;
      InvA[1][0] =
        -Determinant3x3(matr_data<type>::A[1][0], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][2], matr_data<type>::A[2][3],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) / det;
      InvA[2][0] =
        +Determinant3x3(matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][3],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][3],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][3]) / det;
      InvA[3][0] =
        -Determinant3x3(matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][2],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][2],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][2]) / det;
 
      InvA[0][1] =
        -Determinant3x3(matr_data<type>::A[0][1], matr_data<type>::A[0][2], matr_data<type>::A[0][3],
                        matr_data<type>::A[2][1], matr_data<type>::A[2][2], matr_data<type>::A[2][3],
                        matr_data<type>::A[3][1], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) / det;
      InvA[1][1] =
        +Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][2], matr_data<type>::A[0][3],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][2], matr_data<type>::A[2][3],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) / det;
      InvA[2][1] =
        -Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][1], matr_data<type>::A[0][3],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][3],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][3]) / det;
      InvA[3][1] =
        +Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][1], matr_data<type>::A[0][2],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][2],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][2]) / det;
 
 
      InvA[0][2] =
        +Determinant3x3(matr_data<type>::A[0][1], matr_data<type>::A[0][2], matr_data<type>::A[0][3],
                        matr_data<type>::A[1][1], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                        matr_data<type>::A[3][1], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) / det;
      InvA[1][2] =
        -Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][2], matr_data<type>::A[0][3],
                        matr_data<type>::A[1][0], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][2], matr_data<type>::A[3][3]) / det;
      InvA[2][2] =
        +Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][1], matr_data<type>::A[0][3],
                        matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][3],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][3]) / det;
      InvA[3][2] =
        -Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][1], matr_data<type>::A[0][2],
                        matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][2],
                        matr_data<type>::A[3][0], matr_data<type>::A[3][1], matr_data<type>::A[3][2]) / det;
 
      InvA[0][3] =
        -Determinant3x3(matr_data<type>::A[0][1], matr_data<type>::A[0][2], matr_data<type>::A[0][3],
                        matr_data<type>::A[1][1], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                        matr_data<type>::A[2][1], matr_data<type>::A[2][2], matr_data<type>::A[2][3]) / det;
      InvA[1][3] =
        +Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][2], matr_data<type>::A[0][3],
                        matr_data<type>::A[1][0], matr_data<type>::A[1][2], matr_data<type>::A[1][3],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][2], matr_data<type>::A[2][3]) / det;
      InvA[2][3] =
        -Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][1], matr_data<type>::A[0][3],
                        matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][3],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][3]) / det;
      InvA[3][3] =
        +Determinant3x3(matr_data<type>::A[0][0], matr_data<type>::A[0][1], matr_data<type>::A[0][2],
                        matr_data<type>::A[1][0], matr_data<type>::A[1][1], matr_data<type>::A[1][2],
                        matr_data<type>::A[2][0], matr_data<type>::A[2][1], matr_data<type>::A[2][2]) / det;
    } /* End of 'EvaluateInverseMatrix' function */

  public:
    /* Default constructor */
    matr( VOID ) :
      IsInverseEvaluated(false)
    {
      matr_data<type>::A[0][0] = 1;
      matr_data<type>::A[0][1] = 0;
      matr_data<type>::A[0][2] = 0;
      matr_data<type>::A[0][3] = 0;
      matr_data<type>::A[1][0] = 0;
      matr_data<type>::A[1][1] = 1;
      matr_data<type>::A[1][2] = 0;
      matr_data<type>::A[1][3] = 0;
      matr_data<type>::A[2][0] = 0;
      matr_data<type>::A[2][1] = 0;
      matr_data<type>::A[2][2] = 1;
      matr_data<type>::A[2][3] = 0;
      matr_data<type>::A[3][0] = 0;
      matr_data<type>::A[3][1] = 0;
      matr_data<type>::A[3][2] = 0;
      matr_data<type>::A[3][3] = 1;

      InvA[0][0] = 1;
      InvA[0][1] = 0;
      InvA[0][2] = 0;
      InvA[0][3] = 0;
      InvA[1][0] = 0;
      InvA[1][1] = 1;
      InvA[1][2] = 0;
      InvA[1][3] = 0;
      InvA[2][0] = 0;
      InvA[2][1] = 0;
      InvA[2][2] = 1;
      InvA[2][3] = 0;
      InvA[3][0] = 0;
      InvA[3][1] = 0;
      InvA[3][2] = 0;
      InvA[3][3] = 1;
    } /* End of constructor by elements */

    /* Constructor by elements.
     * ARGUMENTS:
     *   - initial data:
     *       typy fld00, fld01, fld02, fld03,
     *            fld10, fld11, fld12, fld13,
     *            fld20, fld21, fld22, fld23,
     *            fld30, fld31, fld32, fld33;
     * RETURNS: None.
     */
    matr( type fld00, type fld01, type fld02, type fld03,
          type fld10, type fld11, type fld12, type fld13,
          type fld20, type fld21, type fld22, type fld23,
          type fld30, type fld31, type fld32, type fld33 ) :
      IsInverseEvaluated(false)
    {
      matr_data<type>::A[0][0] = fld00;
      matr_data<type>::A[0][1] = fld01;
      matr_data<type>::A[0][2] = fld02;
      matr_data<type>::A[0][3] = fld03;
      matr_data<type>::A[1][0] = fld10;
      matr_data<type>::A[1][1] = fld11;
      matr_data<type>::A[1][2] = fld12;
      matr_data<type>::A[1][3] = fld13;
      matr_data<type>::A[2][0] = fld20;
      matr_data<type>::A[2][1] = fld21;
      matr_data<type>::A[2][2] = fld22;
      matr_data<type>::A[2][3] = fld23;
      matr_data<type>::A[3][0] = fld30;
      matr_data<type>::A[3][1] = fld31;
      matr_data<type>::A[3][2] = fld32;
      matr_data<type>::A[3][3] = fld33;

      InvA[0][0] = 1;
      InvA[0][1] = 0;
      InvA[0][2] = 0;
      InvA[0][3] = 0;
      InvA[1][0] = 0;
      InvA[1][1] = 1;
      InvA[1][2] = 0;
      InvA[1][3] = 0;
      InvA[2][0] = 0;
      InvA[2][1] = 0;
      InvA[2][2] = 1;
      InvA[2][3] = 0;
      InvA[3][0] = 0;
      InvA[3][1] = 0;
      InvA[3][2] = 0;
      InvA[3][3] = 1;
    } /* End of constructor by elements */

    /* Constructor by array.
     * ARGUMENTS:
     *   - initial array:
     *       type M[4][4];
     * RETURNS: None.
     */
    matr( type M[4][4] ) :
      IsInverseEvaluated(false)
    {
      memcpy(matr_data<type>::A, M, sizeof(M));
    } /* End of constructor by array */

    /* Identity matrix method.
     * ARGUMENTS: None.
     * RETURNS: 
     *   (matr) Identity matrix.
     */
    static matr Identity( void )
    {
      return matr(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
    } /* End of 'Identity' function */

    /* Translate matrix method.
     * ARGUMENTS:
     *   - translate vector:
     *       const vec3<type> &T;
     * RETURNS:  
     *   (matr) Translate matrix.
     */
    static matr Translate( const vec3<type> &T )
    {
      return matr(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  T.X, T.Y, T.Z, 1);
    } /* End of 'Translate' function */

    /* Scale matrix method.
     * ARGUMENTS:
     *   - scale vector:
     *       const vec3<type> &S;
     * RETURNS:  
     *   (matr) Scale matrix.
     */
    static matr Scale( const vec3<type> &S )
    {
      return matr(S.X, 0, 0, 0,
                  0, S.Y, 0, 0,
                  0, 0, S.Z, 0,
                  0, 0, 0, 1);
    } /* End of 'Scale' function */

    /* Transpose matrix method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr &) Transpose matrix.
     */
    static matr & Transpose( void )
    {
      matr m = matr_data<type>::A;

      matr_data<type>::A[0][1] = m.matr_data<type>::A[1][0];
      matr_data<type>::A[0][2] = m.matr_data<type>::A[2][0];
      matr_data<type>::A[0][3] = m.matr_data<type>::A[3][0];
      matr_data<type>::A[1][0] = m.matr_data<type>::A[0][1];
      matr_data<type>::A[1][2] = m.matr_data<type>::A[2][1];
      matr_data<type>::A[1][3] = m.matr_data<type>::A[3][1];
      matr_data<type>::A[2][0] = m.matr_data<type>::A[0][2];
      matr_data<type>::A[2][1] = m.matr_data<type>::A[1][2];
      matr_data<type>::A[2][3] = m.matr_data<type>::A[3][2];
      matr_data<type>::A[3][0] = m.matr_data<type>::A[0][3];
      matr_data<type>::A[3][1] = m.matr_data<type>::A[1][3];
      matr_data<type>::A[3][2] = m.matr_data<type>::A[2][3];
      return *this;
    } /* End of 'Transpose' function */

    /* Get transposed matrix method.
     * ARGUMENTS: None.
     * RETURNS:
     *   (matr) Transposed matrix.
     */
    static matr Transposed( void )
    {
      return matr(matr_data<type>::A[0][0], matr_data<type>::A[1][0], matr_data<type>::A[2][0], matr_data<type>::A[3][0],
                  matr_data<type>::A[0][1], matr_data<type>::A[1][1], matr_data<type>::A[2][1], matr_data<type>::A[3][1],
                  matr_data<type>::A[0][2], matr_data<type>::A[1][2], matr_data<type>::A[2][2], matr_data<type>::A[3][2],
                  matr_data<type>::A[0][3], matr_data<type>::A[1][3], matr_data<type>::A[2][3], matr_data<type>::A[3][3]);
    } /* End of 'Transposed' function */

    /* X rotation matrix method.
     * ARGUMENTS:
     *   - angle int degrees:
     *       type D;
     * RETURNS
     *   (matr) Rotated matrix.
     */
    static matr RotateX( type D )
    {
      type R = D2R(D);
      type s = sin(R), c = cos(R);
      return matr(1, 0, 0, 0,
                  0, c, s, 0,
                  0, -s, c, 0,
                  0, 0, 0, 1);
    } /* End of 'RotateX' function */

    /* Y rotation matrix method.
     * ARGUMENTS:
     *   - angle int degrees:
     *       type D;
     * RETURNS:
     *   (matr) Rotated matrix.
     */
    static matr RotateY( type D )
    {
      type R = D2R(D);
      type s = sin(R), c = cos(R);
      return matr(c, 0, -s, 0,
                  0, 1, 0, 0,
                  s, 0, c, 0,
                  0, 0, 0, 1);
    } /* End of 'RotateY' function */

    /* Z rotation matrix method.
     * ARGUMENTS:
     *   - angle int degrees:
     *       type D;
     * RETURNS:
     *   (matr) Rotated matrix.
     */
    static matr RotateZ( type D )
    {
      type R = D2R(D);
      type s = sin(R), c = cos(R);
      return matr(c, s, 0, 0,
                  -s, c, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
    } /* End of 'RotateZ' function */

    /* Rotation matrix by axis method.
     * ARGUMENTS:
     *   - axis vector:
     *       const vec3<type> &a;
     *   - angle int degrees:
     *       type D;
     * RETURNS:
     *   (matr) Rotated matrix.
     */
    static matr Rotate( const vec3<type> &a, type D )
    {
      type R = D2R(D);
      type s = sin(R), c = cos(R);
      return matr(c + a.X * a.X * (1 - c),       a.X * a.Y * (1 - c) + a.Z * s, a.X * a.Z * (1 - c) - a.Y * s, 0,
                  a.Y * a.X * (1 - c) - a.Z * s, c + a.Y * a.Y * (1 - c),       a.Y * a.Z * (1 - c) + a.X * s, 0,
                  a.Z * a.X * (1 - c) + a.Y * s, a.Z * a.Y * (1 - c) - a.X * s, c + a.Z * a.Z * (1 - c),       0,
                  0,                             0,                             0,                             1);
    } /* End of 'Rotate' function */

    /* Trasform point method.
     * ARGUMENTS:
     *   - point to transform:
     *       const vec3<type> &p;
     * RETURNS: 
     *   (vec3<type>) Result vector.
     */
    vec3<type> TransformPoint( const vec3<type> &p ) const
    {
      return vec3<type>(p.X * matr_data<type>::A[0][0] + p.Y * matr_data<type>::A[1][0] + p.Z * matr_data<type>::A[2][0] + matr_data<type>::A[3][0],
                        p.X * matr_data<type>::A[0][1] + p.Y * matr_data<type>::A[1][1] + p.Z * matr_data<type>::A[2][1] + matr_data<type>::A[3][1],
                        p.X * matr_data<type>::A[0][2] + p.Y * matr_data<type>::A[1][2] + p.Z * matr_data<type>::A[2][2] + matr_data<type>::A[3][2]);
    } /* End of 'TransformPoint' function */

    /* Trasform vector method.
     * ARGUMENTS:
     *   - vector to transform:
     *       const vec3<type> &p;
     * RETURNS: 
     *   (vec3<type>) Result vector.
     */
    vec3<type> TransformVector( const vec3<type> &p ) const
    {
      return vec3<type>(p.X * matr_data<type>::A[0][0] + p.Y * matr_data<type>::A[1][0] + p.Z * matr_data<type>::A[2][0],
                        p.X * matr_data<type>::A[0][1] + p.Y * matr_data<type>::A[1][1] + p.Z * matr_data<type>::A[2][1],
                        p.X * matr_data<type>::A[0][2] + p.Y * matr_data<type>::A[1][2] + p.Z * matr_data<type>::A[2][2]);

    } /* End of 'TransformVector' function */

    /* Trasform vector method.
     * ARGUMENTS:
     *   - point to transform:
     *       const vec3<type> &p;
     * RETURNS: 
     *   (vec3<type>) Result vector.
     */
    vec3<type> Transform4x4( const vec3<type> &p ) const
    {
      type w = p.X * matr_data<type>::A[0][3] + p.Y * matr_data<type>::A[1][3] + p.Z * matr_data<type>::A[2][3] + matr_data<type>::A[3][3];

      return vec3<type>((p.X * matr_data<type>::A[0][0] + p.Y * matr_data<type>::A[1][0] + p.Z * matr_data<type>::A[2][0] + matr_data<type>::A[3][0]) / w,
                        (p.X * matr_data<type>::A[0][1] + p.Y * matr_data<type>::A[1][1] + p.Z * matr_data<type>::A[2][1] + matr_data<type>::A[3][1]) / w,
                        (p.X * matr_data<type>::A[0][2] + p.Y * matr_data<type>::A[1][2] + p.Z * matr_data<type>::A[2][2] + matr_data<type>::A[3][2]) / w);

    } /* End of 'Transform4x4' function */

    /* Trasform normal method.
     * ARGUMENTS:
     *   - normal to transform:
     *       const vec3<type> &N;
     * RETURNS: 
     *   (vec3<type>) Result vector.
     */
    vec3<type> TransformNormal( const vec3<type> &N )
    {
      EvaluateInverseMatrix();
      return vec3<type>(N.X * InvA[0][0] + N.Y * InvA[0][1] + N.Z * InvA[0][2],
                        N.X * InvA[1][0] + N.Y * InvA[1][1] + N.Z * InvA[1][2],
                        N.X * InvA[2][0] + N.Y * InvA[2][1] + N.Z * InvA[2][2]);
    } /* End of 'TransformNormal' function */

    /* Multiplications of matrixes operator.
     * ARGUMENTS:
     *   - multiplier matrix:
     *       const matr &m;
     * RETURNS: 
     *   (matr) Result matrix.
     */
    matr operator*( const matr &m ) const
    {
      matr r;
      int i, j, k;

      for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
          for (k = 0, r.matr_data<type>::A[i][j] = 0; k < 4; k++)
            r.matr_data<type>::A[i][j] += 
            matr_data<type>::A[i][k] * 
            m.matr_data<type>::A[k][j];
      return r;
    } /* End of 'operator*' function */

    /* Multiplications of matrixes operator.
     * ARGUMENTS:
     *   - multiplier matrix:
     *       const matr &m;
     * RETURNS: 
     *   (matr &) reference-self to result matrix.
     */
    matr & operator*=( const matr &m )
    {
      matr r; 
      int i, j, k;

      for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
          for (k = 0, r.matr_data<type>::A[i][j] = 0; k < 4; k++)
            r.matr_data<type>::A[i][j] += matr_data<type>::A[i][k] * m.matr_data<type>::A[k][j];
      *this = r;
      return *this;
    } /* End of 'operator*=' function */

    /* Evaluate determinant of this matrix operator.
     * ARGUMENTS: None.
     * RETURNS:
     *   (type) Determinant.
     */
    type operator!( void )
    {
      return Determinant();
    } /* End of 'operator!' function */

    /* Obtain matrix first element pointer operator.
     * ARGUMENTS: None;
     * RETURNS: 
     *   (const type*) pointer to [0][0] element.
     */
    operator const type *( void )
    {
      return matr_data<type>::A[0];
    } /* End of 'operator const type *' function */

    /* Obtain matrix first element pointer operator.
     * ARGUMENTS: None;
     * RETURNS: 
     *   (const type*) pointer to [0][0] element.
     */
    operator const type *( void ) const
    {
      return matr_data<type>::A[0];
    } /* End of 'operator const type *' function */

    /* Camera matrix */
    /* Matrix look at viwer setup function.
     * ARGUMENTS:
     *   - Positoin:
     *      vec3<type> &Loc;
     *   - Where we looking for:
     *      vec3<type> &At;
     *   - Direction to up:
     *      vec3<type> &Up1;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr View( vec3<type> &Loc, vec3<type> &At, vec3<type> &Up )
    {
      vec3<type> D = (At - Loc).Normalized(),
                 R = (D % Up).Normalized(),
                 U = R % D;

      return matr(R.X, U.X, -D.X, 0,
                  R.Y, U.Y, -D.Y, 0,
                  R.Z, U.Z, -D.Z, 0,
                  -(Loc & R), -(Loc & U), Loc & D, 1);
    } /* End of 'View' function */

    /* Normalized device coordinates function.
     * ARGUMENTS:
     *   - Screen factors:
     *      Left and Right:
     *        type L, R;
     *      Bottom and Top:
     *        type B, T;
     *      Near and Far:
     *        type N, F;
     * RETURNS:
     *   (matr) result matrix.
     */
    static matr Frustum( type L, type R, type B, type T, type N, type F )
    {
      return matr(2 * N / (R - L),   0,                 0,                    0,
                  0,                 2 * N / (T - B),   0,                    0,
                  (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N),   -1,
                  0,                 0,                 -2 * N * F / (F - N), 0);
    } /* End of 'Frustum' function */

    /* Ortho projection matrix function.
     * ARGUMENTS:
     *   - Screen factors:
     *      Left and Right:
     *        type L, R;
     *      Bottom and Top:
     *        type B, T;
     *      Near and Far:
     *        type N, F;
     * RETURNS:
     *   (matr) viewer matrix.
     */
    static matr Ortho( type L, type R, type B, type T, type N, type F )
    {
      return matr(2 / (R - L),        0,                  0,                  0,
                  0,                  2 / (T - B),        0,                  0,
                  0,                  0,                  -2 / (F - N),       0,
                  -(R + L) / (R - L), -(T + B) / (T - B), -(N + F) / (F - N), 1);
    } /* End of 'Ortho' function */
  }; /* End of 'matr' class */
} /* end of 'mth' namespace */

#endif /* __mth_matr_h_ */

/* END OF 'mth_matr.h' FILE */