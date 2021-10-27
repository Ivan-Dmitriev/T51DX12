/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12_primitive.h
  * PURPOSE     : Animation project with Direct3D12.
  *               Primitive class declaration module.
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __dx12_primitive_h_
#define __dx12_primitive_h_

#include "d3dx12.h"
#include "dx12.h"
#include "../../../def.h"


/* Beginning of 'ivdx' namespace */
namespace ivdx
{
  /* Structure of storing data of vertex */
  typedef struct tagid3VERTEX
  {
    vec3 P;   /* Vertex position */
    vec2 T;   /* Texturte coordinates */
    vec3 N;   /* Normal */
    vec4 C;  /* Color */
  } id3VERTEX;

  /* Beginning of 'dx12' namespace */
  namespace dx12
  {
    /* Beginning of 'primitive' class */
    class primitive
    {
      public:
        UINT
          NumOfElements,    /* Number of indices/vecrtices */
          VA,               /* Vertex array Id */
          VBuf,             /* Vertex buffer Id */
          IBuf;             /* Index buffer Id */

        matr Trans;   /* Additional transformation matrix */
        INT MtlNo;    /* Material number in material array */

        vec3 MinBB, MaxBB; /* max and min bound box */
  /* Load primitive from '*.OBJ' file function.
   * ARGUMENTS:
   *   - pointer to primitive to load:
   *       ns6PRIM *Pr;
   *   - '*.OBJ' file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL PrimLoad( primitive *Pr, const CHAR *FileName )
  {
    FILE *F;
    INT i, nv = 0, nind = 0, size;
    id3VERTEX *V;
    UINT *Ind;
    static CHAR Buf[1000];
 
    memset(Pr, 0, sizeof(primitive));
    if ((F = fopen(FileName, "r")) == NULL)
      return FALSE;
 
    /* Count vertexes and indexes */
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      if (Buf[0] == 'v' && Buf[1] == ' ')
        nv++;
      else if (Buf[0] == 'f' && Buf[1] == ' ')
      {
        INT n = 0;
 
        for (i = 1; Buf[i] != 0; i++)
          if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
            n++;
        nind += (n - 2) * 3;
      }
    }
 
    size = sizeof(id3VERTEX) * nv + sizeof(INT) * nind;
    //if ((V = malloc(size)) == NULL)
    //  return FALSE;
    V = new id3VERTEX[size];
    //V = new vertex[size];
    //Ind = new INT[ ]
    Ind = (UINT *)(V + nv);
    memset(V, 0, size);
 
    /* Load primitive */
    rewind(F);
    nv = 0;
    nind = 0;
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      if (Buf[0] == 'v' && Buf[1] == ' ')
      {
        DBL x, y, z;
        /* !!!!!!!!!!!!!!!!! */
      
        //vec4 c(math::Rnd1F(), math::Rnd1F(), math::Rnd1F(), 1);
 
        (VOID)sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
        V[nv].C = vec4(0, 0, 0, 1);
        V[nv++].P = vec3(x, y, z);
      }
      else if (Buf[0] == 'f' && Buf[1] == ' ')
      {
        INT n = 0, n0 = 0, n1 = 0, nc = 0;
 
        for (i = 1; Buf[i] != 0; i++)
          if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          {
            (VOID)sscanf(Buf + i, "%i", &nc);
            if (nc < 0)
              nc = nv + nc;
            else
              nc--;
 
            if (n == 0)
              n0 = nc;
            else if (n == 1)
              n1 = nc;
            else
            {
              Ind[nind++] = n0;
              Ind[nind++] = n1;
              Ind[nind++] = nc;
              n1 = nc;
            }
            n++;
          }
      }
    }
    fclose(F);

    delete[] V;
    return TRUE;
  } /* End of 'PrimLoad' function */



    }; /* End of 'primitive' class */
  } /* end of 'dx12' namespace */
} /* end of 'ivdx' namespace */


#endif /* __dx12_primitive_h_ */

/* END OF 'dx12_primitive.h' FILE */