/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_noise.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               Math noise.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_noise_h_
#define __mth_noise_h_

#include "mthdef.h"

namespace mth
{
  /* Noise table size constants */
  auto constexpr TAB_SIZE = (1 << 10);
  auto constexpr TAB_MASK = (TAB_SIZE - 1);

  template<typename type>
    class noise
    {
    private:
      type TabNoise[TAB_SIZE];
      INT Perm[TAB_SIZE];
    public:
      noise( VOID )
      {
        for (INT i = 0; i < TAB_SIZE; i++)
          TabNoise[i] = static_cast<DBL>(rand()) / RAND_MAX;
        for (INT i = 0; i < TAB_SIZE; i++)
          Perm[i] = i;

        for (INT i = 0; i < TAB_SIZE * 8; i++)
        {
          INT 
            a = rand() & TAB_MASK,
            b = rand() & TAB_MASK,
            tmp = Perm[a];
   
          Perm[a] = Perm[b];
          Perm[b] = tmp;
        } 
      }

      INT P( INT X ) const
      {
        return Perm[Clamp(X, 0, TAB_SIZE)];
      }

      INT Index1( INT X ) const
      {
        return P(X) & TAB_MASK;
      }

      INT Index2( INT X, INT Y ) const
      {
        return P(X + Index1(Y)) & TAB_MASK;
      }

      /* Obtain noise value function.
       * ARGUMENTS:
       *   - 2D argument:
       *       type X, type Y;
       * RETURNS:
       *   (type) noise value.
       */
      type Noise2D( type X, type Y ) const
      {
        INT ix = INT(floor(X)), ix1, iy = INT(floor(Y)), iy1;
        type fx, fy;

        fx = X - ix;
        fx = (3 - 2 * fx) * fx * fx;
        ix = ix & TAB_MASK;
        ix1 = (ix + 1) & TAB_MASK;

        fy = Y - iy;
        fy = (3 - 2 * fy) * fy * fy;
        iy = iy & TAB_MASK;
        iy1 = (iy + 1) & TAB_MASK;

        return 
          TabNoise[Index2(iy, ix)] * (1 - fx) * (1 - fy) +
          TabNoise[Index2(iy, ix1)] * fx * (1 - fy) +
          TabNoise[Index2(iy1, ix)] * (1 - fx) * fy +
          TabNoise[Index2(iy1, ix1)] * fx * fy;
      } /* End of 'Noise2D' function */

      /* Obtain noise value function.
       * ARGUMENTS:
       *   - 2D argument:
       *       type X, type Y;
       *   - amount of octaves:
       *       INT Octaves;
       * RETURNS:
       *   (type) noise value.
       */
      type NoiseTurb2D( type X, type Y, INT Octaves ) const
      {
        INT frac = 1, i;
        type val = 0;
      
        for (i = 0; i < Octaves; i++)
        {
          val += Noise2D(X, Y) / frac;
          frac <<= 1;
          X = (X + 27.18) * 2;
          Y = (Y + 47.13) * 2;
        }
        return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
      } /* End of 'NoiseTurb2D' function */   

      /* Obtain noise value function.
       * ARGUMENTS:
       *   - 1D argument:
       *       type X;
       * RETURNS:
       *   (type) noise value.
       */
      type Noise1D( type X ) const
      {
        INT ix = INT(floor(X)), ix1;
        type fx;
      
        fx = X - ix;
        fx = (3 - 2 * fx) * fx * fx;
        ix = ix & TAB_MASK;
        ix1 = (ix + 1) & TAB_MASK;
        return TabNoise[ix] * (1 - fx) + TabNoise[ix1] * fx;
      } /* End of 'Noise1D' function */

      /* Obtain noise value function.
       * ARGUMENTS:
       *   - 1D argument:
       *       type X;
       *   - amount of octaves:
       *       INT Octaves;
       * RETURNS:
       *   (type) noise value.
       */
      type NoiseTurb1D( type X, INT Octaves ) const
      {
        INT frac = 1, i;
        type val = 0;
      
        for (i = 0; i < Octaves; i++)
        {
          val += Noise1D(X) / frac;
          frac <<= 1;
          X = (X + 27.18) * 2;
        }
        return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
      }
      /* End of 'NoiseTurb1D' function */
    }; /* End of 'noise' class */
}

#endif /* __mth_noise_h_ */

/* END OF 'mth_matr.h' FILE */