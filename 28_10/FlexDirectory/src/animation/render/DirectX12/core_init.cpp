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
 * LAST UPDATE : 25.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>

#include "../../../bodx.h"

bodx::dx12::core * bodx::dx12::core_ref::D3D;

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
  DHD.NumDescriptors = 2;
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

  
  core_ref::D3D = this;
  // Create new depth  buffer and render target
  DepthBuffer = core_interface::CreateDepthBuffer(W, H);
  RenderTarget = core_interface::CreateRenderTarget(W, H);

  Resize();
  ResourcesInit();
} /* End of 'bodx::dx12::core::core' constructor */

/* D3D12 window resize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::Resize( VOID )
{

  if (Width * Height == 0)
    return;

  WaitForPreviousFrame();

  // Remove old buffer interfaces
  for (UINT i = 0; i < NoofBuffers; i++)
    BackBuffers[i].Reset();

  // Remove old depth buffer and render target
  //SafeRelease(DepthBuffer);
  //SafeRelease(RenderTarget);

  // Resize and recreate buffer interfaces
  SwapChain->ResizeBuffers(NoofBuffers, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

  for (UINT i = 0; i < NoofBuffers; i++)
  {
    SwapChain->GetBuffer(i, IID_PPV_ARGS(&BackBuffers[i]));

    D3D12_CPU_DESCRIPTOR_HANDLE CDH;

    CDH = RTVHeap->GetCPUDescriptorHandleForHeapStart();
    CDH.ptr += i * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

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

/* Resource inition function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::ResourcesInit( VOID )
{
  {
    D3D12_DESCRIPTOR_RANGE1 ranges[2];

    ranges[0].BaseShaderRegister = 0;
    ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    ranges[0].NumDescriptors = 1;
    ranges[0].RegisterSpace = 0;
    ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
    
    ranges[1].BaseShaderRegister = 0;
    ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    ranges[1].NumDescriptors = 1;
    ranges[1].RegisterSpace = 0;
    ranges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    ranges[1].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;

    D3D12_ROOT_PARAMETER1 RootParameters[1];

    RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    RootParameters[0].DescriptorTable.NumDescriptorRanges = 2;
    RootParameters[0].DescriptorTable.pDescriptorRanges = ranges;

    // Allow input layout and deny uneccessary access to certain pipeline stages.
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

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
    RootSignatureDesc.Desc_1_1.Flags = rootSignatureFlags;
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
    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Describe and create the graphics pipeline state object (PSO).
    PipelineState = core_interface::CreatePipeLine(inputElementDescs, _countof(inputElementDescs),
                                                   D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, L"default");
  }

  Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, ComAllocator.Get(), nullptr, IID_PPV_ARGS(&ComList));

  std::vector<VERTEX> VBuffer;
  std::vector<INT> IBuffer;
  vec3 Min(-0.4), Max(0.4);
  VERTEX v;
  VBuffer.reserve(24);
  IBuffer.reserve(36);

  v.position = vec3(Min[0], Max[1], Max[2]);
  v.texcoord = vec2(0, 0);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Max[1], Min[2]);
  v.texcoord = vec2(1, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Max[1], Min[2]);
  v.texcoord = vec2(0, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Max[1], Max[2]);
  v.texcoord = vec2(1, 0);
  VBuffer.emplace_back(v);

  IBuffer.emplace_back(0);
  IBuffer.emplace_back(1);
  IBuffer.emplace_back(2);
  IBuffer.emplace_back(0);
  IBuffer.emplace_back(3);
  IBuffer.emplace_back(1);

  v.position = vec3(Max[0], Min[1], Max[2]);
  v.texcoord = vec2(0, 0);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Max[1], Max[2]);
  v.texcoord = vec2(1, 1);
  VBuffer.emplace_back(v);
  v.texcoord = vec2(0, 1);
  v.position = vec3(Min[0], Min[1], Max[2]);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Max[1], Max[2]);
  v.texcoord = vec2(1, 0);
  VBuffer.emplace_back(v);

  IBuffer.emplace_back(4);
  IBuffer.emplace_back(5);
  IBuffer.emplace_back(6);
  IBuffer.emplace_back(4);
  IBuffer.emplace_back(7);
  IBuffer.emplace_back(5);

  v.position = vec3(Max[0], Max[1], Min[2]);
  v.texcoord = vec2(0, 0);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Min[1], Max[2]);
  v.texcoord = vec2(1, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Min[1], Min[2]);
  v.texcoord = vec2(0, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Max[1], Max[2]);
  v.texcoord = vec2(1, 0);
  VBuffer.emplace_back(v);

  IBuffer.emplace_back(8);
  IBuffer.emplace_back(9);
  IBuffer.emplace_back(10);
  IBuffer.emplace_back(8);
  IBuffer.emplace_back(11);
  IBuffer.emplace_back(9);

  v.position = vec3(Max[0], Max[1], Min[2]);
  v.texcoord = vec2(0, 0);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Min[1], Min[2]);
  v.texcoord = vec2(1, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Max[1], Min[2]);
  v.texcoord = vec2(0, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Min[1], Min[2]);
  v.texcoord = vec2(1, 0);
  VBuffer.emplace_back(v);

  IBuffer.emplace_back(12);
  IBuffer.emplace_back(13);
  IBuffer.emplace_back(14);
  IBuffer.emplace_back(12);
  IBuffer.emplace_back(15);
  IBuffer.emplace_back(13);

  v.position = vec3(Min[0], Min[1], Min[2]);
  v.texcoord = vec2(0, 0);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Max[1], Max[2]);
  v.texcoord = vec2(1, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Max[1], Min[2]);
  v.texcoord = vec2(0, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Min[1], Max[2]);
  v.texcoord = vec2(1, 0);
  VBuffer.emplace_back(v);

  IBuffer.emplace_back(16);
  IBuffer.emplace_back(17);
  IBuffer.emplace_back(18);
  IBuffer.emplace_back(16);
  IBuffer.emplace_back(19);
  IBuffer.emplace_back(17);

  v.position = vec3(Min[0], Min[1], Min[2]);
  v.texcoord = vec2(0, 0);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Min[1], Max[2]);
  v.texcoord = vec2(1, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Min[0], Min[1], Max[2]);
  v.texcoord = vec2(0, 1);
  VBuffer.emplace_back(v);
  v.position = vec3(Max[0], Min[1], Min[2]);
  v.texcoord = vec2(1, 0);
  VBuffer.emplace_back(v);

  IBuffer.emplace_back(20);
  IBuffer.emplace_back(21);
  IBuffer.emplace_back(22);
  IBuffer.emplace_back(20);
  IBuffer.emplace_back(23);
  IBuffer.emplace_back(21);

  {
    const UINT vertexBufferSize = (UINT)(sizeof(VERTEX) * VBuffer.size());

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
    memcpy(VertexDataBegin, VBuffer.data(), sizeof(VERTEX) * VBuffer.size());
    VertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
    VertexBufferView.StrideInBytes = sizeof(VERTEX);
    VertexBufferView.SizeInBytes = vertexBufferSize;
  }

  {
    const UINT IndexBufferSize = (UINT)(sizeof(INT) * IBuffer.size());

    D3D12_HEAP_PROPERTIES HeapProps;
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProps.CreationNodeMask = 1;
    HeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC IndexBufferResourceDesc;
    IndexBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    IndexBufferResourceDesc.Alignment = 0;
    IndexBufferResourceDesc.Width = IndexBufferSize;
    IndexBufferResourceDesc.Height = 1;
    IndexBufferResourceDesc.DepthOrArraySize = 1;
    IndexBufferResourceDesc.MipLevels = 1;
    IndexBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    IndexBufferResourceDesc.SampleDesc.Count = 1;
    IndexBufferResourceDesc.SampleDesc.Quality = 0;
    IndexBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    IndexBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &IndexBufferResourceDesc,
                                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                    IID_PPV_ARGS(&IndexBuffer));

    UINT8 *VertexDataBegin;

    D3D12_RANGE ReadRange;
    ReadRange.Begin = 0;
    ReadRange.End = 0;

    IndexBuffer->Map(0, &ReadRange, reinterpret_cast<VOID **>(&VertexDataBegin));
    memcpy(VertexDataBegin, IBuffer.data(), sizeof(INT) * IBuffer.size());
    IndexBuffer->Unmap(0, nullptr);

    IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
    IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
    IndexBufferView.SizeInBytes = IndexBufferSize;
  }

  Texture.load(L"bogdan.g32").create().free();

  // Create the constant buffer.
  {
    const UINT ConstantBufferSize = sizeof(ConstantBufferData);

    D3D12_HEAP_PROPERTIES HeapProps;
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProps.CreationNodeMask = 1;
    HeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC ConstBufferResourceDesc;
    ConstBufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ConstBufferResourceDesc.Alignment = 0;
    ConstBufferResourceDesc.Width = ConstantBufferSize;
    ConstBufferResourceDesc.Height = 1;
    ConstBufferResourceDesc.DepthOrArraySize = 1;
    ConstBufferResourceDesc.MipLevels = 1;
    ConstBufferResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    ConstBufferResourceDesc.SampleDesc.Count = 1;
    ConstBufferResourceDesc.SampleDesc.Quality = 0;
    ConstBufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    ConstBufferResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE,
                                    &ConstBufferResourceDesc,
                                    D3D12_RESOURCE_STATE_GENERIC_READ,
                                    nullptr,
                                    IID_PPV_ARGS(&ConstantBuffer));

    // Describe and create a constant buffer view.
    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
    CBVDesc.BufferLocation = ConstantBuffer->GetGPUVirtualAddress();
    CBVDesc.SizeInBytes = ConstantBufferSize;

    D3D12_CPU_DESCRIPTOR_HANDLE CBVHandle = SRVHeap->GetCPUDescriptorHandleForHeapStart();
    CBVHandle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    Device->CreateConstantBufferView(&CBVDesc, CBVHandle);


    D3D12_RANGE ReadRange;
    ReadRange.Begin = 0;
    ReadRange.End = 0;
    ConstantBuffer->Map(0, &ReadRange, reinterpret_cast<VOID **>(&CBVDataBegin));
    memcpy(CBVDataBegin, &ConstantBufferData, ConstantBufferSize);
  }

  // Create synchronization objects and wait until assets have been uploaded to the GPU.
  {
    Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));
    FenceValue = 1;

    // Create an event handle to use for frame synchronization.
    FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (FenceEvent == nullptr)
    {
      HRESULT_FROM_WIN32(GetLastError());
    }

    // Wait for the command list to execute; we are reusing the same command 
    // list in our main loop but for now, we just want to wait for setup to 
    // complete before continuing.
    WaitForPreviousFrame();
  }
} /* End of 'bodx::dx12::core::ResourcesInit' function */



/* DirectX 12 core type destructor.
 * ARGUMENTS: None.
 */
bodx::dx12::core::~core( VOID )
{
  ConstantBuffer->Unmap(0, nullptr);
} /* End of 'bods::dx12::core::~core' function */

/* END OF 'core.cpp' FILE */