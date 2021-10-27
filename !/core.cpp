/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : core.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 16.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>

#include "../../../bodx.h"

bodx::dx12::core * bodx::dx12::coreref::D3D;

/* DirectX 12 core type constructor.
 * ARGUMENTS:
 *   - window handle reference:
 *       const HWND &hWnd;
 *   - window size reference:
 *       const INT &W, &H;
 */
bodx::dx12::core::core( const HWND &hWnd, const INT &W, const INT &H ) : Width(W), Height(H), FrameIndex(0)
{
  CreateDXGIFactory(IID_PPV_ARGS(Factory.GetAddressOf()));

#ifdef _DEBUG
  D3D12GetDebugInterface(IID_PPV_ARGS(Debug.GetAddressOf()));
  Debug->EnableDebugLayer();
#endif /* _DEBUG */

  // Find best adapter
  mswrl::ComPtr<IDXGIAdapter> TmpAdapter, BestAdapter = {};
  DXGI_ADAPTER_DESC AdapterDesc;
  DXGI_ADAPTER_DESC BestAdapterDesc = {};
  int i = 0;

  while (Factory->EnumAdapters(i++, TmpAdapter.GetAddressOf()) == S_OK)
  {
    TmpAdapter->GetDesc(&AdapterDesc);

    if (BestAdapter == nullptr)
    {
      BestAdapter = TmpAdapter;
      BestAdapterDesc = AdapterDesc;
    }
    else if (AdapterDesc.DedicatedVideoMemory > BestAdapterDesc.DedicatedVideoMemory)
    {
      BestAdapterDesc = AdapterDesc;
      BestAdapter.Reset();
      BestAdapter = TmpAdapter;
    }
    TmpAdapter.Reset();
  }
 

  // Create Device
  if(FAILED(D3D12CreateDevice(BestAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Device.GetAddressOf()))))
  {
    mswrl::ComPtr<IDXGIAdapter> WarpAdapter;
    Factory->EnumWarpAdapter(IID_PPV_ARGS(WarpAdapter.GetAddressOf()));
    D3D12CreateDevice(WarpAdapter.Get(),  D3D_FEATURE_LEVEL_12_0,  IID_PPV_ARGS(Device.GetAddressOf()));
  }
  else
  {
    // Obtain feature level
    D3D12_FEATURE_DATA_FEATURE_LEVELS FeatureLevelsInfo;
    D3D_FEATURE_LEVEL FeatureLevels[4] =
    {
      D3D_FEATURE_LEVEL_12_0,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3
    };
    
    FeatureLevelsInfo.NumFeatureLevels = 4;
    FeatureLevelsInfo.pFeatureLevelsRequested = FeatureLevels;
    Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS,
                                &FeatureLevelsInfo,
                                sizeof(FeatureLevelsInfo));
    Device.Reset();
    D3D12CreateDevice(BestAdapter.Get(), FeatureLevelsInfo.MaxSupportedFeatureLevel, IID_PPV_ARGS(Device.GetAddressOf()));
  }
 
  // Create root signature
  D3D12_FEATURE_DATA_ROOT_SIGNATURE FeatureData = {};

  FeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

  if (FAILED(Device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE,
                                         &FeatureData,
                                         sizeof(FeatureData))))
    FeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

  // Create_D3D_Command_Queue_List_Allocator
  D3D12_COMMAND_QUEUE_DESC CQDesc {};
  CQDesc.NodeMask = 0;
  CQDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  CQDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  CQDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  Device->CreateCommandQueue(&CQDesc, IID_PPV_ARGS(&ComQueue));

  // Create_Syncronisation_Fence
  Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));

  // Create_DXGI_Swap_Chain
  DXGI_SWAP_CHAIN_DESC1 SCD {};
  SCD.Width = Width;
  SCD.Height = Height;
  SCD.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
  SCD.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  SCD.Scaling = DXGI_SCALING_STRETCH;
  SCD.SampleDesc.Count = 1;
  SCD.SampleDesc.Quality = 0;
  SCD.BufferCount = NoofBuffers;
  SCD.Stereo = FALSE;
  SCD.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SCD.Flags = 0;

  
  IDXGISwapChain1 *SwapChain1 {};
  Factory->CreateSwapChainForHwnd(ComQueue.Get(), hWnd, &SCD, nullptr, nullptr, &SwapChain1);

  SwapChain1->QueryInterface(IID_PPV_ARGS(&SwapChain));
  SwapChain1->Release();

  // Create_Heap_Descriptors
  // Obtain heap descriptors
  D3D12_DESCRIPTOR_HEAP_DESC DHD {};
  DHD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  DHD.NodeMask = 0;
  DHD.NumDescriptors = NoofBuffers /* frame buffers */ + 1 /* render target */;
  DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  Device->CreateDescriptorHeap(&DHD, IID_PPV_ARGS(&RTVHeap));

  // Depth-stencil
  DHD.NumDescriptors = 1 /* depth buffer */ + 
                       1 /* stencil buffer */;
  DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
  Device->CreateDescriptorHeap(&DHD, IID_PPV_ARGS(&DSVHeap));

  // Constant/shaders
  DHD.NumDescriptors = 1;
  DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  DHD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  Device->CreateDescriptorHeap(&DHD, IID_PPV_ARGS(&SRVHeap));

  // Create viewport and render rectangle
  ScissorRect.left = 0;
  ScissorRect.top = 0;
  ScissorRect.right = static_cast<LONG>(Width);
  ScissorRect.bottom = static_cast<LONG>(Height);

  ScreenViewport.TopLeftX = 0.0f;
  ScreenViewport.TopLeftY = 0.0f;
  ScreenViewport.Width = static_cast<float>(Width);
  ScreenViewport.Height = static_cast<float>(Height);
  ScreenViewport.MinDepth = 0.1f;
  ScreenViewport.MaxDepth = 1000.f; 

  Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&ComAllocator));

  
  coreref::D3D = this;

  //Resize();
  ResourcesInit();
} /* End of 'bodx::dx12::core::core' constructor */

/* D3D12 window resize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::Resize( VOID )
{
  for (UINT i = 0; i < NoofBuffers; i++)
    if (BackBuffers[i])
      BackBuffers[i].Reset();

  // Resize buffers
  SwapChain->ResizeBuffers(NoofBuffers, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

  for (UINT i = 0; i < NoofBuffers; i++)
  {
    D3D12_CPU_DESCRIPTOR_HANDLE CDH;

    CDH.ptr =
      RTVHeap->GetCPUDescriptorHandleForHeapStart().ptr +
      i * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    SwapChain->GetBuffer(i, IID_PPV_ARGS(&BackBuffers[i]));
    Device->CreateRenderTargetView(BackBuffers[i].Get(), nullptr, CDH);
  }
} /* End of 'bodx::dx12::core::Resize' function */

/* Synchroize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::WaitForPreviousFrame( VOID )
{
  //Signal and increment the fence value.
  const UINT64 fence = FenceValue;
  ComQueue->Signal(Fence.Get(), fence);
  FenceValue++;

  // Wait until the previous frame is finished.
  if (Fence->GetCompletedValue() < fence)
  {
    Fence->SetEventOnCompletion(fence, FenceEvent);
    WaitForSingleObject(FenceEvent, INFINITE);
  }

  FrameIndex = SwapChain->GetCurrentBackBufferIndex();
} /* End of 'bodx::dx12::core::WaitForPreviousFrame' function */

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
bodx::dx12::shader::shader( const std::wstring &SName, const std::string &EntryPoint,
                            const std::string &STarget, const UINT &CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION )
{
  if (FAILED(D3DCompileFromFile(std::wstring(L"bin/shaders/" + SName + L".hlsl").c_str(), nullptr, nullptr,
                                EntryPoint.c_str(), STarget.c_str(), CompileFlags, 0, &Shader, &errors)))
    throw std::exception(std::string("Shader was not compiled").c_str());

  ByteCode.pShaderBytecode = Shader->GetBufferPointer();
  ByteCode.BytecodeLength = Shader->GetBufferSize();
} /* End of 'bodx::dx12::shader::shader' function */

/* Get byte code function.
 * ARGUMENTS: None;
 * RETURNS: (D3D12_SHADER_BYTECODE) shader byte code.
 */
D3D12_SHADER_BYTECODE & bodx::dx12::shader::GetByteCode( VOID )
{
  return ByteCode;
} /* End of 'bodx::dx12::shader::GetByteCode' function */

/* Class construcotr.
 * ARGUMENTS:
 *   - texture name:
 *       const std::swtring &TName;
 */
bodx::dx12::texture::texture( const std::wstring &TName )
{
  load(TName);
} /* End of 'bodx::dx12::texture::texture' function */

/* Load texture function.
 * ARGUMENTS:
 *   - texture name:
 *       const std::wstring &FileName;
 */
VOID bodx::dx12::texture::load( const std::wstring &TName )
{
  WORD w = 0, h = 0;
  reader rd(L"bin/images/" + TName);
  if (!rd.is_open())
  {
    throw std::exception(std::string("Image file" + std::string((CHAR *)TName.c_str()) + "was not opened").c_str());
  }
  rd(&w);
  rd(&h);
  W = w;
  H = h;

  BYTE *Bits = new BYTE[W * H * 4];

  if (rd.length - 4 == W * H * 4)
    rd(Bits, W * H * 4);
  else
    for (INT i = 0; i < W * H; i++)
    {  
      Bits[i] = (BYTE)0xFF000000;
      for (INT c = 1; c < 4; c++)
      {
        BYTE d = 0;
        rd(&d);
        Bits[i + c] = d;
      }
    }

  mswrl::ComPtr<ID3D12Resource> TextureUploadHeap;

  // Create the texture.
  {
    FLT mips;

    mips = (FLT)(W > H ? H : W);
    mips = (FLT)(log(mips) / log(2));
    if (mips < 1)
      mips = 1;

    // Describe and create a Texture2D.
    D3D12_RESOURCE_DESC TextureDesc = {};
    TextureDesc.MipLevels = 1;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.Width = W;
    TextureDesc.Height = H;
    TextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    TextureDesc.DepthOrArraySize = 1;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    D3D12_HEAP_PROPERTIES HeapProps {};
    HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProps.CreationNodeMask = 1;
    HeapProps.VisibleNodeMask = 1;

    D3D->Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &TextureDesc,
                                         D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&Texture));


    HeapProps = {};
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC BufferDesc = {};
    BufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    BufferDesc.Alignment = 0;
    BufferDesc.Width = W * H * 4;
    BufferDesc.Height = 1;
    BufferDesc.DepthOrArraySize = 1;
    BufferDesc.MipLevels = 1;
    BufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    BufferDesc.SampleDesc.Count = 1;
    BufferDesc.SampleDesc.Quality = 0;
    BufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    BufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    // Create the GPU upload buffer.
    D3D->Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
                                         &BufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
                                         nullptr,
                                         IID_PPV_ARGS(&TextureUploadHeap));

    BYTE *pData;

    TextureUploadHeap->Map(0, nullptr, reinterpret_cast<VOID **>(&pData));
    memcpy(pData, Bits, W * H * 4);
    TextureUploadHeap->Unmap(0, nullptr);

    // Copy data from buffer to texture
    D3D12_BOX TextureBox { 0, 0, 0, (UINT)W, (UINT)H, 1 };
    D3D12_TEXTURE_COPY_LOCATION SourceTCL {}, DestTCL {};

    SourceTCL.pResource = TextureUploadHeap.Get();
    SourceTCL.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    SourceTCL.PlacedFootprint.Offset = 0;
    SourceTCL.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SourceTCL.PlacedFootprint.Footprint.Depth = 1;
    SourceTCL.PlacedFootprint.Footprint.Width = W;
    SourceTCL.PlacedFootprint.Footprint.Height = H;
    SourceTCL.PlacedFootprint.Footprint.RowPitch = W * 4;

    DestTCL.pResource = Texture.Get();
    DestTCL.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    DestTCL.SubresourceIndex = 0;

    D3D->ComList->CopyTextureRegion(&DestTCL, 0, 0, 0, &SourceTCL, &TextureBox);

    //D3D->ComList->CopyBufferRegion(Texture.Get(), 0, TextureUploadHeap.Get(), 0, W); 

    D3D12_RESOURCE_BARRIER barrier {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = Texture.Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    D3D->ComList->ResourceBarrier(1, &barrier);

    // Describe and create a SRV for the texture.
    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    SRVDesc.Format = TextureDesc.Format;
    SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;
    D3D->Device->CreateShaderResourceView(Texture.Get(), &SRVDesc, D3D->SRVHeap->GetCPUDescriptorHandleForHeapStart());
  }

  if (Bits != nullptr)
    delete[] Bits;

  D3D->ComList->Close();
  ID3D12CommandList *ppCommandLists[] = { D3D->ComList.Get() };
  D3D->ComQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  D3D->Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&D3D->Fence));
  D3D->FenceValue++;

  D3D->FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

  D3D->WaitForPreviousFrame();
} /* End of 'bodx::dx12::texture::load' function */

/* D3D12 window resize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::ResourcesInit( VOID )
{
  {
    D3D12_DESCRIPTOR_RANGE1 ranges[1];
    ranges[0].BaseShaderRegister = 0;

    ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    ranges[0].NumDescriptors = 1;
    ranges[0].RegisterSpace = 0;
    ranges[0].OffsetInDescriptorsFromTableStart = 0;
    ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;

    D3D12_ROOT_PARAMETER1 RootParameters[1];
    RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    RootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
    RootParameters[0].DescriptorTable.pDescriptorRanges = &ranges[0]; 

    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC RootSignatureDesc;
    RootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
    RootSignatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    RootSignatureDesc.Desc_1_1.NumParameters = 1;
    RootSignatureDesc.Desc_1_1.pParameters = RootParameters;
    RootSignatureDesc.Desc_1_1.NumStaticSamplers = 1;
    RootSignatureDesc.Desc_1_1.pStaticSamplers = &sampler;

    mswrl::ComPtr<ID3DBlob> signature {};
    mswrl::ComPtr<ID3DBlob> error {};
    D3D12SerializeVersionedRootSignature(&RootSignatureDesc, &signature, &error);
    Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                IID_PPV_ARGS(&RootSignature));
    RootSignature->SetName(L"Texture Triangle Root Signature");
  }

  {
#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    shader VertexShader(L"default", "VSMain", "vs_5_0");
    shader PixelShader(L"default", "PSMain", "ps_5_0");

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Describe and create the graphics pipeline state object (PSO).

    D3D12_RASTERIZER_DESC RasterDesc {};
    RasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
    RasterDesc.CullMode = D3D12_CULL_MODE_NONE;
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
    const D3D12_RENDER_TARGET_BLEND_DESC DefaultRenderTargetBlendDesc = {
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
      BlendDesc.RenderTarget[i] = DefaultRenderTargetBlendDesc;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC PSODesc = {};
    PSODesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    PSODesc.pRootSignature = RootSignature.Get();
    PSODesc.VS = VertexShader.GetByteCode();
    PSODesc.PS = PixelShader.GetByteCode();
    PSODesc.RasterizerState = RasterDesc;
    PSODesc.BlendState = BlendDesc;
    PSODesc.DepthStencilState.DepthEnable = FALSE;
    PSODesc.DepthStencilState.StencilEnable = FALSE;
    PSODesc.SampleMask = UINT_MAX;
    PSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    PSODesc.NumRenderTargets = 1;
    PSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PSODesc.SampleDesc.Count = 1;
    Device->CreateGraphicsPipelineState(&PSODesc, IID_PPV_ARGS(&PipelineState));
  }

  Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, ComAllocator.Get(), nullptr, IID_PPV_ARGS(&ComList));

  {
    const UINT vertexBufferSize = sizeof(VertexBufferData);

    D3D12_HEAP_PROPERTIES HeapProps {};
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC VertexBufferResourceDesc;
    VertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    VertexBufferResourceDesc.Alignment = 0;
    VertexBufferResourceDesc.Width = vertexBufferSize;
    VertexBufferResourceDesc.Height = 1;
    VertexBufferResourceDesc.DepthOrArraySize = 1;
    VertexBufferResourceDesc.MipLevels = 1;
    VertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    VertexBufferResourceDesc.SampleDesc.Count = 1;
    VertexBufferResourceDesc.SampleDesc.Quality = 0;
    VertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    VertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &VertexBufferResourceDesc,
                                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                    IID_PPV_ARGS(&VertexBuffer));

    UINT8 *VertexDataBegin;

    D3D12_RANGE ReadRange;
    ReadRange.Begin = 0;
    ReadRange.End = 0;

    VertexBuffer->Map(0, &ReadRange, reinterpret_cast<VOID **>(&VertexDataBegin));
    memcpy(VertexDataBegin, VertexBufferData, sizeof(VertexBufferData));
    VertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
    VertexBufferView.StrideInBytes = sizeof(VERTEX);
    VertexBufferView.SizeInBytes = vertexBufferSize;
  }

  {
    const UINT IndexBufferSize = sizeof(IndexBufferData);

    D3D12_HEAP_PROPERTIES HeapProps;
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProps.CreationNodeMask = 1;
    HeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC VertexBufferResourceDesc;
    VertexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    VertexBufferResourceDesc.Alignment = 0;
    VertexBufferResourceDesc.Width = IndexBufferSize;
    VertexBufferResourceDesc.Height = 1;
    VertexBufferResourceDesc.DepthOrArraySize = 1;
    VertexBufferResourceDesc.MipLevels = 1;
    VertexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    VertexBufferResourceDesc.SampleDesc.Count = 1;
    VertexBufferResourceDesc.SampleDesc.Quality = 0;
    VertexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    VertexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &VertexBufferResourceDesc,
                                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                    IID_PPV_ARGS(&IndexBuffer));

    UINT8 *VertexDataBegin;

    D3D12_RANGE ReadRange;
    ReadRange.Begin = 0;
    ReadRange.End = 0;

    IndexBuffer->Map(0, &ReadRange, reinterpret_cast<VOID **>(&VertexDataBegin));
    memcpy(VertexDataBegin, IndexBufferData, sizeof(IndexBufferData));
    IndexBuffer->Unmap(0, nullptr);

    IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
    IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
    IndexBufferView.SizeInBytes = IndexBufferSize;
  }

  Texture.load(L"grass.g32");
} /* End of 'bodx::dx12::core::ResourcesInit' function */

/* DirectX 12 core type destructor.
 * ARGUMENTS: None.
 */
bodx::dx12::core::~core( VOID )
{
  WaitForPreviousFrame();

  CloseHandle(FenceEvent);
} /* End of 'bods::dx12::core::~core' function */

/* Rendring function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::Render( VOID )
{
  ComAllocator->Reset();

  ComList->Reset(ComAllocator.Get(), PipelineState.Get());

  // Set necessary state.
  ComList->SetGraphicsRootSignature(RootSignature.Get());

  ComList->SetDescriptorHeaps(1, SRVHeap.GetAddressOf());
  ComList->SetGraphicsRootDescriptorTable(0, SRVHeap->GetGPUDescriptorHandleForHeapStart());

  ComList->RSSetViewports(1, &ScreenViewport);
  ComList->RSSetScissorRects(1, &ScissorRect);

  // Indicate that the back buffer will be used as a render target.
  D3D12_RESOURCE_BARRIER RenderTargetBarrier;
  RenderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  RenderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  RenderTargetBarrier.Transition.pResource = BackBuffers[FrameIndex].Get();
  RenderTargetBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
  RenderTargetBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
  RenderTargetBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

  ComList->ResourceBarrier(1, &RenderTargetBarrier);

  D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle;
  RTVHandle.ptr = RTVHeap->GetCPUDescriptorHandleForHeapStart().ptr +
    (FrameIndex * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
  ComList->OMSetRenderTargets(1, &RTVHandle, FALSE, nullptr);

  // Record commands.
  const float ClearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  ComList->ClearRenderTargetView(RTVHandle, ClearColor, 0, nullptr);
  ComList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  ComList->IASetVertexBuffers(0, 1, &VertexBufferView);
  ComList->IASetIndexBuffer(&IndexBufferView);

  ComList->DrawIndexedInstanced(3, 1, 0, 0, 0);

  // Indicate that the back buffer will now be used to present.
  D3D12_RESOURCE_BARRIER PresentBarrier;
  PresentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  PresentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  PresentBarrier.Transition.pResource = BackBuffers[FrameIndex].Get();
  PresentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
  PresentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
  PresentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

  ComList->ResourceBarrier(1, &PresentBarrier);

  ComList->Close();

  ID3D12CommandList *ppCommandLists[] = { ComList.Get() };
  ComQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  SwapChain->Present(1, 0);

  WaitForPreviousFrame();
} /* End of 'bodx::dx12::core::Render' function */

/* END OF 'core.cpp' FILE */