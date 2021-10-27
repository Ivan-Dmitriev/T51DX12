/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_matr.h
 * PURPOSE     : Animation project.
 *               Mathematics library.
 *               Vector3 handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include <iostream>

#include "mthdef.h"

namespace math
{
  template<typename Type>
    class vec3
    {
      template<typename Type1> friend class matr;
    private:
      Type X, Y, Z;
    public:
      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      vec3( Type NewX, Type NewY, Type NewZ ) : X(NewX), Y(NewY), Z(NewZ)
      {
      } /* End of 'constructor' function */

      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      explicit vec3( Type NewV ) : X(NewV), Y(NewV), Z(NewV)
      {
      } /* End of 'constructor' function */

      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      explicit vec3( VOID )
      {
      } /* End of 'constructor' function */

      /* Length of vec3 operator.
       * ARGUMENTS: None.
       * RETURNS: (Type) length of vector.
       */
      Type operator!( VOID )
      {
        return sqrt(X * X + Y * Y + Z * Z);
      } /* End of 'operator!' function */

      /* Dot product operator of vec3 class.
       * ARGUMENTS:
       *   Component of dot product vector:
       *     - const vec3 &V;
       * RETURNS: (Type) result value.
       */
      Type operator&( const vec3 &V )
      {
        return X * V.X + Y * V.Y + Z * V.Z;
      } /* End of 'operator&' function */

      /* Cross product operator of vec3 class.
       * ARGUMENTS: 
       *   Component of cross product vector:
       *     - const vec3 &V;
       * RETURNS: (vec3) result vector.
       */
      vec3 operator%( const vec3 &V )
      {
        return vec3(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X);
      } /* End of 'operator%' function */

      /* Inverse coords of vec3 class.
       * ARGUMENTS: None.
       * RETURNS: (vec3) result vector.
       */
      vec3 operator-( VOID )
      {
        return vec3(-X, -Y, -Z);
      } /* End of 'operator-' function */

      /* Plus vector operator of vec3 class.
       *   Component of sum of vectors:
       *     - const vec3 &V;
       * RETURNS: (vec3) result vector.
       */
      vec3 operator+( const vec3 &V )
      {
         return vec3(V.X + X, V.Y + Y, V.Z + Z);
      } /* End of 'operator+' function */

      /* Minus vector operator of vec3 class.
       *   Component of subtraction of vectors:
       *     - const vec3 &V;
       * RETURNS: (vec3) result vector.
       */
      vec3 operator-( const vec3 &V )
      {
         return vec3(X - V.X, Y - V.Y, Z - V.Z);
      } /* End of 'operator-' function */

      /* Sum vec3 and number function.
       * ARGUMENTS: 
       *   - Summing number: 
       *       DBL N;
       * RETURNS: (vec3) result vector.
       */
      vec3 operator+( Type N )
      {
         return vec3(X + N, Y + N, Z + N);
      } /* End of 'operator/' function */
      /* Sum vec3 and number function.
       * ARGUMENTS: 
       *   - Summing number: 
       *       DBL N;
       * RETURNS: (vec3) result vector.
       */
      vec3 operator-( Type N )
      {
         return vec3(X - N, Y - N, Z - N);
      } /* End of 'operator/' function */

      /* Multiply vec3 by number function.
       * ARGUMENTS: None.
       * RETURNS: (vec3) result vector.
       */
      vec3 operator*( Type N )
      {
         return vec3(X * N, Y * N, Z * N);
      } /* End of 'operator*' function */

      /* Divide vec3 by number function.
       * ARGUMENTS: 
       *   - Divider number: 
       *       float N;
       * RETURNS: (vec3) result vector.
       */
      vec3 operator/( Type N )
      {
         if (N == 0)
           return vec3(0);
         return vec3(X / N, Y / N, Z / N);
      } /* End of 'operator/' function */

      /* Add vec3 to vec3 function.
       * ARGUMENTS: 
       *   Link on vectot sum component:
       *     - const vec3 &V;
       * RETURNS: (vec3 &) link on result vector.
       */
      vec3 & operator+=( const vec3 &V )
      {
         X += V.X;
         Y += V.Y;
         Z += V.Z;
         return *this;
      } /* End of 'operator+=' function */

      /* Substract vec3 from this vec3 function.
       * ARGUMENTS: 
       *   Link on vector substraction component:
       *     - const vec3 &V;
       * RETURNS: (vec3 &) link on result vector.
       */
      vec3 & operator-=( const vec3 &V )
      {
         X -= V.X;
         Y -= V.Y;
         Z -= V.Z;
         return *this;
      } /* End of 'operator-=' function */

      /* Multiply vec3 by vec3 function.
       * ARGUMENTS: 
       *   Link on vector multiplication component:
       *     - const vec3 &V;
       * RETURNS: (vec3 &) link on result vector.
       */
      vec3 & operator*=( const vec3 &V )
      {
         X *= V.X;
         Y *= V.Y;
         Z *= V.Z;
         return *this;
      } /* End of 'operator*=' function */

      /* Divide vec3 by vec3 function.
       * ARGUMENTS: 
       *   Link on vector division component:
       *     - const vec3 &V;
       * RETURNS: (vec3 &) link on result vector.
       */
      vec3 & operator/=( const vec3 &V )
      {
         X /= V.X;
         Y /= V.Y;
         Z /= V.Z;
         return *this;
      } /* End of 'operator/=' function */

      /* Normalize vec3 function.
       * ARGUMENTS: None.
       * RETURNS: (vec3) result vector.
       */
      vec3 Normalizing( VOID ) 
      {
        if (!*this == 0 || !*this == 1)
          return *this;
        Type length = sqrt(X * X + Y * Y + Z * Z);
        return *this / length;
        //return vec3(X / !*this, Y / !*this, Z / !*this);
      } /* End of 'Normalizing' function */

      /* Normalize this vector function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Normalize( VOID )
      {
        if (!*this == 0 || !*this == 1)
          return;
        Type length = !*this;
        X /= length;
        Y /= length;
        Z /= length;
        //*this /= !(*this);
      } /* End of 'Normalize' function */

      /* Find squared length of this vector function.
       * ARGUMENTS: None.
       * RETURNS: (Type) result value.
       */
      Type Length2( VOID )
      {
        return X * X + Y * Y + Z * Z;
      } /* End of 'Length2' function */

      /* Find distance between two vectors function.
       * ARGUMENTS: 
       *   Link on vector:
       *     - const vec3 &V;
       * RETURNS: (DBL) result value.
       */
      DBL Distance( const vec3 &V )
      {
        return sqrt((X - V.X) * (X - V.X) + (Y - V.Y) * (Y - V.Y) + (Z - V.Z) * (Z - V.Z));
      } /* End of 'Distance' function */

      /* Set zero vector function.
       * ARGUMENTS: None.
       * RETURNS: (vec3) result vector.
       */
      vec3 Zero( VOID )
      {
        return vec3(0);
      } /* End of 'Zero' function */
      /* Get random vector function.
       * ARGUMENTS: None.
       * RETURNS: (vec3 &) link on result vector.
       */
      vec3 & Rnd0( VOID )
      {
        return vec3(math::Rnd0(), math::Rnd0(), math::Rnd0());
      } /* End of 'Rnd0' function */

      /* Get coord of vector by index function.
       * ARGUMENTS: 
       *   Index of coord:
       *     - INT i;
       * RETURNS: (Type &) link on result value.
       */
      Type & operator[]( INT i )
      {
        assert(i >= 0 && i <= 2);

        if (i == 0)
          return X;
        else if (i == 1)
          return Y;
        else
          return Z;
      } /* End of 'operator[]' function */

      /* Get coord of vector by index function.
       * ARGUMENTS: 
       *   Index of coord:
       *     - INT i;
       * RETURNS: (Type) result value.
       */
      Type operator[]( INT i ) const
      {
        assert(i >= 0 && i <= 2);

        if (i == 0)
          return X;
        else if (i == 1)
          return Y;
        else
          return Z;
      } /* End of 'operator[]' function */

      /* Get address of first coord of vector function.
       * ARGUMENTS: None. 
       * RETURNS: None.
       */
      operator const Type *( VOID ) const
      {
        return &X;
      } /* End of 'const Type *' function */

      /* Get address of first coord of vector function.
       * ARGUMENTS: None. 
       * RETURNS: None.
       */
      operator const Type *( VOID ) 
      {
        return &X;
      } /* End of 'const Type *' function */

      /*
      vec3 & operator<<( const char *Text )
      {
        printf("<%f, %f, %f>", X, Y, Z);
        return *this;
      }
      */
      friend std::ostream & operator<<( std::ostream &C, const vec3 &V )
      {
        C << "<" << V.X << "," << V.Y << "," << V.Z << ">";
        return C;
      }
     /* Get minimum value function.
      * ARGUMENTS:
      *   - Input values:
      *       Type A, B;
      * RETURNS: (Type) result value.
      */
      static vec3 Min( vec3 V1, vec3 V2 )
      {
        return vec3(V1[0] > V2[0] ? V2[0] : V1[0],
                    V1[1] > V2[1] ? V2[1] : V1[1],
                    V1[2] > V2[2] ? V2[2] : V1[2]);
      } /* End of 'Min' function */
     /* Get maximum value function.
      * ARGUMENTS:
      *   - Input values:
      *       Type A, B;
      * RETURNS: (Type) result value.
      */
      static vec3 Max( vec3 V1, vec3 V2 )
      {
        return vec3(V1[0] > V2[0] ? V1[0] : V2[0],
                    V1[1] > V2[1] ? V1[1] : V2[1],
                    V1[2] > V2[2] ? V1[2] : V2[2]);
      } /* End of 'Max' function */

     /* Zero y-coord function.
      * ARGUMENTS:
      *   - Input values:
      *       Type A, B;
      * RETURNS: (Type) result value.
      */
      vec3 & GetVecInPlaneXZ( VOID )
      {
        Y = 5;
        return *this;
      }
   /* Zero y-coord function.
    * ARGUMENTS:
    *   - Input values:
    *       Type A, B;
    * RETURNS: (Type) result value.
    */
    vec3 GetVecInPlaneXZ( vec3 V )
    {
      return vec3(V[0], 5, V[2]);
    }

  }; /* End of 'vec3' class */

} /* end of 'math' namespace */

#endif /* __mth_vec3_h_ */

/* END OF 'mth_vec3.h' FILE */