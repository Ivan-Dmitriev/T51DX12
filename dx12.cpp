/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12.cpp
  * PURPOSE     : Animation project with Direct3D12.
  *               DirectX12 Class implementation module.
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 02.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <ivdx.h>
#include <vector>
#include <fstream>

#include "../../../ivdx.h"

#include "d3dx12.h"
#include "dx12_shader.h"
#include "dx12.h"

//#include <client.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

/* Wait for previous frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ivdx::dx12::core::WaitForPreviousFrame( VOID )
{
  // Signal and increment the Fence value.
  const UINT64 fenceV = FenceValue;

  ComQueue->Signal(Fence.Get(), fenceV);
  FenceValue++;

  // Wait until the previous frame is finished.
  if (Fence->GetCompletedValue() < fenceV)
  {
    Fence->SetEventOnCompletion(fenceV, Event);
    WaitForSingleObject(Event, INFINITE);
  }

  FrameIndex = SwapChain->GetCurrentBackBufferIndex();
} /* End of 'ivdx::dx12::core::WaitForPreviousFrame' function */

/* Depth buffer creation function.
 * ARGUMENTS:
 *   - buffer size:
 *       UINT Size;
 *   - buffer data pointer:
 *       VOID *Data;
 *   - width and height of depth buffer:
 *       INT W, INT H;
 *       Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
 * RETURNS: (Microsoft::WRL::ComPtr<ID3D12Resource>) pointer on created buffer.
 */
Microsoft::WRL::ComPtr<ID3D12Resource> ivdx::dx12::core::CreateDepthBuffer( INT W, INT H )
{
  Microsoft::WRL::ComPtr<ID3D12Resource> DepthBuf;
  depthStencilDesc.Width = W;
  depthStencilDesc.Height = H;

  CD3DX12_HEAP_PROPERTIES CDXheapprop(D3D12_HEAP_TYPE_DEFAULT);
  Device->CreateCommittedResource(
    &CDXheapprop, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON,
    &optClear, IID_PPV_ARGS(DepthBuf.GetAddressOf()));
  Device->CreateDepthStencilView(DepthBuf.Get(), nullptr, 
    DSVHeap->GetCPUDescriptorHandleForHeapStart());

  CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(DepthBuf.Get(), 
    D3D12_RESOURCE_STATE_COMMON,
    D3D12_RESOURCE_STATE_DEPTH_WRITE); 
  ComList->ResourceBarrier(1, &tmp);
  ComList->Close();

  ID3D12CommandList *cmdsLists[] =
  {
    ComList.Get()
  };
  ComQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

  return DepthBuf;
} /* End of 'ivdx::dx12::core::CreateDepthBuffer' function */

/* D3D12 window resize function.
 * ARGUMENTS:
 *   - new window size:
 *       UINT W, H;
 * RETURNS: None.
 */
VOID ivdx::dx12::core::Resize( UINT W, UINT H )
{
  Width = W;
  Height = H;

  if (W * H == 0)
    return;
  FenceValue++;
  ComQueue->Signal(Fence.Get(), FenceValue);
  Fence->SetEventOnCompletion(FenceValue, Event);
  WaitForSingleObject(Event, INFINITE);

  // Remove olds buffers
  for (INT i = 0; i < NumOfBuffers; i++)
    if (BackBuffers[i])
      BackBuffers[i].Reset();

  SwapChain->ResizeBuffers(NumOfBuffers, W, H, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
  // Resize an recreate buffers 
  for (INT i = 0; i < NumOfBuffers; i++)
  {
    SwapChain->GetBuffer(i, IID_PPV_ARGS(BackBuffers[i].GetAddressOf()));
    
    D3D12_CPU_DESCRIPTOR_HANDLE CDH;
    CDH = RTVHeap->GetCPUDescriptorHandleForHeapStart();
    CDH.ptr +=
      i * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    Device->CreateRenderTargetView(BackBuffers[i].Get(), nullptr, CDH);
  }

  // Remove old depth buffer and render target 
  //if (DepthStencilBuffer != nullptr)
  //  DepthStencilBuffer->Release();
  if (RenderTarget != nullptr)
    RenderTarget->Release();

  RenderTarget = CreateRenderTarget(W, H);
  //DepthStencilBuffer = CreateDepthBuffer(W, H);
} /* End of 'ivdx::dx12::core::Resize' function */

/* D3D12 create render target function.
 * ARGUMENTS:
 *   - new window size:
 *       UINT W, H;
 * RETURNS: (ID3D12Resource *) pointer on result target.
 */
ID3D12Resource * ivdx::dx12::core::CreateRenderTarget( UINT W, UINT H )
{
  D3D12_RESOURCE_DESC ResDesc = {};
  ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  ResDesc.Width = W;
  ResDesc.Height = H;
  ResDesc.DepthOrArraySize = 1; 
  ResDesc.MipLevels = 1;
  ResDesc.SampleDesc.Count = 1;
  ResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  ResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  ResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

  D3D12_HEAP_PROPERTIES HeapProp = {};
  
  HeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

  ID3D12Resource *RT;
  D3D12_CLEAR_VALUE ClearV = {};
  ClearV.Format = ResDesc.Format; 

  ClearV.Color[0] = BackColor[0];
  ClearV.Color[1] = BackColor[1];
  ClearV.Color[2] = BackColor[2];
  ClearV.Color[3] = BackColor[3];

  Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, 
                                  &ResDesc, D3D12_RESOURCE_STATE_PRESENT, 
                                  &ClearV, IID_PPV_ARGS(&RT));
  D3D12_CPU_DESCRIPTOR_HANDLE CDH;

  CDH = RTVHeap->GetCPUDescriptorHandleForHeapStart();
  CDH.ptr +=
    3 * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  D3D12_RENDER_TARGET_VIEW_DESC RTVDesr = {};

  RTVDesr.Format = ResDesc.Format;
  RTVDesr.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
  RTVDesr.Texture2D.MipSlice = 0;
  RTVDesr.Texture2D.PlaneSlice = 0;

  //Device->CreateRenderTargetView(RT, &RTVDesr, CDH);
  //CDH = SRVHeap->GetCPUDescriptorHandleForHeapStart();

  return RT;
} /* End of 'ivdx::dx12::core::CreateRenderTarget' function */

/* Rendering pipeline creation function.
 * ARGUMENTS:
 *   - pipeline index:
 *       UINT PipeLineIndex;
 * RETURNS: (PIPELINE) result pipeline filled structure.
 */
ivdx::dx12::core::PIPELINE ivdx::dx12::core::CreatePipeline( UINT PipeLineIndex )
{
  PIPELINE Res;
  D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc {};

  Desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

  Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
  Desc.PrimitiveTopologyType =
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  Desc.NumRenderTargets = 1;

  Desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;
  Desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
  Desc.SampleDesc.Count = 1;
  //Desc.SampleDesc.Quality = 0;
  //Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
  Desc.SampleMask = UINT_MAX;

  //Desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
  //Desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
  
  Desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  
  if (PipeLineIndex == 1)
  {
    Desc.DepthStencilState.DepthEnable = FALSE;
    //Desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    //Desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; 
    Desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
  }
  
  Microsoft::WRL::ComPtr<ID3DBlob> VSCode, PSCode, Error;

  const WCHAR *ShaderPath = L"BIN/SHADERS/Default/shader.hlsl";

  UINT compileFlags = 0;

#ifdef _DEBUG
  // Enable better shader debugging with the graphics debugging tools.
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
  shader Vert(L"shader", "VSMain", "vs_5_0");
  shader Pixel(L"shader", "PSMain", "ps_5_0");

  //if ((D3DCompileFromFile(ShaderPath, nullptr, nullptr, "VSMain", "vs_5_0",
  //                        D3DCOMPILE_DEBUG, 0, VSCode.GetAddressOf(),
  //                        Error.GetAddressOf())) != S_OK)
  //{
  //  OutputDebugString((CHAR *)Error->GetBufferPointer());
  //  throw "Not compiled shader!";
  //}

  //if ((D3DCompileFromFile(ShaderPath, nullptr, nullptr, "PSMain", "ps_5_0",
  //                        D3DCOMPILE_DEBUG, 0, PSCode.GetAddressOf(),
  //                        Error.GetAddressOf())) != S_OK)
  //{
  //  OutputDebugString((CHAR *)Error->GetBufferPointer());
  //  throw "Not compiled shader!";
  //}
  
  //Desc.PS.pShaderBytecode = (BYTE *)PSCode->GetBufferPointer();
  //Desc.VS.pShaderBytecode = (BYTE *)VSCode->GetBufferPointer();
  //Desc.PS.BytecodeLength = PSCode->GetBufferSize();
  //Desc.VS.BytecodeLength = VSCode->GetBufferSize();

  Desc.PS = Pixel.GetByteCode();
  Desc.VS = Vert.GetByteCode();
  //Desc.PS = CD3DX12_SHADER_BYTECODE(PSCode.Get());
  //Desc.VS = CD3DX12_SHADER_BYTECODE(VSCode.Get());

  // Create root signature
  D3D12_FEATURE_DATA_ROOT_SIGNATURE FeatureData = {};

  FeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

  if (FAILED(Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE,
                                         &FeatureData,
                                         sizeof(FeatureData))))
    FeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

  D3D12_DESCRIPTOR_RANGE1 ranges[1];
  ranges[0].BaseShaderRegister = 0;

  ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  ranges[0].NumDescriptors = 1;
  ranges[0].RegisterSpace = 0;
  ranges[0].OffsetInDescriptorsFromTableStart = 0;
  ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;

  D3D12_ROOT_PARAMETER1 RootParameters[1];
  RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

  RootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
  RootParameters[0].DescriptorTable.pDescriptorRanges = ranges;

  D3D12_STATIC_SAMPLER_DESC Samplers[1] {};
  Samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
  Samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
  Samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
  Samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
  Samplers[0].MipLODBias = 0;
  Samplers[0].MaxAnisotropy = 0;
  Samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
  Samplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
  Samplers[0].MinLOD = 0.0f;
  Samplers[0].MaxLOD = D3D12_FLOAT32_MAX;
  Samplers[0].ShaderRegister = 0;
  Samplers[0].RegisterSpace = 0;
  Samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

  D3D12_VERSIONED_ROOT_SIGNATURE_DESC RootSignatureDesc;
  RootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
  RootSignatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
  RootSignatureDesc.Desc_1_1.NumParameters = 1;
  RootSignatureDesc.Desc_1_1.pParameters = RootParameters;
  RootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
  RootSignatureDesc.Desc_1_1.pStaticSamplers = Samplers;

  Microsoft::WRL::ComPtr<ID3DBlob> signature {};
  Microsoft::WRL::ComPtr<ID3DBlob> error {};
  D3D12SerializeVersionedRootSignature(&RootSignatureDesc, &signature, &error);
  Device->CreateRootSignature(0, signature->GetBufferPointer(), 
    signature->GetBufferSize(), 
    IID_PPV_ARGS(Res.RootSignature.GetAddressOf()));
  
  Res.RootSignature->SetName(L"Hello Triangle Root Signature");
  if (error != nullptr)

  {
    CHAR *e = (CHAR *)error->GetBufferPointer();
    OutputDebugString(e);
  }

  //Res.RootSignature = rootSignatureDesc;
  Desc.pRootSignature = Res.RootSignature.Get();

  //std::vector<>
  if (PipeLineIndex == 1)
  {
    D3D12_INPUT_ELEMENT_DESC InputLayout[] = 
    {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };
    Desc.InputLayout = {InputLayout, _countof(InputLayout)};
    //Desc.InputLayout.pInputElementDescs = 
  }
  else
  {
    Desc.InputLayout.NumElements = 0;
    Desc.InputLayout.pInputElementDescs = nullptr;
  }
  //const WCHAR *ShaderPath = PipeLineIndex == 1 ? "BIN/SHADERS/Default/shader.hlsl" :

  Device->CreateGraphicsPipelineState(&Desc, IID_PPV_ARGS(Res.PipelineState.GetAddressOf()));
  
  return Res;
} /* End of 'ivdx::dx12::core::CreatePipeline' function */

/* Buffer creation function.
 * ARGUMENTS:
 *   - buffer size:
 *       UINT Size;
 *   - buffer data pointer:
 *       VOID *Data;
 *   - states of buffer:
 *       Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;
 * RETURNS: (Microsoft::WRL::ComPtr<ID3D12Resource>) pointer on created buffer.
 */
Microsoft::WRL::ComPtr<ID3D12Resource> ivdx::dx12::core::CreateBuffer( UINT Size, VOID *Data, 
                                                                       D3D12_RESOURCE_STATES State )
{
  D3D12_RESOURCE_DESC ResDesc = {};

  ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  ResDesc.Width = Size;
  ResDesc.Height = 1;
  ResDesc.DepthOrArraySize = 1;
  ResDesc.MipLevels = 1;
  ResDesc.SampleDesc.Count = 1;
  ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  ResDesc.Format = DXGI_FORMAT_UNKNOWN;

  D3D12_HEAP_PROPERTIES HeapProp = {};

  HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

  ID3D12Resource *Buf;

  Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES, &ResDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                  IID_PPV_ARGS(&Buf));

  VOID *InData;
  D3D12_RANGE Range = {0, Size};

  if (Data != nullptr)
  {
    Buf->Map(0, &Range, &InData);
    memcpy(InData, Data, Size);
    Buf->Unmap(0, &Range);
  }
  return Buf;
} /* End of 'vidx::dx12::core::CreateBuffer' function */

/* Class construcotr.
 * ARGUMENTS:
 *   - shader name:
 *       const std::wstring &SName;
 *   - entry point:
 *       const std::string &EntryPoint;
 *   - shader target:
 *       const std::string &STarget;
 *   - compile flags:
 *       const UINT &CompileFlags;
 */
ivdx::dx12::shader::shader( const std::wstring &SName, const std::string &EntryPoint,
                            const std::string &STarget, const UINT &CompileFlags )
{

  if ((D3DCompileFromFile(std::wstring(L"bin/shaders/Default/" + SName + L".hlsl").c_str(), nullptr, nullptr,
                     EntryPoint.c_str(), STarget.c_str(), CompileFlags, 0, &VSCode, &Errors)) != S_OK)
  {
    OutputDebugString((CHAR *)Errors->GetBufferPointer());
    throw "Not compiled shader!";
  }

  ByteCode.pShaderBytecode = VSCode->GetBufferPointer();
  ByteCode.BytecodeLength = VSCode->GetBufferSize();
} /* End of 'ivdx::dx12::shader::shader' function */

/* Texture resource creation function.
 * ARGUMENTS:
 *   - File name:
 *       std::string FileName;
 * RETURNS: NONE.
 */
VOID ivdx::dx12::core::LoadTexture( std::string FileName )
{
  std::string res = "bin/textures/" + FileName;
  std::ifstream F(res, std::ios::binary);
  INT W = 0, H = 0; 
  DWORD *mem = nullptr;
  //std::vector<UINT8> mem1;

  if (F.is_open())
  {
    F.read((CHAR *)&W, 2);
    F.read((CHAR *)&H, 2);
    mem = new DWORD[W * H];
    //mem1.resize(W * H);
    BYTE *row = new BYTE[(W + 1) * 4];
    for (INT y = 0; y < H; y++)
    {
      F.read((CHAR *)row, 3 * W);
      for (INT x = 0; x < W; x++)
      {
        //mem1[y * W + x] = (row[x * 3] << 16) | (row[x * 3 + 1] << 8) | row[x * 3 + 2] | 0xFF000000;
        mem[y * W + x] = (row[x * 3] << 16) | (row[x * 3 + 1] << 8) | row[x * 3 + 2] | 0xFF000000;
      }
    }
    delete[] row;
    F.close();
  }
  
  FLT mips;
    UINT PixelSize = 4;

  mips = (FLT)(W > H ? H : W);
  mips = (FLT)(log(mips) / log(2));
  if (mips < 1)
    mips = 1;

  // Describe and create a Texture2D.
  D3D12_RESOURCE_DESC TextureDesc = {};
  TextureDesc.Alignment = 0;
  TextureDesc.MipLevels = 1;
  TextureDesc.Width = TextureWidth;
  TextureDesc.Height = TextureHeight;
  TextureDesc.DepthOrArraySize = 1;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;
  TextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
  TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  TextureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  TextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

  D3D12_HEAP_PROPERTIES HeapProp = {};
  HeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
  HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  HeapProp.CreationNodeMask = 1;
  HeapProp.VisibleNodeMask = 1;

  Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_NONE, &TextureDesc,
                                  D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&TextureSample));

  // Create the GPU upload buffer
  auto Desc = TextureSample.Get()->GetDesc();
  UINT64 Size = 0;
  ID3D12Device *pDevice = nullptr;
  TextureSample.Get()->GetDevice(IID_ID3D12Device, (VOID **)&pDevice);
  pDevice->GetCopyableFootprints(&Desc, 0, 1, 0, nullptr, nullptr, nullptr, &Size);
  pDevice->Release();

  //const UINT64 uploadBufferSize = GetRequiredIntermediateSize(TextureSample.Get(), 0, 1);
  const UINT64 uploadBufferSize = Size; 

  HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

  // Describe upload heap
  D3D12_HEAP_PROPERTIES UpHeap = {};
  UpHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
  UpHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  UpHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  UpHeap.CreationNodeMask = 1;
  UpHeap.VisibleNodeMask = 1;
  
  // Describe upload buffer
  D3D12_RESOURCE_DESC UpBuf = {};
  UpBuf.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  UpBuf.Alignment = 0;
  UpBuf.Width = uploadBufferSize;
  UpBuf.Height = 1;
  UpBuf.DepthOrArraySize = 1;
  UpBuf.MipLevels = 1;
  UpBuf.Format = DXGI_FORMAT_UNKNOWN;
  UpBuf.SampleDesc.Count = 1;
  UpBuf.SampleDesc.Quality = 0;
  UpBuf.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  UpBuf.Flags = D3D12_RESOURCE_FLAG_NONE;

  CD3DX12_HEAP_PROPERTIES tmp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
  //ID3D12Resource *TextureBuffer;
  CD3DX12_RESOURCE_DESC tmp2 = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

  Device->CreateCommittedResource(&UpHeap, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
                                  &UpBuf, D3D12_RESOURCE_STATE_GENERIC_READ,
                                  nullptr,
                                  IID_PPV_ARGS(textureUploadHeap.GetAddressOf()));

  // Copy data to the intermediate upload heap and then schedule a copy 
  // from the upload heap to the Texture2D.
  std::vector<UINT8> texture = GenerateTextureData(TextureHeight, TextureWidth, TexturePixelSize);

  D3D12_SUBRESOURCE_DATA textureData = {};
  textureData.pData = &texture[0];
  textureData.RowPitch = TextureWidth * TexturePixelSize;
  textureData.SlicePitch = textureData.RowPitch * TextureHeight;

  CD3DX12_RESOURCE_BARRIER tmp3 = CD3DX12_RESOURCE_BARRIER::Transition(TextureSample.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

  UpdateSubresources(ComList.Get(), TextureSample.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);

  D3D12_RESOURCE_BARRIER TexBar;
  TexBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  TexBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  TexBar.Transition.pResource = TextureSample.Get();
  TexBar.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
  TexBar.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
  TexBar.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  ComList->ResourceBarrier(1, &TexBar);

  // Describe and create a SRV for the texture.
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc {};
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvDesc.Format = TextureDesc.Format;
  srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  Device->CreateShaderResourceView(TextureSample.Get(), &srvDesc, SRVHeap->GetCPUDescriptorHandleForHeapStart());

  // Create a constant buffer view.
  const UINT constantBufferSize = sizeof(SceneConstantBuffer);
  CD3DX12_RESOURCE_DESC Buf = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
  Device->CreateCommittedResource(&UpHeap, D3D12_HEAP_FLAG_NONE, &Buf, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&CBuffer));
  D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
  cbvDesc.BufferLocation = CBuffer->GetGPUVirtualAddress();
  cbvDesc.SizeInBytes = constantBufferSize;
  Device->CreateConstantBufferView(&cbvDesc, CBVHeap->GetCPUDescriptorHandleForHeapStart());
  
  D3D12_RANGE readRange1;
  readRange1.Begin = 0;
  readRange1.End = 0;
  CBuffer->Map(0, &readRange1, (VOID **)&CBVDataBegin);
  memcpy(CBVDataBegin, &CBufferData, sizeof(CBufferData));

  if (mem != nullptr)
    delete[] mem;
} /* End of 'ivdx::dx12::texture::texture' function */

/* Create texture.
 * ARGUMENTS: None.
 * RETURNS: std::vector<UINT8> data.
 */
std::vector<UINT8> ivdx::dx12::core::GenerateTextureData( INT TextureWidth,
  INT TextureHeight, INT TexturePixelSize )
{
  const UINT rowPitch = TextureWidth * TexturePixelSize;
  const UINT cellPitch = rowPitch >> 3;
  const UINT cellHeight = TextureWidth >> 3;
  const UINT textureSize = rowPitch * TextureHeight;

  std::vector<UINT8> data(textureSize);
  UINT8* Data = &data[0];

  for (UINT n = 0; n < textureSize; n += TexturePixelSize)
  {
    UINT i = n % rowPitch / cellPitch;
    UINT j = n / rowPitch / cellHeight;

    if (i % 2 == j % 2)
    {
      Data[n] = 0;      
      Data[n + 1] = 0;  
      Data[n + 2] = 0;  
      Data[n + 3] = 255;
    }
    else
    {
      Data[n] = 255;    
      Data[n + 1] = 255;
      Data[n + 2] = 0;
      Data[n + 3] = 255;
    }
  }

  return data;
} /* End of 'ivdx::core::GenerateTextureData' function */

/* Synchronize CPU and GPU function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ivdx::dx12::core::Synchronize( VOID )
{
  FenceValue++;
  ComQueue->Signal(Fence.Get(), FenceValue);
  Fence->SetEventOnCompletion(FenceValue, Event);
  WaitForSingleObject(Event, INFINITE);
} /* End of 'ivdx::dx12::core::Synchronize' function */

/* END OF 'dx_12_init.cpp' FILE */