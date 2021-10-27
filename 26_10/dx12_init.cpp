/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12_init.cpp
  * PURPOSE     : Animation project with Direct3D12.
  *               DirectX12 Class implementation module.
  *               D3D initialization/deinitialization functions.
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <ivdx.h>
#include <cstring>
#include <vector>

#include "../../../ivdx.h"
#include "dx12.h"
#include "d3dx12.h"
#include "dx12_shader.h"

ivdx::dx12::core * ivdx::dx12::coreref::D3D;
//ivdx::dx12::core * ivdx::dx12::coreref 

/* Class constructor */
ivdx::dx12::core::core( HWND hWnd )
{
  // Retrive window size data
  RECT rect;
  GetWindowRect(hWnd, &rect);
  Width = rect.right - rect.left;
  Height = rect.bottom - rect.top;

  CreateDXGIFactory(IID_PPV_ARGS(Factory.GetAddressOf()));

#ifdef _DEBUG
  D3D12GetDebugInterface(IID_PPV_ARGS(&Debug));
  Debug->EnableDebugLayer();
#endif /* _DEBUG */

  Microsoft::WRL::ComPtr<IDXGIAdapter> TmpAdapter, BestAdapter = nullptr;
  DXGI_ADAPTER_DESC AdapterDesc;
  DXGI_ADAPTER_DESC BestAdapterDesc = {};
  INT i = 0;

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
      BestAdapter->Release();
      BestAdapter = TmpAdapter;
    }
    else
      TmpAdapter->Release();
  }
  //OutputDebugString(AdapterDesc.Description);

  // Create D3D device
  D3D12CreateDevice(BestAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Device.GetAddressOf()));
  //BestAdapter->Release();

  // Create D3D command queue
  D3D12_COMMAND_QUEUE_DESC CQDesc {};
  CQDesc.NodeMask = 0;
  CQDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  CQDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  CQDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

  Device->CreateCommandQueue(&CQDesc, IID_PPV_ARGS(ComQueue.GetAddressOf()));

  // Create DXGI swap chain
  DXGI_SWAP_CHAIN_DESC1 SCD {};
  SCD.Width = Width;
  SCD.Height = Height;
  SCD.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
  SCD.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  SCD.Scaling = DXGI_SCALING_STRETCH;
  SCD.SampleDesc.Count = 1;
  SCD.SampleDesc.Quality = 0;
  SCD.BufferCount = NumOfBuffers;
  SCD.Stereo = FALSE;
  SCD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  SCD.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SCD.Flags = 0;

  Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain1 {};
  Factory->CreateSwapChainForHwnd(ComQueue.Get(), hWnd, &SCD, nullptr, nullptr, SwapChain1.GetAddressOf());

  SwapChain1->QueryInterface(IID_PPV_ARGS(SwapChain.GetAddressOf()));
  FrameIndex = SwapChain->GetCurrentBackBufferIndex();

  //SwapChain1->Release();
  //for (INT i = 0; i < NumOfBuffers; i++)
  //  SwapChain->GetBuffer(i, IID_PPV_ARGS(BackBuffers[i].GetAddressOf()));

  // Obtain heap descriptors
  D3D12_DESCRIPTOR_HEAP_DESC DHD {};
  DHD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  DHD.NodeMask = 0;
  DHD.NumDescriptors = 2 /* frame buffers */ + 1 /* render target */;
  DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  Device->CreateDescriptorHeap(&DHD, IID_PPV_ARGS(RTVHeap.GetAddressOf()));
  RtvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  // Depth-stencil
  DHD.NumDescriptors = 1 /* frame buffers */ + 1 /* render target */;
  DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
  DHD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
  DHD.NodeMask = 0;
  Device->CreateDescriptorHeap(&DHD, IID_PPV_ARGS(DSVHeap.GetAddressOf()));

  // Constant/shaders
  DHD.NumDescriptors = 2 /* ??? */;
  DHD.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  DHD.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  Device->CreateDescriptorHeap(&DHD, IID_PPV_ARGS(SRVHeap.GetAddressOf()));

  // Create cbv heap
  D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
  cbvHeapDesc.NumDescriptors = 1;
  cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  Device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&CBVHeap));

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
  // Create a RTV for each frame.
  for (UINT n = 0; n < NumOfBuffers; n++)
  {
    SwapChain->GetBuffer(n, IID_PPV_ARGS(&BackBuffers[n]));
    Device->CreateRenderTargetView(BackBuffers[n].Get(), nullptr, rtvHandle);
    rtvHandle.Offset(1, RtvDescriptorSize);
  }

  Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(ComAllocator.GetAddressOf()));

  //Pipeline1 = CreatePipeline(1);
  //Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, ComAllocator.Get(), Pipeline1.PipelineState.Get(),
  //                          IID_PPV_ARGS(ComList.GetAddressOf()));

  ScreenViewport.TopLeftX = 0.0f;
  ScreenViewport.TopLeftY = 0.0f;
  ScreenViewport.Width = static_cast<float>(Width);
  ScreenViewport.Height = static_cast<float>(Height);
  ScreenViewport.MinDepth = 0.0f;
  ScreenViewport.MaxDepth = 1.0f;
  
  //ComList->RSSetViewports(1, &ScreenViewport);

  // Create viewport and render rectangle
  ScissorRect.left = 0;
  ScissorRect.top = 0;
  ScissorRect.right = static_cast<LONG>(Width);
  ScissorRect.bottom = static_cast<LONG>(Height);

  RenderTarget = CreateRenderTarget(Width, Height);

  RtvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  //DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
  //depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  //depthStencilDesc.Alignment = 0;
  //depthStencilDesc.Width = Width;
  //depthStencilDesc.Height = Height;
  //depthStencilDesc.DepthOrArraySize = 1;
  //depthStencilDesc.MipLevels = 1;
  //depthStencilDesc.Format = DepthStencilFormat;
  //depthStencilDesc.SampleDesc.Count = 1;
  //depthStencilDesc.SampleDesc.Quality = 0;
  //depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
  //depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;  
  //
  //optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  //optClear.DepthStencil.Depth = 1.0f;
  //optClear.DepthStencil.Stencil = 0;

  //CD3DX12_HEAP_PROPERTIES CDXheapprop(D3D12_HEAP_TYPE_DEFAULT);
  //Device->CreateCommittedResource(
  //  &CDXheapprop, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON,
  //  &optClear, IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));
  //Device->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, 
  //  DSVHeap->GetCPUDescriptorHandleForHeapStart());

  //ID3D12CommandList *cmdsLists[] =
  //{
  //  ComList.Get()
  //};
  //ComQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
  
  //LoadTexture("BRICK.G24");
  coreref::D3D = this;
  Pipeline1 = ResourcesInit();

  SceneCam.SetLocAtUp(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0));
} /* End of 'ivdx::dx12::core::core' function */

/* D3D12 resource init function.
 * ARGUMENTS: None.
 * RETURNS: (PIPELINE) result pipeline.
 */
ivdx::dx12::core::PIPELINE ivdx::dx12::core::ResourcesInit( VOID )
{
  PIPELINE Res;

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
    //RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    //RootParameters[0].Constants = {0, 0, 16};

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

    Microsoft::WRL::ComPtr<ID3DBlob> signature {};
    Microsoft::WRL::ComPtr<ID3DBlob> error {};
    D3D12SerializeVersionedRootSignature(&RootSignatureDesc, &signature, &error);
    Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                                IID_PPV_ARGS(Res.RootSignature.GetAddressOf()));
    //Res.RootSignature->SetName(L"Texture Triangle Root Signature");
  }

  {
#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    shader VertexShader(L"shaders", "VSMain", "vs_5_0");
    shader PixelShader(L"shaders", "PSMain", "ps_5_0");

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      //{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
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
    PSODesc.pRootSignature = Res.RootSignature.Get();
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
    Device->CreateGraphicsPipelineState(&PSODesc, IID_PPV_ARGS(Res.PipelineState.GetAddressOf()));
  }

  Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, ComAllocator.Get(), nullptr, IID_PPV_ARGS(&ComList));

  {
    const UINT vertexBufferSize = sizeof(triangleVertices);

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
    memcpy(VertexDataBegin, triangleVertices, sizeof(triangleVertices));
    VertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
    VertexBufferView.StrideInBytes = sizeof(vertex);
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
  UINT elementByteSize = CalcConstantBufferByteSize(sizeof(SceneConstantBuffer));

  CD3DX12_HEAP_PROPERTIES tmp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
  CD3DX12_RESOURCE_DESC tmp2 = CD3DX12_RESOURCE_DESC::Buffer(elementByteSize);
  Device->CreateCommittedResource(&tmp, D3D12_HEAP_FLAG_NONE, &tmp2, 
    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&UploadCBuffer));

  LoadTexture("brick.g24");

  return Res;
} /* End of 'ivdx::dx12::core::ResourcesInit' function */

/* Class destructor */
ivdx::dx12::core::~core( VOID )
{
  //Debug->
  //ComAllocator->Release();
  //ComList->Release();
  //ComQueue->Release();
  //for (INT i = 0; i < NumOfBuffers; i++)
  //  BackBuffers[i]->Release();
  //RTVHeap->Release();
  //DSVHeap->Release();
  //SRVHeap->Release();

  //SwapChain->Release();
  //Device->Release();
  //Factory->Release();

#ifdef _DEBUG
  Debug->Release();
  if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, 
      DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_ALL));

#endif
  WaitForPreviousFrame();
  CloseHandle(Event);

} /* End of 'ivdx::dx12::core::~core' function */

/* END OF 'dx_12_init.cpp' FILE */