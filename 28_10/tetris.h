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

#define InFrame(X, Y) ((X) >= 0 && (X) < W && (Y) >= 0 && (Y) < H)
#define Frame(X, Y) Frame[(Y) * W + (X)]

/* Beginning of 'ivdx' namespace */
namespace ivdx
{
  /* Beginning of 'tetris' class */
  struct figure
  {
    INT NumInFiguresTable;
    INT RotateNum;
  }; /* End of 'tetris' class */

  /* Tetris representation class */
  class tetris
  {
  public:
    INT NumOfCubes = 0;             // Cubes number
    vec4 TransArray[100];           // Translation array
    INT W, H;                       // Frame size
    CHAR *Frame;                    // Game field

  private:
    static const INT NumOfFigs = 3; // Nnumber of existing figures
    INT CurStat;                    // Current figure rotate state
    INT CurFig;                     // Current figure index
    INT PosX, PosY;                 // Current figure center position
    DBL LastTime;                   // Time on previous frame
    BOOL IsNext;                    // Next step flag
    DBL Speed;                      // Game speed

    struct 
    {
      INT X, Y;
    } Centers[NumOfFigs] {
      {1, 1},
      {1, 1},
      {0, 0}
    };
    const CHAR *Figures[NumOfFigs][4] = 
    { 
      {
        "***-  *",
        "**-*-*",
        "-*-***",
        " *- *-**"
      },
      {
        "-****",
        " *- *- *- *",
        "-****",
        " *- *- *- *"
      },
      {
        "**-**",
        "**-**",
        "**-**",
        "**-**"
      },
    };

  public:
    tetris( VOID )
    {
    }

    /* Base class construictor. */
    tetris( INT W, INT H, DBL Speed = 1 ) :
      LastTime(0), IsNext(TRUE), 
      Speed(CLOCKS_PER_SEC * Speed),
      W(W), H(H)//, NumOfFigs(1) // Only now 1!!
    {
      INT Size = W * H;

      Frame = new CHAR[Size];
      for (INT i = 0; i < Size; i++)
        Frame[i] = ' ';
    } /* End of 'tetris' funcrion */

    /* Class destructor */
    ~tetris( VOID )
    {
      //if (Frame != nullptr)
      //  delete[] Frame;
    } /* End of '~tetris' funcrion */

    /* Move figure function 
     * ARGUMENTS:
     *   - Vector of moving:
     *       INT Dx, INT Dy;
     * RETURNS: None.
     */
    VOID Move( INT Dx, INT Dy )
    {
      if (!IsNext && !Collision(Dx, Dy))
      {
        Put(' ');
        PosX = ((PosX + Dx >= W || PosX + Dx < 0) ? PosX : PosX + Dx);
        PosY = ((PosY + Dy >= H || PosY + Dy < 0) ? PosY : PosY + Dy);
        Put('#');
        //Draw();
      }
    } /* End of 'Move' function */

    /* Draw field function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Draw( VOID )
    {
      //system("cls");

      //for (INT y = 0; y < H; y++)
      //{
      //  for (INT x = 0; x < W; x++)
      //    std::cout << Frame(x, y);
      //  std::cout << "\n";
      //}
    } /* End of 'Draw' function */

    /* Rotate current figure function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Rotate( VOID )
    {
      if (!IsNext)
      {
        Put(' ');
        CurStat = (CurStat + 1) & 3;
        if (Collision(0, 0))
          CurStat = (CurStat - 1) & 3;
        Put('#');
        //Draw();
      }
    } /* End of 'Rotate' function */

    /* Figure collision function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    BOOL Collision( INT Dx, INT Dy )
    {
      INT i = -1;
      INT tx = PosX - Centers[CurFig].X, 
        ty = PosY - Centers[CurFig].Y;

      while (Figures[CurFig][CurStat][++i])
        switch(Figures[CurFig][CurStat][i])
        {
        case '*':
          if (InFrame(tx + Dx, ty + Dy) && Frame(tx + Dx, ty + Dy) == '%' || 
              (Dy > 0 && ty + Dy >= H) || (Dx > 0 && tx + Dx >= W) ||
              (Dy < 0 && ty + Dy < 0) || (Dx < 0 && tx + Dx < 0))
            return TRUE;
          tx++;
          break;
        case ' ':
          tx++;
          break;
        case '-':
          ty++;
          tx = PosX - 1;
          break;
        }
      return FALSE;
    } /* End of 'Collision' function */

    /* Put current figure to frame function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Put( CHAR Cell )
    {
      INT i = -1;
      INT tx = PosX - Centers[CurFig].X, 
        ty = PosY - Centers[CurFig].Y;

      while (Figures[CurFig][CurStat][++i])
      {
        switch(Figures[CurFig][CurStat][i])
        {
        case '*':
          if (InFrame(tx, ty))
            Frame(tx, ty) = Cell;
          tx++;
          break;
        case ' ':
          tx++;
          break;
        case '-':
          ty++;
          tx = PosX - Centers[CurFig].X;
          break;
        }
      }
    } /* End of 'Put' function */

    /* Check if line is full function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CheckLine( VOID )
    {
      INT x, y, y2;
      auto Line = [&]( INT y )
      {
        INT t;

        for (t = 0; t < W; t++)
          if (Frame(t, y) != '%')
            return FALSE;
        return TRUE;
      };

      for (y = 0; y < H; y++)
        if (Line(y))
          for (x = 0; x < W; x++)
          {
            for (y2 = y; y2 > 0; y2--)
              Frame(x, y2) = Frame(x, y2 - 1);
            Frame(x, 0) = ' ';
          }
    } /* End of 'CheckLine' funcrion */

    /* Main responce function 
     * ARGUMENTS: None. 
     * RETURNS: None.
     */
    VOID Responce( BYTE *Key )
    {
      if (Key['q'] || Key['e'])  
        Rotate();
      if (Key['j'])
        Move(-1, 0);
      if (Key['l'])
        Move(1, 0);
      if (Key['k'])
        Move(0, 1);
    } /* End of 'Responce' functon */

    /* Game step function 
     * ARGUMENTS: 
     *   - Current time:
     *       DBL CurTime;
     * RETURNS: None.
     */
    VOID GameStep( DBL CurTime )
    {
      if (CurTime - LastTime <= Speed)
        return;

      LastTime = CurTime;
      if (IsNext)
      {
        NumOfCubes += 4;
        CurFig = rand() % NumOfFigs;
        CurStat = rand() & 3;
        PosX = rand() % (W - 2) + 1;
        PosY = -Centers[CurFig].Y;
        IsNext = FALSE;
      }
      else
      {
        Put(' ');
        PosY++;
        // Check if there is a collision
        if (Collision(0, 1))
        {
          IsNext = TRUE;
          Put('%');
          CheckLine();
        }
        else
          Put('#');
      }
      // Put('#');
      //Draw();
      memset(TransArray, 0, sizeof(INT) * 200);

      INT i = 0;
      for (INT k = 0; k < 20; k++)
        for (INT p = 0; p < 10; p++)
          if ((Frame(p, k) == '#') || (Frame(p, k) == '%'))
            TransArray[i++] = vec4(p, k, 0, 0);

    } /* End of 'GameStep' functon */
  }; /* End of 'tetris' class */
} /* end of 'ivdx' namespace */

#endif /* __tetris_h_ */

/* END OF 'tetris.h' FILE */

