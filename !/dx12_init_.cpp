/***************************************************************
 * Copyright (C) 2020-2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : dx12.cpp
  * PURPOSE     : T51DX project.
  *               Direct X 12 initialization module.
  * PROGRAMMER  : VB5.
  * LAST UPDATE : 19.10.2021
  * NOTE        : None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <vldx.h>
#include "../../vldx.h"

#include "dx12.h"

/* Pointer to Direct3D core */
vldx::core *vldx::coreref::D3D;

/* core class constructor.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
vldx::core::core( HWND &hWnd ) : hWnd(hWnd)
{
  // Retrive window size data
  RECT rect;
  GetWindowRect(hWnd, &rect);
  Width = rect.right - rect.left;
  Height = rect.bottom - rect.top;

  // Set ViewPort
  FrameIndex = 0;
  ViewPort.TopLeftX = 0.0f;
  ViewPort.TopLeftY = 0.0f;
  ViewPort.Width = Width;
  ViewPort.Height = Height;

  ScissorRect.left = 0;
  ScissorRect.top = 0;
  ScissorRect.right = Width;
  ScissorRect.bottom = Height;
  RTVDescriptorSize = 0;
#if defined(_DEBUG)
   Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
   D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
   debugController->EnableDebugLayer();
#endif
  
  // Create Factory
  CreateDXGIFactory(IID_PPV_ARGS(&Factory));
  
  // Create Device
  HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device));
  if (FAILED(hardwareResult))
  {
    Microsoft::WRL::ComPtr<IDXGIAdapter> pWarpAdapter;
    Factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));
    D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device));
  }
  
  // Create the command queue
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&CommandQueue));

  // Create the swap chain
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = FrameCount;
  swapChainDesc.Width = Width;
  swapChainDesc.Height = Height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
  Factory->CreateSwapChainForHwnd(CommandQueue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &swapChain);

  swapChain.As(&SwapChain);
  FrameIndex = SwapChain->GetCurrentBackBufferIndex();

  // Create rtv heap
  D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
  rtvHeapDesc.NumDescriptors = FrameCount;
  rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&RTVHeap));
  RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  
  // Create srv heap
  D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
  srvHeapDesc.NumDescriptors = 1;
  srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&SRVHeap));

  // Create cbv heap
  D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
  cbvHeapDesc.NumDescriptors = 1;
  cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  Device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&CBVHeap));

  // Create frame resources
  D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
  for (UINT n = 0; n < FrameCount; n++)
  {
    SwapChain->GetBuffer(n, IID_PPV_ARGS(&RenderTargets[n]));
    Device->CreateRenderTargetView(RenderTargets[n].Get(), nullptr, rtvHandle);
    rtvHandle.ptr = SIZE_T(INT64(rtvHandle.ptr) + INT64(RTVDescriptorSize));
  }

  // Create command allocator
  Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator));

  // Create root signature
  D3D12_DESCRIPTOR_RANGE ranges[2];
  D3D12_ROOT_PARAMETER rootParameters[2];
  
  ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  ranges[0].NumDescriptors = 1;
  ranges[0].BaseShaderRegister = 0;
  ranges[0].RegisterSpace = 0;
  ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
  
  rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
  rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
  rootParameters[1].DescriptorTable.pDescriptorRanges = &ranges[0];

  rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
  rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
  rootParameters[0].Constants = {0, 0, 16};

  // Texture sampler
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

  // Describe root signature desc
  D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
  rootSignatureDesc.NumParameters = 2;
  rootSignatureDesc.pParameters = rootParameters;
  rootSignatureDesc.NumStaticSamplers = 1;
  rootSignatureDesc.pStaticSamplers = &sampler;
  rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  Microsoft::WRL::ComPtr<ID3DBlob> signature;
  Microsoft::WRL::ComPtr<ID3DBlob> error;
  D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
  if (signature == nullptr)
    OutputDebugString((CHAR *)error->GetBufferPointer());
  Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&RootSignature));
  // Create the pipeline
  Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
  Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;
#if defined(_DEBUG)
  UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UINT compileFlags = 0;
#endif
  D3DCompileFromFile(L"bin\\shaders\\shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, &error);
  if (error != nullptr)
    OutputDebugString("Vertex shader error: "), OutputDebugString((CHAR *)error->GetBufferPointer());

  D3DCompileFromFile(L"bin\\shaders\\shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error);
  if (error != nullptr)
    OutputDebugString("Pixel shader error: "), OutputDebugString((CHAR *)error->GetBufferPointer());

  // Define the vertex input layout
  D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
  };

  // Create the graphics pipeline state object (PSO).
  D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
  psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
  psoDesc.pRootSignature = RootSignature.Get();
  psoDesc.VS = { (BYTE *)vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
  psoDesc.PS = { (BYTE *)pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };

  D3D12_RASTERIZER_DESC RDesc = {};

  RDesc.FillMode = D3D12_FILL_MODE_SOLID;
  RDesc.CullMode = D3D12_CULL_MODE_BACK;
  RDesc.FrontCounterClockwise = FALSE;
  RDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
  RDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
  RDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  RDesc.DepthClipEnable = TRUE;
  RDesc.MultisampleEnable = FALSE;
  RDesc.AntialiasedLineEnable = FALSE;
  RDesc.ForcedSampleCount = 0;
  RDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
  psoDesc.RasterizerState = RDesc;

  D3D12_BLEND_DESC BDesc = {};

  BDesc.AlphaToCoverageEnable = FALSE;
  BDesc.IndependentBlendEnable = FALSE;
  const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
  {
    FALSE,FALSE,
    D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
    D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
    D3D12_LOGIC_OP_NOOP,
    D3D12_COLOR_WRITE_ENABLE_ALL,
  };
  for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    BDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

  psoDesc.BlendState = BDesc;
  psoDesc.DepthStencilState.DepthEnable = FALSE;
  psoDesc.DepthStencilState.StencilEnable = FALSE;
  psoDesc.SampleMask = UINT_MAX;
  psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  psoDesc.NumRenderTargets = 1;
  psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
  psoDesc.SampleDesc.Count = 1;

  Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&PipelineState));

  // Create the command list
  Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), PipelineState.Get(), IID_PPV_ARGS(&CommandList));

  // Define the geometry for a triangle
  FLT m_aspectRatio = (FLT)Width / (FLT)Height;
  Vertex triangleVertices[] =
  {
    { vec3(0.0, 0.25 * m_aspectRatio, 0.0), vec4(1.0, 0.0, 0.0, 1.0) },
    { vec3(0.25, -0.25 * m_aspectRatio, 0.0), vec4(0.0, 1.0, 1.0, 1.0) },
    { vec3(-0.25, -0.25 * m_aspectRatio, 0.0), vec4(0.0, 0.0, 1.0, 1.0) }
  };

  const UINT vertexBufferSize = sizeof(triangleVertices);

  D3D12_HEAP_PROPERTIES heap;
  heap.Type = D3D12_HEAP_TYPE_UPLOAD;
  heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  heap.CreationNodeMask = 1;
  heap.VisibleNodeMask = 1;

  D3D12_RESOURCE_DESC buffer;
  buffer.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  buffer.Alignment = 0;
  buffer.Width = vertexBufferSize;
  buffer.Height = 1;
  buffer.DepthOrArraySize = 1;
  buffer.MipLevels = 1;
  buffer.Format = DXGI_FORMAT_UNKNOWN;
  buffer.SampleDesc.Count = 1;
  buffer.SampleDesc.Quality = 0;
  buffer.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  buffer.Flags = D3D12_RESOURCE_FLAG_NONE;

  Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &buffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&VBuffer));

  // Copy the triangle data to the vertex buffer
  UINT8* pVertexDataBegin;
  D3D12_RANGE readRange;
  readRange.Begin = 0;
  readRange.End = 0;
  VBuffer->Map(0, &readRange, (VOID **)&pVertexDataBegin);
  memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
  VBuffer->Unmap(0, nullptr);

  // Initialize the vertex buffer view
  VBufferView.BufferLocation = VBuffer->GetGPUVirtualAddress();
  VBufferView.StrideInBytes = sizeof(Vertex);
  VBufferView.SizeInBytes = vertexBufferSize;
  
  // Describe and create a Texture2D
  Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;
  D3D12_RESOURCE_DESC textureDesc = {};
  textureDesc.MipLevels = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.Width = TextureWidth;
  textureDesc.Height = TextureHeight;
  textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
  textureDesc.DepthOrArraySize = 1;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;
  textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

  D3D12_HEAP_PROPERTIES DefHeap = {};
  DefHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
  DefHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  DefHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  DefHeap.CreationNodeMask = 1;
  DefHeap.VisibleNodeMask = 1;
  Device->CreateCommittedResource(&DefHeap, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&Texture));

  // Create the GPU upload buffer
  auto Desc = Texture.Get()->GetDesc();
  UINT64 Size = 0;
  ID3D12Device* pDevice = nullptr;
  Texture.Get()->GetDevice(IID_ID3D12Device, (VOID **)&pDevice);
  pDevice->GetCopyableFootprints(&Desc, 0, 1, 0, nullptr, nullptr, nullptr, &Size);
  pDevice->Release();
  const UINT64 uploadBufferSize = Size;

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

  Device->CreateCommittedResource(&UpHeap, D3D12_HEAP_FLAG_NONE, &UpBuf, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&textureUploadHeap));

  // Update texture.
  std::vector<UINT8> texture = GenerateTextureData();
  D3D12_SUBRESOURCE_DATA textureData = {};
  textureData.pData = &texture[0];
  textureData.RowPitch = TextureWidth * TexturePixelSize;
  textureData.SlicePitch = textureData.RowPitch * TextureHeight;
  
  UINT64 RequiredSize = 0;
  auto MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * 1;
  if (MemToAlloc > SIZE_MAX)
    return;
  void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
  if (pMem == nullptr)
    return;
  auto pLayouts = static_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
  auto pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + 1);
  auto pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + 1);
  
  auto Desc1 = Texture.Get()->GetDesc();
  ID3D12Device* pDevice1 = nullptr;
  Texture.Get()->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(&pDevice1));
  pDevice1->GetCopyableFootprints(&Desc1, 0, 1, 0, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
  pDevice1->Release();
  
  // Minor validation
  auto IntermediateDesc = textureUploadHeap.Get()->GetDesc();
  auto DestinationDesc = Texture.Get()->GetDesc();
  if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
    IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
    RequiredSize > SIZE_T(-1) ||
    (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER))
    return;

  BYTE* pData;
  textureUploadHeap.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pData));

  if (pRowSizesInBytes[0] > SIZE_T(-1))
    return;
  D3D12_MEMCPY_DEST DestData = { pData + pLayouts[0].Offset, pLayouts[0].Footprint.RowPitch, SIZE_T(pLayouts[0].Footprint.RowPitch)
    * SIZE_T(pNumRows[0]) };
  for (UINT z = 0; z < pLayouts[0].Footprint.Depth; ++z)
  {
    auto pDestSlice = static_cast<BYTE*>(DestData.pData) + DestData.SlicePitch * z;
    auto pSrcSlice = static_cast<const BYTE*>(textureData.pData) + textureData.SlicePitch * LONG_PTR(z);
    for (UINT y = 0; y < pNumRows[0]; ++y)
    {
      memcpy(pDestSlice + DestData.RowPitch * y,
        pSrcSlice + textureData.RowPitch * LONG_PTR(y),
        static_cast<SIZE_T>(pRowSizesInBytes[0]));
    }
  }
  textureUploadHeap.Get()->Unmap(0, nullptr);

  if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
    CommandList.Get()->CopyBufferRegion(Texture.Get(), 0, textureUploadHeap.Get(), pLayouts[0].Offset, pLayouts[0].Footprint.Width);
  else
  {
    D3D12_TEXTURE_COPY_LOCATION Dst;
    Dst.pResource = Texture.Get();
    Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    Dst.PlacedFootprint = {};
    Dst.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION Src;
    Src.pResource = textureUploadHeap.Get();
    Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    Src.PlacedFootprint = pLayouts[0];

    CommandList.Get()->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
  }
  HeapFree(GetProcessHeap(), 0, pMem);
  
  // Update texture barrier
  D3D12_RESOURCE_BARRIER TexBar;
  TexBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  TexBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  TexBar.Transition.pResource = Texture.Get();
  TexBar.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
  TexBar.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
  TexBar.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  CommandList->ResourceBarrier(1, &TexBar);

  // Create srv for the texture.
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  Device->CreateShaderResourceView(Texture.Get(), &srvDesc, SRVHeap->GetCPUDescriptorHandleForHeapStart());
  
  // Create a constant buffer view.
  const UINT constantBufferSize = sizeof(SceneConstantBuffer);
  D3D12_RESOURCE_DESC Buf;
  Buf.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  Buf.Alignment = 0;
  Buf.Width = constantBufferSize;
  Buf.Height = 1;
  Buf.DepthOrArraySize = 1;
  Buf.MipLevels = 1;
  Buf.Format = DXGI_FORMAT_UNKNOWN;
  Buf.SampleDesc.Count = 1;
  Buf.SampleDesc.Quality = 0;
  Buf.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  Buf.Flags = D3D12_RESOURCE_FLAG_NONE;

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

  // Close command list 
  CommandList->Close();
  ID3D12CommandList* CommandLists[] = { CommandList.Get() };
  CommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);
  
  // Create Fence
  Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));
  FenceValue = 1;

  // Create an event handle to use for frame synchronization.
  FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  if (FenceEvent == nullptr)
    HRESULT_FROM_WIN32(GetLastError());

  WaitForPreviousFrame();
} /* End of 'vldx::core::core' function */

/* core class destructor.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
vldx::core::~core( VOID )
{
  WaitForPreviousFrame();
  CloseHandle(FenceEvent);
} /* End of 'vldx::core::~core' function */

/* END OF 'dx12.cpp' FILE */