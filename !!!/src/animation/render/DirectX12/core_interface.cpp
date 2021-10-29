/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : core_interface.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core interface.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 26.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>

#include "../../../bodx.h"

/* Class contructor.
 * ARGUMENTS:
 * RETURNS:
 *   - pointer to description of vertex element:
 *       D3D12_INPUT_ELEMENT_DESC *VertexDescription;
 *   - vertex description fileds counter:
 *       UINT VertexDescriptonFieldsCnt;
 *   - rasterisation parametrs:
 *       D3D12_FILL_MODE FillMode, D3D12_CULL_MODE CullMode;
 *   - shader name:
 *       const std::wstring &SName;
 * RETURNS: (ID3D12PipelineState *) Pointer to created pipeline.
 */
ID3D12PipelineState * bodx::dx12::core_interface::CreatePipeLine( D3D12_INPUT_ELEMENT_DESC *VertexDescription,
                                                                  UINT VertexDescriptonFieldsCnt,
                                                                  D3D12_FILL_MODE FillMode, D3D12_CULL_MODE CullMode,
                                                                  const std::wstring &SName )
{
  ID3D12PipelineState *State;
  shader_programm Shader(SName);

  D3D12_RASTERIZER_DESC RasterDesc;
  RasterDesc.FillMode = FillMode;
  RasterDesc.CullMode = CullMode;
  RasterDesc.FrontCounterClockwise = FALSE;
  RasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
  RasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
  RasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  RasterDesc.DepthClipEnable = TRUE;
  RasterDesc.MultisampleEnable = FALSE;
  RasterDesc.AntialiasedLineEnable = FALSE;
  RasterDesc.ForcedSampleCount = 0;
  RasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

  D3D12_BLEND_DESC BlendDesc;
  BlendDesc.AlphaToCoverageEnable = FALSE;
  BlendDesc.IndependentBlendEnable = FALSE;
  const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
    FALSE,
    FALSE,
    D3D12_BLEND_ONE,
    D3D12_BLEND_ZERO,
    D3D12_BLEND_OP_ADD,
    D3D12_BLEND_ONE,
    D3D12_BLEND_ZERO,
    D3D12_BLEND_OP_ADD,
    D3D12_LOGIC_OP_NOOP,
    D3D12_COLOR_WRITE_ENABLE_ALL,
  };
  for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    BlendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

  D3D12_GRAPHICS_PIPELINE_STATE_DESC PSODesc {};
  PSODesc.InputLayout = { VertexDescription, VertexDescriptonFieldsCnt };
  PSODesc.pRootSignature = D3D->RootSignature.Get();
  PSODesc.VS = Shader.Vertex.GetByteCode();
  PSODesc.PS = Shader.Pixel.GetByteCode();
  PSODesc.RasterizerState = RasterDesc;
  PSODesc.BlendState = BlendDesc;
  PSODesc.DepthStencilState.DepthEnable = FALSE;
  PSODesc.DepthStencilState.StencilEnable = FALSE;
  PSODesc.SampleMask = UINT_MAX;
  PSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  PSODesc.NumRenderTargets = 1;
  PSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
  PSODesc.SampleDesc.Count = 1;

  D3D->Device->CreateGraphicsPipelineState(&PSODesc, IID_PPV_ARGS(&State));

  return State;
} /* End of 'bodx::dx12::core_interface::CreatePipeLine' function */

/* END OF 'core_interface.cpp' FILE */