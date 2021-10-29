/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx_12_response.cpp
  * PURPOSE     : Animation project with Direct3D12.
  *               DirectX12 Class Responce function implementation module.
  *               D3D initialization/deinitialization functions.
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <ivdx.h>
#include "../../../ivdx.h"

#include "../../../ivdx.h"
#include "dx12.h"
#include "d3dx12.h"

/* D3D12 Responce function.
 * ARGUMENTS: 
 *   - State of keys on the current frame:
 *       BYTE *Keys;
 *   - Global interframe interval:
 *       FLT GlobalDeltaTime;
 *   - Mouse coordinates:
 *       FLT MouseDX, MouseDY;
 * RETURNS: None.
 */
VOID ivdx::dx12::core::Response( BYTE *Keys, FLT GlobalDeltaTime, FLT MouseDX, FLT MouseDY )
{
  if (Keys['W'] || Keys['A'] || Keys['D'] || Keys['S'])
    SceneCam.Move((SceneCam.Dir * ((Keys['W'] - Keys['S'])) + 
                   SceneCam.Right * ((Keys['D'] - Keys['A']))) * 30 * GlobalDeltaTime);
  if (Keys[VK_RBUTTON])
  {
    FLT Hp, Wp = Hp = SceneCam.ProjDist;

    if (SceneCam.FrameW > SceneCam.FrameH)
      Wp *= static_cast<FLT>(SceneCam.FrameW) / SceneCam.FrameH;
    else
      Hp *= static_cast<FLT>(SceneCam.FrameH) / SceneCam.FrameW;

    FLT Dist = !(SceneCam.At - SceneCam.Loc);

    FLT sx = -MouseDX * Wp / SceneCam.FrameW * Dist / SceneCam.ProjDist;
    FLT sy = MouseDY * Hp / SceneCam.FrameH * Dist / SceneCam.ProjDist;

    SceneCam.Move(SceneCam.Right * sx + SceneCam.Up * sy);
  }
  //if (Keys[VK_LBUTTON] || Keys[VK_LEFT] || Keys[VK_RIGHT])
  //  SceneCam.Rotate(vec3(0, 1, 0), (-Keys[VK_LBUTTON] * MouseDX +
  //                                      (Keys[VK_LEFT] - Keys[VK_RIGHT])) * GlobalDeltaTime * 130);

  //if (Keys[VK_LBUTTON] || Keys[VK_UP] || Keys[VK_DOWN])
  //  SceneCam.Rotate(SceneCam.Right, (-Keys[VK_LBUTTON] * MouseDY +
  //                                          (Keys[VK_UP] - Keys[VK_DOWN])) * GlobalDeltaTime * 130);
  if (Keys[VK_CONTROL] || Keys[VK_SPACE])
    SceneCam.Move(vec3(0, 20 * GlobalDeltaTime, 0) * (Keys[VK_SPACE] - Keys[VK_CONTROL]));
} /* End of 'ivdx::dx12::core::Response' function */

  /* END OF 'dx_12_response.cpp' FILE */