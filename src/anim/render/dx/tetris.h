/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12.h
  * PURPOSE     : Animation project.
  *               Direct X 12 .
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __tetris_h_
#define __tetris_h_

#include <vector>

#include "../../../def.h"

/* Beginning of 'ivdx' namespace */
namespace ivdx
{
  /* Beginning of 'tetris' class */
  struct figure
  {
    INT NumInFiguresTable;
    INT RotateNum;
  }; /* End of 'tetris' class */
  /* Beginning of 'tetris' class */
  class tetris
  {
  private:
    const INT W = 10, H = 20;      // game field size; 
    INT CurX = 5, CurY = 19;

  public:
    CHAR figures[7][4][4][4]; 
    CHAR CurFigure[4][4];

    CHAR glass[20][10];
    INT NumOfCubes = 0;
    std::vector<vec2> TranslateCubes;

    /* Initialization of game field function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID )
    {
      for (INT y = 0; y < H; y++)
        for (INT x = 0; x < W; x++)
          glass[y][x] = ' ';
      NumOfCubes = 4;

      //const CHAR *figures[] = {"***  *          ",
      //                         "***   *         ",
      //                         "*** *           ",
      //                         "****            ",
      //                         "**  **          ",
      //                         "**   **         ",
      //                         " ** **          "};

      const CHAR figures_1[7][5][5] = 
      {
        {
          "    ", 
          "*** ", 
          " *  ", 
          "    " 
        }, 
        {
          "    ", 
          "*** ", 
          "  * ", 
          "    " 
        }, 
        {
          "    ", 
          "*** ", 
          "*   ", 
          "    " 
        }, 
        {
          "    ", 
          "****", 
          "    ", 
          "    " 
        }, 
        {
          "    ", 
          " ** ", 
          " ** ", 
          "    " 
        }, 
        {
          "    ", 
          "**  ", 
          " ** ", 
          "    " 
        }, 
        {
          "    ", 
          "  **", 
          " ** ", 
          "    " 
        } 
      };
      for (INT i = 0; i < 7; i++)
      {
        CHAR tmp[4][4], block[4][4];
        //Save temporarily block

        for (INT k = 0; k < 4; k++)
          for (INT p = 0; p < 4; p++)
            tmp[k][p] = figures_1[i][k][p];
        for (INT k = 0; k < 4; k++)
          for (INT p = 0; p < 4; p++)
             figures[i][0][k][p] = tmp[k][p];

        for (INT t = 1; t < 4; t++)
        {
          for (INT k = 0; k < 4; k++)
            for (INT p = 0; p < 4; p++)
              block[k][p] = tmp[k][p];

          for (INT k = 0; k < 4; k++)
            for (INT p = 0; p < 4; p++)
              tmp[k][p] = block[3 - p][k];

          for (INT k = 0; k < 4; k++)
            for (INT p = 0; p < 4; p++)
               figures[i][t][k][p] = tmp[k][p];
          printf("");
        }
        //for (INT k = 0; k < 4; k++)
        //  for (INT p = 0; p < 4; p++)
        //    tmp[k][p] = figures_1[i][3 - p][k];
        //return;
      }
      printf("");

      //for (INT i = 0; i < 7; i++)
      //  for (INT k = 0; k < 4; k++)
      //    for (INT p = 0, j = 0; p < 4; p++, j++)
      //      figures_1[i][k][p] = figures[i][j];
      for (INT k = 0; k < 4; k++)
        for (INT p = 0; p < 4; p++)
           CurFigure[k][p] = figures[2][0][k][p];
    } /* End of 'Init' function */

    /* Place one figure function.
     * ARGUMENTS: 
     *   - input figure: 
     *       figure Fig;
     * RETURNS: None.
     */
    BOOL IsCollide( INT X, INT Y )
    {
      if (X >= W || Y >= H || X < 0 || Y < 0)
        return TRUE;
      if (glass[Y][X] == '*')
        return TRUE;
      return FALSE;
    } /* End of 'IsCollide' function */

    /* Place one figure function.
     * ARGUMENTS: None. 
     * RETURNS: None.
     */
    VOID Placefigure( VOID )
    {
      for (INT k = 0; k < 4; k++)
        for (INT p = 0; p < 4; p++)
          if (CurFigure[k][p] == '*')
            glass[CurY - k][CurX - p] = '*';
    } /* End of 'Placefigure' function */

    /* Check if figure is not colliding function.
     * ARGUMENTS: 
     *   - Figure translation:
     *       INT dx, INT dy;
     * RETURNS: None.
     */
    BOOL CheckFigure( VOID )
    {
      for (INT k = 0; k < 4; k++)
        for (INT p = 0; p < 4; p++)
          if (CurFigure[k][p] == '*')
            if (IsCollide(CurX - p, CurY - k))
              return FALSE;
      return TRUE;
    } /* End of 'CheckFigure' function */

    /* Draw field function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Draw( VOID )
    {
    } /* End of 'Draw' function */
    /* Draw field function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID MoveFigure( INT NewX, INT NewY )
    {
        for (INT k = 0; k < 4; k++)
          for (INT p = 0; p < 4; p++)
            if (CurFigure[k][p] == '*')
              glass[CurY - k][CurX - p] = ' ';

        CurX = NewX; 
        CurY = NewY; 

        for (INT k = 0; k < 4; k++)
          for (INT p = 0; p < 4; p++)
            if (CurFigure[k][p] == '*')
              glass[CurY - k][CurX - p] = '*';

    } /* End of 'MoveFigure' function */

    /* Draw field function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Response( BYTE *Keys )
    {
      if (Keys[VK_LEFT])
      {
        for (INT k = 0; k < 4; k++)
          for (INT p = 0; p < 4; p++)
            if (CurFigure[k][p] == '*')
              if (IsCollide(k + CurY, p + CurX))
                break;
        MoveFigure(CurX - 1, CurY);
      }

    }
    /* Game step function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Step( VOID )
    {
      static INT SavedCurY = CurY, SavedCurX = CurX; 
      static BOOL IsPut = FALSE;

      if (!IsPut)
      {
        for (INT k = 0; k < 4; k++)
          for (INT p = 0; p < 4; p++)
            if (CurFigure[k][p] == '*')
              glass[CurY - k][CurX - p] = ' ';
      }
      IsPut = FALSE;

      CurY--; 
      if (CurY <= 4)
      {  
        NumOfCubes += 4;

        CurX = 5; 
        CurY = 19; 
        IsPut = TRUE;
      }
      if (!CheckFigure())
      {
        NumOfCubes += 4;
     
        //Placefigure();

        CurX = 5; 
        CurY = 19; 
        IsPut = TRUE;
      }
      Placefigure();
      FILE *F;
      F = fopen("a.txt", "w");
      for (INT k = 0; k < 20; k++)
      {
        OutputDebugString(glass[k]);
        OutputDebugString("\n");
      }
      OutputDebugString("\n\n");
      
      INT count = 0;

      for (INT y = 0; y < 20; y++)
        for (INT x = 0; x < 10; x++)
        {
          if (glass[y][x] == '*')
            TranslateCubes.push_back(vec2(x, y));
        }
      //fprintf(F, "\n\n\n");
    } /* End of 'Step' function */

  }; /* End of 'tetris' class */
} /* end of 'ivdx' namespace */

#endif /* __tetris_h_ */

/* END OF 'tetris.h' FILE */

