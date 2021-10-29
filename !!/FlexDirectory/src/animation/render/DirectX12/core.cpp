/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : core_init.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core inition.
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

  
  coreref::D3D = this;

  Resize();
  ResourcesInit();
} /* End of 'bodx::dx12::core::core' constructor */

/* D3D12 window resize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::core::Resize( VOID )
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

/* DirectX 12 core type destructor.
 * ARGUMENTS: None.
 */
bodx::dx12::core::~core( VOID )
{
} /* End of 'bods::dx12::core::~core' function */

/* END OF 'core_init.cpp' FILE */