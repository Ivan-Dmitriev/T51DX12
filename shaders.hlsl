/***************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/
 
/* FILE NAME   : Default.hlsl
 * PURPOSE     : DirectX 12 Startup project.
 *               Default shader sample.
 * PROGRAMMER  : CGSG'2021.
 *               VB5.
 * LAST UPDATE : 21.10.2021
 * NOTE        : SMain - Pixel/Vertex/Geom/Domain/Hull
 *               shader entry point.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
struct PSInput
{
  float4 position : SV_POSITION;
  float2 uv : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
  float4x4 MatrWVP;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput VSMain( float4 position : POSITION, float4 uv : TEXCOORD )
{
  PSInput result;

  //result.position = mul(MatrWVP, positionqawd);
  result.position = position;
  result.uv = uv;

  return result;
}

float4 PSMain( PSInput input ) : SV_TARGET
{
  return g_texture.Sample(g_sampler, input.uv);
} /* END OF 'Default.hlsl' FILE */
