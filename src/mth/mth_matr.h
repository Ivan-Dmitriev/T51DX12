/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_matr.h
 * PURPOSE     : Animation project.
 *               Mathematics library.
 *               Matrices handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * NOTE        : Module namespace 'mth'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mth_vec3.h"
//#include "mth_camera.h"

#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}

namespace math
{
  //class camera;
  template<typename Type1>
    class matr_data
    {
    public:
      Type1 M[4][4];

    };
  template<typename Type1>
    class matr
    {
      //public:
      //template<typename Type1> friend class camera;
      //friend camera & camera::SetLocAtUp( const vec3<Type1> &L, const vec3<Type1> &A, const vec3<Type1> &U = vec3<Type1>(0, 1, 0) );
    private:
      Type1 M[4][4];
      mutable Type1 InvM[4][4];        /* Inverse matrix */
      mutable BOOL IsInverseEvaluated; /* Inverse flag */

      mutable Type1 TransposeM[4][4]; /* Transpose matrix */
      mutable BOOL IsTransEn;         /* Transpose flag */

      /* Find determinant of 3x3 matrix function.
       * ARGUMENTS: None.
       *   Input matrix value:
       *     - Type1 A11, A12, A13,
       *             A21, A22, A23,
       *             A31, A32, A33,
       * RETURNS: (Type1) result value.
       */
      Type1 Determ3x3( Type1 A11, Type1 A12, Type1 A13,
                           Type1 A21, Type1 A22, Type1 A23,
                           Type1 A31, Type1 A32, Type1 A33 ) const
      {
        return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
               A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
      } /* End of 'Determ3x3' function */

      /* Find determinant of 4x4 matrix function.
       * ARGUMENTS: None.
       * RETURNS: (Type1) result value.
       */
      Type1 Determ( VOID ) const
      {
        return
          +M[0][0] * Determ3x3(M[1][1], M[1][2], M[1][3],
                                   M[2][1], M[2][2], M[2][3],
                                   M[3][1], M[3][2], M[3][3]) +
          -M[0][1] * Determ3x3(M[1][0], M[1][2], M[1][3],
                                   M[2][0], M[2][2], M[2][3],
                                   M[3][0], M[3][2], M[3][3]) +
          +M[0][2] * Determ3x3(M[1][0], M[1][1], M[1][3],
                                   M[2][0], M[2][1], M[2][3],
                                   M[3][0], M[3][1], M[3][3]) +
          -M[0][3] * Determ3x3(M[1][0], M[1][1], M[1][2],
                                   M[2][0], M[2][1], M[2][2],
                                   M[3][0], M[3][1], M[3][2]);
      } /* End of 'Determ' function */
    public:
      /* Constructor of matr class function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      matr( VOID ) : IsInverseEvaluated(FALSE), IsTransEn(FALSE)
      {
      } /* End of 'Constructor' function */

      /* Constructor of matr class function.
       * ARGUMENTS: 
       *   - Input array of values:
       *       R[4][4];
       * RETURNS: None.
       */
      matr( Type1 R[4][4] ) : IsInverseEvaluated(FALSE), IsTransEn(FALSE)
      {
        memcpy(M, R, sizeof(M));
      } /* End of 'Constructor' function */

      /* Constructor of matr class function.
       * ARGUMENTS: 
       *   Input matrix value:
       *     - Type1 a00, a01, a02, a03,
       *             a10, a11, a12, a13,
       *             a20, a21, a22, a23,
       *             a30, a31, a32, a33; 
       * RETURNS: (matr) result matrix.
       */
      matr( Type1 a00, Type1 a01, Type1 a02, Type1 a03,
            Type1 a10, Type1 a11, Type1 a12, Type1 a13,
            Type1 a20, Type1 a21, Type1 a22, Type1 a23,
            Type1 a30, Type1 a31, Type1 a32, Type1 a33) 
      {
        IsInverseEvaluated = FALSE;
        IsTransEn = FALSE;
        M[0][0] = a00;
        M[0][1] = a01;
        M[0][2] = a02;
        M[0][3] = a03;

        M[1][0] = a10;
        M[1][1] = a11;
        M[1][2] = a12;
        M[1][3] = a13;

        M[2][0] = a20;
        M[2][1] = a21;
        M[2][2] = a22;
        M[2][3] = a23;

        M[3][0] = a30;
        M[3][1] = a31;
        M[3][2] = a32;
        M[3][3] = a33;
        //InvM[0][0] = 0;
      } /* End of 'constructor' function */

      /* Constructor of matr class function.
       * ARGUMENTS: 
       *   Input matrix value:
       *     - Type1 AllEqual;
       * RETURNS: (matr) result matrix.
       */
      /*
      matr( Type1 AllEqual ) : IsInverseEvaluated(FALSE)
      {
        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            M[i][j] = AllEqual;
        InvM[0][0] = 0;

      }
      */
      /* End of 'constructor' function */

      /* Get identity matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr &) link on result matrix.
       */
      static matr Identity( VOID )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'Identity' function */
      /* Get identity matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr &) link on result matrix.
       */
      matr & toIdentity( VOID )
      {
        matr M(1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1);
        return M;
      } /* End of 'Identity' function */

      /* Find determinator of matrix operator.
       * ARGUMENTS: None.
       * RETURNS: (Type) Result value.
       */
      Type1 operator!( VOID )
      {
         return Determ();
      } /* End of 'operator!' function */

      /* Inverse matrix function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Inverse( VOID )  
      {
        if (IsInverseEvaluated)
          return;
        IsInverseEvaluated = TRUE;

        DBL det = !*this;

        if (det == 0)
        {
          memcpy(InvM, Identity().M, sizeof(Type1) * 16);
        }
        /* build adjoint matrix */
        InvM[0][0] =
          +Determ3x3(M[1][1], M[1][2], M[1][3],
                         M[2][1], M[2][2], M[2][3],
                         M[3][1], M[3][2], M[3][3]) / det;
        InvM[1][0] =
          -Determ3x3(M[1][0], M[1][2], M[1][3],
                         M[2][0], M[2][2], M[2][3],
                         M[3][0], M[3][2], M[3][3]) / det;
        InvM[2][0] =
          +Determ3x3(M[1][0], M[1][1], M[1][3],
                         M[2][0], M[2][1], M[2][3],
                         M[3][0], M[3][1], M[3][3]) / det;
        InvM[3][0] =
          -Determ3x3(M[1][0], M[1][1], M[1][2],
                         M[2][0], M[2][1], M[2][2],
                         M[3][0], M[3][1], M[3][2]) / det;
        InvM[0][1] =
          -Determ3x3(M[0][1], M[0][2], M[0][3],
                         M[2][1], M[2][2], M[2][3],
                         M[3][1], M[3][2], M[3][3]) / det;
        InvM[1][1] =
          +Determ3x3(M[0][0], M[0][2], M[0][3],
                         M[2][0], M[2][2], M[2][3],
                         M[3][0], M[3][2], M[3][3]) / det;
        InvM[2][1] =
          -Determ3x3(M[0][0], M[0][1], M[0][3],
                         M[2][0], M[2][1], M[2][3],
                         M[3][0], M[3][1], M[3][3]) / det;
        InvM[3][1] =
          +Determ3x3(M[0][0], M[0][1], M[0][2],
                         M[2][0], M[2][1], M[2][2],
                         M[3][0], M[3][1], M[3][2]) / det;
        InvM[0][2] =
          +Determ3x3(M[0][1], M[0][2], M[0][3],
                         M[1][1], M[1][2], M[1][3],
                         M[3][1], M[3][2], M[3][3]) / det;
        InvM[1][2] =
          -Determ3x3(M[0][0], M[0][2], M[0][3],
                         M[1][0], M[1][2], M[1][3],
                         M[3][0], M[3][2], M[3][3]) / det;
        InvM[2][2] =
          +Determ3x3(M[0][0], M[0][1], M[0][3],
                         M[1][0], M[1][1], M[1][3],
                         M[3][0], M[3][1], M[3][3]) / det;
        InvM[3][2] =
          -Determ3x3(M[0][0], M[0][1], M[0][2],
                         M[1][0], M[1][1], M[1][2],
                         M[3][0], M[3][1], M[3][2]) / det;

        InvM[0][3] =
          -Determ3x3(M[0][1], M[0][2], M[0][3],
                         M[1][1], M[1][2], M[1][3],
                         M[2][1], M[2][2], M[2][3]) / det;
        InvM[1][3] =
          +Determ3x3(M[0][0], M[0][2], M[0][3],
                         M[1][0], M[1][2], M[1][3],
                         M[2][0], M[2][2], M[2][3]) / det;
        InvM[2][3] =
          -Determ3x3(M[0][0], M[0][1], M[0][3],
                         M[1][0], M[1][1], M[1][3],
                         M[2][0], M[2][1], M[2][3]) / det;
        InvM[3][3] =
          +Determ3x3(M[0][0], M[0][1], M[0][2],
                         M[1][0], M[1][1], M[1][2],
                         M[2][0], M[2][1], M[2][2]) / det;
      } /* End of 'Inverse' function */

      /* Evaluate transpose matrix function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID EvalTransMatr( VOID )
      {
        if (this->IsTransEn)
          return;
        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            TransposeM[i][j] = M[j][i];
        this->IsTransEn = TRUE;
      } /* End of 'EvalTransMatr' function */

      /* !!!!!! */
      /* Transpose matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr) transposed matrix
       */
      matr Transpose( VOID )
      {
        EvalTransMatr();
        return TransposeM;
        /*
        return matr(M.M[0][0], M.M[1][0], M.M[2][0], M.M[3][0],
                    M.M[0][1], M.M[1][1], M.M[2][1], M.M[3][1],
                    M.M[0][2], M.M[1][2], M.M[2][2], M.M[3][2],
                    M.M[0][3], M.M[1][3], M.M[2][3], M.M[3][3]);
        */
      } /* End of 'Transpose' function */


      static matr Translate( vec3<Type1> T )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    T.X, T.Y, T.Z, 1);
      } /* End of 'Translate' function */

      /* Multiply two matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr) result matrix.
       */
      matr operator*( const matr &Matr )
      {
        matr r; 
        INT k;

        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            for (r.M[i][j] = 0, k = 0; k < 4; k++)
              r.M[i][j] += M[i][k] * Matr.M[k][j];

        return r;
      } /* End of 'operator*' function */

      /* Multiply two matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr) result matrix.
       */
      matr operator*=( matr Matr )
      {
        matr r; 
        INT k;

        for (INT i = 0; i < 4; i++)
          for (INT j = 0; j < 4; j++)
            for (r.M[i][j] = 0, k = 0; k < 4; k++)
              r.M[i][j] += M[i][k] * Matr.M[k][j];

        return r;
      } /* End of 'operator*' function */

      /* Pointer operator of matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr *) Result matrix.
       */
      operator Type1*( VOID )
      {
        return M[0];
      } /* End of 'Type1*' function */

      /* Pointer operator of matrix function.
       * ARGUMENTS: None.
       * RETURNS: (matr *) Result matrix.
       */
      operator const Type1*( VOID ) const
      {
        return M[0];
      } /* End of 'Type1*' function */

      /* Get scale matrix function.
       * ARGUMENTS:
       *   - vector of scaling:
       *       const vec3<Type1> S;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr Scale( vec3<Type1> S )
      {
        return matr(S.X, 0, 0, 0,
                    0, S.Y, 0, 0,
                    0, 0, S.Z, 0,
                    0, 0, 0, 1);
      } /* End of 'Scale' function */
      
      /* Rotate matrix function.
       * ARGUMENTS:
       *   - vector to matrix:
       *       vec3<Type1> T;
       *   - angle in degrees:
       *       Type1 AngleInDegree;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr Rotate( vec3<Type1> V, Type1 AngleInDegree )
      {
        Type1 a = D2R(AngleInDegree), s = sin(a), c = cos(a);
        vec3<Type1> A = V.Normalizing();
        
        return matr(c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) + A.Z * s, A.X * A.Z * (1 - c) - A.Y * s, 0,
                    A.Y * A.X * (1 - c) - A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) + A.X * s, 0,
                    A.Z * A.X * (1 - c) + A.Y * s, A.Z * A.Y * (1 - c) - A.X * s, c + A.Z * A.Z * (1 - c), 0,
                    0, 0, 0, 1);
      } /* End of 'Rotate' function */

      /* Rotate around X axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       Type1 AngleInDegree;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr RotateX( Type1 AngleInDegree )
      {
        Type1 a = D2R(AngleInDegree), s = sin(a), c = cos(a);

        return matr(1, 0, 0, 0,
                    0, c, s, 0,
                    0, -s, c, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateX' function */

      /* Rotate around Y axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       Type1 AngleInDegree;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr RotateY( Type1 AngleInDegree )
      {
        Type1 a = D2R(AngleInDegree), s = sin(a), c = cos(a);

        return matr(c, 0, -s, 0,
                    0, 1, 0, 0,
                    s, 0, c, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateY' function */
      
      /* Rotate around Z axis matrix function.
       * ARGUMENTS:
       *   - angle in degrees:
       *       Type1 AngleInDegree;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr RotateZ( Type1 AngleInDegree )
      {
        Type1 a = D2R(AngleInDegree), s = sin(a), c = cos(a);
        return matr(c, s, 0, 0,
                   -s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateZ' function */

      /* Transform point position.
       * ARGUMENTS:
       *   - vectors to be dot multiplied:
       *       const vec3<Type1> V;
       * RETURNS:
       *   (vec3<Type1>) result vector.
       */
      vec3<Type1> TransformNormal( const vec3<Type1> &N ) 
      {
        Inverse();
        return vec3<Type1>(N.X * InvM[0][0] + N.Y * InvM[0][1] + N.Z * InvM[0][2],
                           N.X * InvM[1][0] + N.Y * InvM[1][1] + N.Z * InvM[1][2],
                           N.X * InvM[2][0] + N.Y * InvM[2][1] + N.Z * InvM[2][2]);
      } /* End of 'TransformNormal' function */

      /* Transform point position.
       * ARGUMENTS:
       *   - vectors to be dot multiplied:
       *       const vec3<Type1> V;
       * RETURNS:
       *   (vec3<Type1>) result vector.
       */
      vec3<Type1> TransformPoint( const vec3<Type1> V ) const
      {
        return vec3<Type1>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0],
                           V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1],
                           V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]);
      } /* End of 'TransformPoint' function */

      /* Transform point position.
       * ARGUMENTS:
       *   - vectors to be dot multiplied:
       *       const vec3<Type1> V;
       * RETURNS:
       *   (vec3<Type1>) result vector.
       */
      vec3<Type1> TransformVector( const vec3<Type1> V ) const
      {
        return vec3<Type1>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0],
                           V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1],
                           V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2]);
      } /* End of 'TransformVector' function */

      /* Multiply matrix and vector.
       * ARGUMENTS:
       *   - multiplier vector:
       *       const vec3<Type1> V;
       * RETURNS:
       *   (vec3<Type1>) result vector.
       */
      vec3<Type1> Transform4x4( const vec3<Type1> V ) const
      {
        Type1 w = V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + M[3][3];

        return vec3<Type1>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]) / w,
                           (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]) / w,
                           (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]) / w);
      } /* End of 'Transform4x4' function */
     /* Matrix look at viwer setup function.
      * ARGUMENTS:
      *   - Positoin:
      *      VEC Loc;
      *   - Where we looking for:
      *      VEC At;
      *   - Direction to up:
      *      VEC Up1;
      * RETURNS:
      *   (matr) result matrix.
      */
      static matr View( vec3<Type1> &Loc, vec3<Type1> &At, vec3<Type1> &Up1 ) 
      {
        vec3<Type1> Dir, Up, Right;

        Dir = At - Loc;
        Dir.Normalize();

        /* Cross product */
        Right = Dir % Up1;
        Right.Normalize();

        /* Cross product */
        Up = Right % Dir;

        return matr(Right.X, Up.X, -Dir.X, 0,
                    Right.Y, Up.Y, -Dir.Y, 0,
                    Right.Z, Up.Z, -Dir.Z, 0,
                    -(Loc & Right), -(Loc & Up), Loc & Dir, 1);
      } /* End of 'MatrView' function */

      /* Normalized device coordinates function.
       * ARGUMENTS:
       *   -  Left and Right:
       *        Type1 L, R;
       *   -  Bottom and Top:
       *        Type1 B, T;
       *   -  Near and Far:
       *        Type1 N, F;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr Frustum( Type1 L, Type1 R, Type1 B, Type1 T, Type1 N, Type1 F ) 
      {
        return matr(2 * N / (R - L), 0, 0, 0,
                    0, 2 * N / (T - B), 0, 0,
                    (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N), -1,
                    0, 0, -2 * N * F / (F - N), 0);
      } /* End of 'MatrFrustum' function */

   }; /* End of 'matr' class */
} /* end of 'math' namespace */

#endif /* __mth_matr_h_ */

/* END OF 'mth_matr.h' FILE */