/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : core.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core class defenition.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 25.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __core_h_
#define __core_h_

#include "../../timer.h"
#include "../../../def.h"

/* Project name space */
namespace bodx
{
  /* DirectX 12 namecpace */
  namespace dx12
  { 
    // DirectX 12 core type
    class core
    {
    public:
      INT Width, Height; // window frame size

      // Direct3D12 common interfaces
      mswrl::ComPtr<IDXGIFactory5> Factory {};
      mswrl::ComPtr<ID3D12Device5> Device {}; 
      mswrl::ComPtr<ID3D12PipelineState> PipelineState {};
      mswrl::ComPtr<ID3D12RootSignature> RootSignature {};

      // Commands control
      mswrl::ComPtr<ID3D12CommandQueue> ComQueue {};
      mswrl::ComPtr<ID3D12CommandAllocator> ComAllocator {};
      mswrl::ComPtr<ID3D12GraphicsCommandList> ComList {};
      mswrl::ComPtr<IDXGISwapChain4> SwapChain {};

      // Render target
      static const INT NoofBuffers = 3;
      mswrl::ComPtr<ID3D12Resource> BackBuffers[NoofBuffers];
      mswrl::ComPtr<ID3D12DescriptorHeap> RTVHeap {};
      UINT RtvDescriptorSize {};

      mswrl::ComPtr<ID3D12DescriptorHeap> SRVHeap {};

      // Depth buffer
      mswrl::ComPtr<ID3D12DescriptorHeap> DSVHeap {};

      UINT DsvDescriptorSize {};

      // Synchronization
      INT FrameIndex;
      mswrl::ComPtr<ID3D12Fence> Fence {};
      HANDLE FenceEvent {};
      UINT64 FenceValue {};

      // Resources
      mswrl::ComPtr<ID3D12Resource> VertexBuffer {};
      D3D12_VERTEX_BUFFER_VIEW VertexBufferView {};
      mswrl::ComPtr<ID3D12Resource> IndexBuffer {};
      D3D12_INDEX_BUFFER_VIEW IndexBufferView {};
      mswrl::ComPtr<ID3D12Resource> ConstantBuffer {};

            // Depth buffer and render target
      mswrl::ComPtr<ID3D12Resource> DepthBuffer {}, RenderTarget {};

      texture Texture;

      struct VERTEX
      {
        vec3 position;
        vec2 texcoord;
      };

      vec4 BackColor {0.30f, 0.47f, 0.8f, 1.0f}; 

      //VERTEX VertexBufferData[3] =
      //{
      //  { { 1.0f, -1.0f, 0.0f}, { 0.5f, 0.0f } },
      //  { { 0.0f,  1.0f, 0.0f}, { 1.0f, 1.0f } },
      //  { {-1.0f, -1.0f, 0.0f}, { 0.0f, 1.0f } }
      //};

      struct CONSTBUFFER
      {
        matr_data M;
        vec4 Poses[988];
      };

      vec4 Poses[988];

      CONSTBUFFER ConstantBufferData[1] =
      {
        matr::Identity(),
        *Poses
      };
      BYTE *CBVDataBegin;


      //UINT IndexBufferData[3] = {0, 1, 2};

#ifdef _DEBUG
      mswrl::ComPtr<ID3D12Debug3> Debug {};
#endif /* _DEBUG */
      UINT CbvSrvDescriptorSize {};

      D3D12_VIEWPORT ScreenViewport {};
      D3D12_RECT ScissorRect {};
      
      /* Synchroize function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID WaitForPreviousFrame( VOID );

      /* D3D12 window resize function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Resize( VOID );

      /* Resource inition function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID ResourcesInit( VOID );
    public:
      /* DirectX 12 core type constructor.
       * ARGUMENTS:
       *   - window handle reference:
       *       const HWND &hWnd;
       *   - window size reference:
       *       const INT &W, &H;
       */
      core( const HWND &hWnd, const INT &W, const INT &H );

      /* DirectX 12 core type destructor.
       * ARGUMENTS: None.
       */
      ~core( VOID );

      /* Rendring function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Render( VOID );

    }; /* End of 'core' class */


    /* Core interface helper class */
    class core_interface : public core_ref
    {
    public:
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
      static ID3D12PipelineState * CreatePipeLine( D3D12_INPUT_ELEMENT_DESC *VertexDescription,
                                                   UINT VertexDescriptonFieldsCnt,
                                                   D3D12_FILL_MODE FillMode, D3D12_CULL_MODE CullMode,
                                                   const std::wstring &SName );

      /* Depth texture resource creation function.
       * ARGUMENTS:
       *   - depth buffer size in pixels:
       *       UINT Width, Height;
       * RETURNS:
       *   (ID3D12Resource *) vidx::dx12::core depth buffer resource pointer.
       */
      static ID3D12Resource * CreateDepthBuffer( UINT Width, UINT Height );

      /* Render target texture resource creation function.
       * ARGUMENTS:
       *   - render target size in pixels:
       *       UINT Width, Height;
       * RETURNS:
       *   (ID3D12Resource *) vidx::dx12::core render target resource pointer.
       */
      static ID3D12Resource * CreateRenderTarget( UINT Width, UINT Height );
    }; /* End of 'core_interface' class */
  } /* end of 'dx12' namespace */
} /* end of 'bodx' namespace */

#endif // __core_h_


/* END OF 'core.h' FILE */