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

/* Depth texture resource creation function.
 * ARGUMENTS:
 *   - depth buffer size in pixels:
 *       UINT Width, Height;
 * RETURNS:
 *   (ID3D12Resource *) vidx::dx12::core depth buffer resource pointer.
 */
ID3D12Resource * bodx::dx12::core_interface::CreateDepthBuffer( UINT Width, UINT Height )
{
  D3D12_RESOURCE_DESC ResDesc = {};

  ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  ResDesc.Width = Width;
  ResDesc.Height = Height;
  ResDesc.DepthOrArraySize = 1;
  ResDesc.MipLevels = 1;
  ResDesc.SampleDesc.Count = 1;
  ResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  ResDesc.Format = DXGI_FORMAT_D32_FLOAT;
  ResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

  D3D12_HEAP_PROPERTIES HeapProp = {};

  HeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

  ID3D12Resource *DBuf;
  D3D12_CLEAR_VALUE ClearV = {};

  ClearV.Format = ResDesc.Format;
  ClearV.DepthStencil.Depth = 1.0f;

  D3D->Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_NONE, // D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
                                  &ResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
                                  &ClearV, IID_PPV_ARGS(&DBuf));

  D3D12_CPU_DESCRIPTOR_HANDLE CDH;

  CDH = D3D->DSVHeap->GetCPUDescriptorHandleForHeapStart();

  D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};

  DSVDesc.Format = ResDesc.Format;
  DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
  DSVDesc.Texture2D.MipSlice = 0;

  D3D->Device->CreateDepthStencilView(DBuf, &DSVDesc, CDH);

  return DBuf;
} /* End of 'bodx::dx12::core_interface::CreateDepthBuffer' function */

/* Render target texture resource creation function.
 * ARGUMENTS:
 *   - render target size in pixels:
 *       UINT Width, Height;
 * RETURNS:
 *   (ID3D12Resource *) vidx::dx12::core render target resource pointer.
 */
ID3D12Resource * bodx::dx12::core_interface::CreateRenderTarget( UINT Width, UINT Height )
{
  D3D12_RESOURCE_DESC ResDesc {};

  ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  ResDesc.Width = Width;
  ResDesc.Height = Height;
  ResDesc.DepthOrArraySize = 1;
  ResDesc.MipLevels = 1;
  ResDesc.SampleDesc.Count = 1;
  ResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  ResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  ResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

  D3D12_HEAP_PROPERTIES HeapProp {};

  HeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

  ID3D12Resource *RT;
  D3D12_CLEAR_VALUE ClearV {};

  ClearV.Format = ResDesc.Format;
  ClearV.Color[0] = D3D->BackColor[0];
  ClearV.Color[1] = D3D->BackColor[1];
  ClearV.Color[2] = D3D->BackColor[2];
  ClearV.Color[3] = D3D->BackColor[3];

  D3D->Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
                                  &ResDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                  &ClearV, IID_PPV_ARGS(&RT));

  D3D12_CPU_DESCRIPTOR_HANDLE CDH {};

  CDH = D3D->RTVHeap->GetCPUDescriptorHandleForHeapStart(); 
  CDH.ptr += D3D->NoofBuffers * D3D->Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  D3D12_RENDER_TARGET_VIEW_DESC RTVDesc {};

  RTVDesc.Format = ResDesc.Format;
  RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
  RTVDesc.Texture2D.MipSlice = 0;
  RTVDesc.Texture2D.PlaneSlice = 0;

  D3D->Device->CreateRenderTargetView(RT, &RTVDesc, CDH);

  CDH = D3D->SRVHeap->GetCPUDescriptorHandleForHeapStart();

  D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc {};

  SRVDesc.Format = ResDesc.Format;
  SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  SRVDesc.Texture2D.MipLevels = 1;
  SRVDesc.Texture2D.MostDetailedMip = 0;
  SRVDesc.Texture2D.ResourceMinLODClamp = 0;
  SRVDesc.Texture2D.PlaneSlice = 0;
  SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

  D3D->Device->CreateShaderResourceView(RT, &SRVDesc, CDH);

  return RT;
} /* End of 'bodx::dx12::core_interface::CreateRenderTarget' function */

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