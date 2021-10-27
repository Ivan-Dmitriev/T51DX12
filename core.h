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
 * LAST UPDATE : 16.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __core_h_
#define __core_h_

#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <dxgidebug.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl/client.h>
namespace mswrl = Microsoft::WRL;

#include "../../../def.h"

/* Project name space */
namespace bodx
{
  /* DirectX 12 namecpace */
  namespace dx12
  {
    class core; // forward declaration
    
    class coreref
    {
      friend core;
    protected:
      static core *D3D;
    };

    // Shader compiler class interface
    class shader
    {
    private:
      mswrl::ComPtr<ID3DBlob> Shader {}; // shader
      mswrl::ComPtr<ID3DBlob> errors {}; // errors
      D3D12_SHADER_BYTECODE ByteCode;    // shader byte code

    public:
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
      shader( const std::wstring &SName, const std::string &EntryPoint,
              const std::string &STarget, const UINT &CompileFlags );

      /* Get byte code function.
       * ARGUMENTS: None;
       * RETURNS: (D3D12_SHADER_BYTECODE) shader byte code.
       */
      D3D12_SHADER_BYTECODE & GetByteCode( VOID );
    };

    // Texture class interface
    class texture : public coreref
    {
    private:
      INT W, H; // texture size
      mswrl::ComPtr<ID3D12Resource> Texture;

    public:
      /* Default construcotr. */
      texture( VOID ) = default;

      /* Class construcotr.
       * ARGUMENTS:
       *   - texture name:
       *       const std::wstring &FileName;
       */
      texture( const std::wstring &TName );

      /* Load texture function.
       * ARGUMENTS:
       *   - texture name:
       *       const std::wstring &FileName;
       */
      VOID load( const std::wstring &TName );
    };

    // DirectX 12 core type
    class core
    {
    public:
      const INT &Width, &Height; // window frame size

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

      texture Texture;

      struct VERTEX
      {
        FLT position[3];
        FLT texcoord[2];
      };

      VERTEX VertexBufferData[3] =
      {
        { { 1.0f, -1.0f, 0.0f}, { 0.5f, 0.0f } },
        { { 0.0f,  1.0f, 0.0f}, { 1.0f, 1.0f } },
        { {-1.0f, -1.0f, 0.0f}, { 0.0f, 1.0f } }
      };
      UINT IndexBufferData[3] = {0, 1, 2};

#ifdef _DEBUG
      mswrl::ComPtr<ID3D12Debug3> Debug {};
#endif /* _DEBUG */
      UINT CbvSrvDescriptorSize {};

      D3D12_VIEWPORT ScreenViewport {};
      D3D12_RECT ScissorRect {};

      VOID WaitForPreviousFrame( VOID );

      /* D3D12 window resize function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Resize( VOID );

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
  } /* end of 'dx12' namespace */
} /* end of 'bodx' namespace */

#endif // __core_h_


/* END OF 'core.h' FILE */