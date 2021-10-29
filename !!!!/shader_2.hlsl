/***************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/
 
/* FILE NAME   : shader_2.hlsl
 * PURPOSE     : DirectX 12 Startup project.
 *               Default shader sample.
 * PROGRAMMER  : CGSG'2021.
 *               Ivan Dmitriev.
 * LAST UPDATE : 19.10.2021
 * NOTE        : SMain - Pixel/Vertex/Geom/Domain/Hull
 *               shader entry point.
 *                                 45
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 *
/
/* Vertex shader input structure */
struct VS_INPUT
{
  float3 Position : POSITION; // Vertex 3D space coordinates
  float3 Color : COLOR;       // Vertex color
}; /* End of 'VS_INPUT' structure */
 
/* Vertex shader output structure */
struct VS_OUTPUT
{
  float4 Position : SV_POSITION; // Vertex homogenuous coordinates
  float3 Color : COLOR;          // Vertex color
}; /* End of 'VS_OUTPUT' structure */

//cbuffer ConstantBuffer : register(b0)
//{
//  float4x4 MatrWVP;
//};

cbuffer ConstBuffer : register(b0)
{ 
  float4x4 MatrWVP;
  float4 Poses[988];
};

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);
 
/* Vertex shader main function.
 * ARGUMENTS:
 *   - vertex input data:
 *       VS_INPUT In;
 *   - unique vertex id (for instancing):
 *       uint instanceID;
 * RETURNS:
 *   - (VS_OUTPUT) Vertex position in screen space and vertex color.
 */
VS_OUTPUT VSMain( VS_INPUT In, uint instanceID : SV_InstanceID )
{
  VS_OUTPUT Res;

  //Res.Position = float4(In.Position, 1);
  Res.Color = In.Color.rgb;
  Res.Position = mul(MatrWVP, float4(In.Position, 1));
  //Res.Position.x += instanceID * 4;
  Res.Position.x += Poses[instanceID].x * 2;
  Res.Position.y += Poses[instanceID].y * 2;

  //float4x4 MatrWVP = float4x4(In.M[0], In.M[1], In.M[2], In.M[3]);
  //Res.Position = mul(float4(In.Pos, 1), (In.MatrWVP));
  //Res.Position = mul((MatrWVP), float4(In.Pos, 1));
  //Res.TexCoord = In.uv;

  return Res;
} /* End of 'VSMain' function */
 
/* Pixel shader main function.
 * ARGUMENTS:
 *   - pixel input data (interpolated from vertex shader):
 *       VS_INPUT In;
 * RETURNS:
 *   - (float4) Color.
 */
float4 PSMain( VS_OUTPUT In ) : SV_Target0
{
  //float3 tc = Texture.Sample(Sampler, In.TexCoord * 8).rgb;
  return float4(In.Color, 1);
  //return Texture.Sample(Sampler, In.TexCoord);
} /* End of 'PSMain' function */
 
/* END OF 'shader_1.hlsl' FILE */