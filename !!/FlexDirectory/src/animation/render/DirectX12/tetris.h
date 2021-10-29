#ifndef __tetris_h_
#define __tetris_h_

#include "../../../def.h"

namespace bodx
{
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

  struct figure
  {
  public:
    INT X, Y, Index, Rotation;

    figure( INT X, INT Y, INT Index, INT Rotation ) :
      X(X), Y(Y), Index(Index), Rotation(Rotation)
    {
    }
  };

  class field : public dx12::core_ref
  {
  private:
    std::string
      filled_item = "[]",
      empty_item = "  ",
      wall_item = "..";
    CHAR *Cells;
    INT W, H;

  public:
    field( INT W, INT H ) : Cells(new CHAR[W * H]), W(W), H(H)
    {
      memset(Cells, 0, W * H);
    }

    field( const field &F ) : Cells(new CHAR[F.W * F.H]), W(F.W), H(F.H)
    {
      for (INT y = 0; y < H; y++)
        for (INT x = 0; x < W; x++)
          Cells[y * W + x] = F.Cells[y * W + x];
    }

    INT Draw( VOID ) const
    {
      INT q = 0;
      for (INT y = 0; y < H + 2; y++)
      {
        for (INT x = 0; x < W + 2; x++)
          if (x == 0 || x == W + 1 || y == 0 || y == H + 1 || Cells[(y - 1) * W + x - 1])
            D3D->ConstantBufferData[0].Poses[q++] = vec4((FLT)(W / 2 - x), (FLT)(H / 2 - y), 0, 1);
          //else
          //  D3D->ConstantBufferData[0].Poses[q++] = vec4(0, 0, 0, 0);
      }
      return q;
    }

    VOID Clear( VOID )
    {
      ZeroMemory(Cells, W * H);
    }

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
    }

    VOID PlaceFigure( const figure F )
    {
      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          if (Figures[F.Index][F.Rotation][i][j])
            Cells[(F.Y + i) * W + j + F.X] = 1;
    }

    BOOL CheckFigureTouch( const figure &F, INT XDir, INT YDir ) const
    {
      for (INT i = 0; i < 4; i++)
        for (INT j = 0; j < 4; j++)
          if (Figures[F.Index][F.Rotation][i][j] &&
              (Cells[(F.Y + i + YDir) * W + F.X + j] ||
               Cells[(F.Y + i) * W + F.X + j + XDir] ||
               F.Y + i + YDir >= H                   ||
               F.X + j + XDir >= W || F.X + j + XDir < 0))
            return TRUE;
      return FALSE;
    }

    field &operator=( const field &F )
    {
      W = F.W;
      H = F.H;
      for (INT y = 0; y < H; y++)
        for (INT x = 0; x < W; x++)
          Cells[y * W + x] = F.Cells[y * W + x];
      return *this;
    }

    CHAR *operator[]( INT Y ) const
    {
      if (Y > H)
        return &Cells[0];
      return &Cells[Y * W];
    }
  };

  class tetris : public dx12::core_ref
  {
  private:
    const INT W, H;
    field Glass, Frame;
    figure Player;
    timer T;

    const INT ScoreMultiplyer = 200;
    INT Score;

  public:
    tetris( VOID ) :
      W(26), H(36),
      Glass(W, H), Frame(W, H), Player(0, 0, rand() % _countof(Figures), rand() % 4),
      Score(0)
    {
    }

  private:
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
      if (KeysClick[VK_LEFT] || d1 > 0.1 && Keys[VK_LEFT] && !Glass.CheckFigureTouch(Player, -1, 0))
          Player.X--, d1 = 0;
      if (KeysClick[VK_RIGHT] || d2 > 0.1 && Keys[VK_RIGHT] && !Glass.CheckFigureTouch(Player, 1, 0))
        Player.X++, d2 = 0;
      if (Keys[VK_DOWN] && !Glass.CheckFigureTouch(Player, 0, 1))
        Player.Y++;
      if (KeysClick[VK_SPACE] || d3 > 0.2 && Keys[VK_SPACE])
      {
        figure tmp = Player;
        tmp.Rotation = (Player.Rotation + 1) % 4;
        if (!Glass.CheckFigureTouch(tmp, 0, 0))
          Player.Rotation = (Player.Rotation + 1) % 4;
        d3 = 0;
      }
    }

    INT Render( VOID )
    {
      Frame = Glass;
      Frame.PlaceFigure(Player);
      if (Glass.CheckFigureTouch(Player, 0, 1))
        Glass.PlaceFigure(Player), Player = figure(0, 0, rand() % _countof(Figures), 0);

      // Output player's score
      //std::cout << "Your Score: " << Score << '\n';
      return Frame.Draw();
    }

  public:
    INT Tick( BYTE *Keys, BYTE *KeysClick )
    {
      T.Response();
      PlayerMove(Keys, KeysClick);
      Score += Glass.ClearLines() * ScoreMultiplyer;
      return Render();
    }
  };
}

#endif // __tetris_h_