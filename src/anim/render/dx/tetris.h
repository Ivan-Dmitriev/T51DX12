/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : tetris.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               Tetris game module.
 * PROGRAMMER  : CGSG'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 27.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tetris_h_
#define __tetris_h_

#include "../../../def.h"
#include "../../timer.h"
#include "dx12.h"

/* Project namespace */
namespace ivdx
{
  // bricks configurations
  static constexpr BYTE Figures[7][4][4][4] =
  {
    { 
      {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
      },
      {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
      },
    },

    {
      {
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
      },
    },

    {
      {
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
      },
    },

    {
      {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
      },
    },

    {
      {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
      },
    },

    {
      {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
    },

    {
      {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
      },
      {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
      },
    },
  };

  /* Figure class */
  struct figure
  {
  public:
    INT X, Y, Index, Rotation;

    /* Class constructor.
     * ARGUMENTS:
     *   - position:
     *        INT X, Y;
     *   - figure index:
     *        INT Index;
     *   - rotation:
     *        INT Rotation;
     */
    figure( INT X, INT Y, INT Index, INT Rotation ) :
      X(X), Y(Y), Index(Index), Rotation(Rotation)
    {
    } /* End of 'figure' function */
  }; /* End of 'figure' class */

  /* Field class */
  class field 
  {
  private:
    std::string
      filled_item = "[]",
      empty_item = "  ",
      wall_item = "..";
    CHAR *Cells;
    INT W, H;

  public:
    vec4 TransCubes[20000];
    INT NumOfCubes = 0;

    /* Class constructor.
     * ARGUMENTS:
     *   - size:
     *        INT W, H;
     */
    field( INT W, INT H ) : Cells(new CHAR[W * H]), W(W), H(H)
    {
      memset(Cells, 0, W * H);
    } /* End of 'field' function */
    
    /* Copy class constructor.
     * ARGUMENTS:
     *   - other field:
     *       const field &F;
     */
    field( const field &F ) : Cells(new CHAR[F.W * F.H]), W(F.W), H(F.H)
    {
      for (INT y = 0; y < H; y++)
        for (INT x = 0; x < W; x++)
          Cells[y * W + x] = F.Cells[y * W + x];
    } /* End of 'field' function */

    /* Draw field function.
     * ARGUMENTS: None.
     * RETURNS: (INT)Num of active bricks.
     */
    INT Draw( VOID )
    {
      INT q = 0;
      //for (INT y = 0; y < H + 2; y++)
      //{
      //  for (INT x = 0; x < W + 2; x++)
      //    if (x == 0 || x == W + 1 || y == 0 || y == H + 1 || Cells[(y - 1) * W + x - 1])
      //      TransCubes[q++] = vec4((FLT)(-W / 2 + x), (FLT)(H / 2 - y), 0, 0);
      //}
      for (INT y = 0; y < H + 2; y++)
      {
        for (INT x = 0; x < W + 2; x++)
          if (x == 0 || x == W + 1 || y == 0 || y == H + 1 || Cells[(y - 1) * W + x - 1])
            TransCubes[q++] = vec4(2 * x, 2 * (H + 2 - y), 0, 0);
      }

      q += 8;
      NumOfCubes = q;

      return q;
    } /* End of 'Draw' function */

    /* Clear field function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Clear( VOID )
    {
      ZeroMemory(Cells, W * H);
    } /* End of 'Clear' function */

    /* Clear lines field function.
     * ARGUMENTS: None.
     * RETURNS: (INT)Num of active bricks.
     */
    INT ClearLines( VOID )
    {
      INT ClearedLinesCount = 0;

      for (INT i = 0; i < H; i++)
      {
        BOOL ShouldClear = TRUE;
        for (INT j = 0; j < W; j++)
          if (!Cells[i * W + j])
            ShouldClear = FALSE;

        if (ShouldClear)
        {
          for (INT y = i; y >= 1; y--)
            for (INT x = 0; x < W; x++)
              Cells[y * W + x] = Cells[(y - 1) * W + x];
          memset(Cells, 0, W);
          ClearedLinesCount++;
        }
      }

      return ClearedLinesCount;
    } /* End of 'ClearLines' function */

    /* Place figure function.
     * ARGUMENTS:
     *   - refference to figure:
     *       const figure &F;
     * RETURNS: None.
     */
    VOID PlaceFigure( const figure &F )
    {
      
      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          if (Figures[F.Index][F.Rotation][i][j])
            Cells[(F.Y + i) * W + j + F.X] = 1;
    } /* End of 'PlaceFigure' function */

    /* Place figure function.
     * ARGUMENTS:
     *   - refference to figure:
     *       const figure &F;
     *   - direction:
     *       const mth::vec2<INT> &Dir;
     * RETURNS: (BOOL) TRUE if touched, FALSE otherwise.
     */
    BOOL CheckFigureTouch( const figure &F, math::vec2<INT> Dir ) 
    {
      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          if (Figures[F.Index][F.Rotation][i][j] &&
              (Cells[(F.Y + i + Dir[1]) * W + F.X + j] ||
               Cells[(F.Y + i) * W + F.X + j + Dir[0]] ||
               F.Y + i + Dir[1] >= H ||
               F.X + j + Dir[0] >= W || F.X + j + Dir[0] < 0))
            return TRUE;
      return FALSE;
    } /* End of 'CheckFigureTouch' function */

    /* Copiyng operator function.
     * ARGUMENTS:
     *   - refference to field:
     *       const field &F;
     * RETURNS: (field &) Seld-refference.
     */
    field & operator=( const field &F )
    {
      W = F.W;
      H = F.H;
      for (INT y = 0; y < H; y++)
        for (INT x = 0; x < W; x++)
          Cells[y * W + x] = F.Cells[y * W + x];
      return *this;
    } /* End of 'operator=' function */ 

    /* Get line operator function.
     * ARGUMENTS:
     *   - line:
     *       INT Y;
     * RETURNS: (CHAR *) y line.
     */
    CHAR * operator[]( INT Y ) const
    {
      if (Y > H)
        return &Cells[0];
      return &Cells[Y * W];
    } /* End of 'operator[]' function */

    /* Destructor */
    ~field( VOID )
    {
      delete[] Cells;
    } /* End of '~field' function */
  }; /* End of 'field' class */

  /* Tetris class */
  class tetris 
  {
  private:
    figure Player;
    ::ivdx::timer T;

    INT IsPause = FALSE;

  public:
    const INT ScoreMultiplyer = 200;
    INT Score;

    const INT W, H;
    //INT NumOfCubes = 4;
    
    field Glass, Frame;
    
    /* Default constructor. */
    tetris( VOID ) :
      W(10), H(20),
      Glass(W, H), Frame(W, H), Player(0, 0, rand() % _countof(Figures), rand() % 4),
      Score(0)
    {
    } /* End of 'tetris' function */

  private: 
    /* Move figure function.
     * ARGUMENTS:
     *   - input states:
     *       BYTE *Keys, *KeysClick;
     * RETURNS: None.
     */
    VOID PlayerMove( BYTE *Keys, BYTE *KeysClick )
    {
      static DOUBLE delta = 0;
      delta += T.DeltaTime;
      if (delta > 0.3)
        Player.Y++, delta = 0;

      static DOUBLE d1 = 0, d2 = 0, d3 = 0;
      d1 += T.DeltaTime;
      d2 += T.DeltaTime;
      d3 += T.DeltaTime;
      if ((KeysClick[VK_LEFT] || d1 > 0.1 && Keys[VK_LEFT]) && !Glass.CheckFigureTouch(Player, math::vec2<INT>(-1, 0)))
          Player.X--, d1 = 0;
      if ((KeysClick[VK_RIGHT] || d2 > 0.1 && Keys[VK_RIGHT]) && !Glass.CheckFigureTouch(Player, math::vec2<INT>(1, 0)))
        Player.X++, d2 = 0;
      if (Keys[VK_DOWN] && !Glass.CheckFigureTouch(Player, math::vec2<INT>(0, 1)))
        Player.Y++;
      if (KeysClick['R'] || d3 > 0.2 && Keys['R'])
      {
        figure tmp = Player;
        tmp.Rotation = (Player.Rotation + 1) % 4;
        if (!Glass.CheckFigureTouch(tmp, math::vec2<INT>(0, 0)))
          Player.Rotation = (Player.Rotation + 1) % 4;
        d3 = 0;
      }
    } /* End of 'PlayerMove' function */

    /* Render game function.
     * ARGUMENTS: None.
     * RETURNS: (INT) Number of active bricks.
     */
    INT Render( VOID )
    {
      Frame = Glass;
      Frame.PlaceFigure(Player);
      if (Glass.CheckFigureTouch(Player, math::vec2<INT>(0, 1)))
      {
        //NumOfCubes += 4;
        Glass.PlaceFigure(Player), Player = figure(0, 0, rand() % _countof(Figures), 0);
      }

      // Output player's score
      //std::cout << "Your Score: " << Score << '\n';
      return Frame.Draw();
    } /* End of 'Render' function */

  public:
    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: (INT) Number of active bricks.
     */
    INT Tick( BYTE *Keys, BYTE *KeysClick )
    {
      if (Keys[VK_CONTROL] && KeysClick['P'])
        IsPause = !IsPause;
      T.Response();
      if (!IsPause)
      {
        PlayerMove(Keys, KeysClick);
        Score += Glass.ClearLines() * ScoreMultiplyer;
      }
      return Render();
    } /* End of 'Tick' function */
  }; /* End of 'tetris' class */
} /* end of 'bodx' namespace */

#endif // __tetris_h_

/* END OF 'tetris.h' FILE */