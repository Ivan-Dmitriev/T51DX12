/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12.h
  * PURPOSE     : Animation project.
  *               Direct X 12 .
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __dx12_h_
#define __dx12_h_

#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <dxgidebug.h>

#include "../../../def.h"
#include "tetris.h"
#include "d3dx12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//typedef ivdx::tetris tetris;

/* Beginning of 'DxException' class */
class DxException
{
public:
  DxException() = default;
  DxException( HRESULT hr, const std::wstring &functionName,
  const std::wstring &filename, INT lineNumber )
  {
    std::wstring Res = L"Exeption thrown";
    Res += L"Function name: " + functionName;
    Res += L"Line number: " + lineNumber;
    Res += L"File name: " + filename;

    OutputDebugStringW(Res.c_str());
    MessageBoxW(nullptr, Res.c_str(), L"HR Failed", MB_OK);
   }
  std::wstring ToString() const;
  HRESULT ErrorCode = S_OK;
  std::wstring FunctionName;
  std::wstring Filename;
  INT LineNumber = -1;
}; /* End of 'DxException' class */

/* Useful macro function */
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                              \
{                                                     \
  HRESULT hr__ = (x);                                 \
  std::wstring wfn = __FILEW__);         \
  if (FAILED(hr__))                                   \
  {                                                   \
    throw DxException(hr__, L#x, wfn,                 \
  __LINE__);                                          \
  }                                                   \
}                                                     \ 
#endif

/* Beginning of 'ivdx' namespace */
namespace ivdx
{
  /* Beginning of 'dx12' namespace */
  namespace dx12
  {
    class coreref;

    /* Useful vertex type */
    struct vertex
    {
      FLOAT Pos[3];
      FLOAT Color[3];
      FLOAT texcoord[2];
    }; /* End of 'vertex' struct */

    /* Safe release COM interface function.
     * ARGUMENTS:
     *   - COM interface pointer reference:
     *       IUnknown **Ui;
     * RETURNS: None.
     */
    template<typename IPtrRef>
      VOID SafeRelease( IPtrRef  *&Ui )
      {
        if (Ui != nullptr)
          Ui->Release();
        Ui = nullptr;
      } /* End of 'SafeRelease' function */

    /* Beginning of 'core' class */
    class core
    {
    private:
      INT Width, Height;
      // Direct3D12 interfaces
      static const INT NumOfBuffers = 2;
      Microsoft::WRL::ComPtr<IDXGIFactory5> Factory {};
      Microsoft::WRL::ComPtr<ID3D12Device5> Device {};
      Microsoft::WRL::ComPtr<ID3D12CommandQueue> ComQueue {};
      Microsoft::WRL::ComPtr<IDXGISwapChain4> SwapChain {};
      Microsoft::WRL::ComPtr<ID3D12Resource> BackBuffers[NumOfBuffers];
      Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap {};
      Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVHeap {};
      Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap {};
      Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CBVHeap {};
      Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;

      // Update structure
      struct SceneConstantBuffer
      {
        matr wvp = matr::Identity();
        FLT padding[14]; // Padding so the constant buffer is 256-byte aligned.
      }; /* End of 'SceneConstantBuffer' structure */
      
      D3D12_RESOURCE_DESC depthStencilDesc;
      D3D12_CLEAR_VALUE optClear;

      Microsoft::WRL::ComPtr<ID3D12CommandAllocator> ComAllocator {};
      Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> ComList {};
      Microsoft::WRL::ComPtr<ID3D12Resource> ConstantBuffer {};
      Microsoft::WRL::ComPtr<ID3D12Fence> Fence {};

      Microsoft::WRL::ComPtr<ID3D12Resource> VBuffer;

      Microsoft::WRL::ComPtr<ID3D12Resource> VertexBuffer;

      D3D12_VERTEX_BUFFER_VIEW VertexBufferView;

      Microsoft::WRL::ComPtr<ID3D12Resource> IndexBuffer;
      D3D12_INDEX_BUFFER_VIEW IndexBufferView;

      Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;
      Microsoft::WRL::ComPtr<ID3D12Resource> CBuffer;
      
      SceneConstantBuffer CBufferData = {};
      Microsoft::WRL::ComPtr<ID3D12Resource> UploadCBuffer;

      //UINT8 *CBVDataBegin = nullptr;

      HANDLE Event {};
      UINT64 FenceValue = 0;
      D3D12_VIEWPORT ScreenViewport;
      D3D12_RECT ScissorRect;
      INT RtvDescriptorSize = 0;
      INT CurrBackBuffer = 0;
      INT FrameIndex = 0;
      INT creationFlags = 0;
      // UINT IndexBufferData[3] = {0, 1, 2};
      // Texture
      UINT TextureWidth = 256;
      UINT TextureHeight = 256;
      UINT TexturePixelSize = 4;

      //::tetris Tetris;

#ifdef _DEBUG
      Microsoft::WRL::ComPtr<ID3D12Debug3> Debug {};
      Microsoft::WRL::ComPtr<IDXGIDebug1> dxgiDebug;
      //ID3D12Debug *DebugDev;
#endif /* _DEBUG */
      
      /* Beginning of pipeline structure */
      struct PIPELINE 
      {
        Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineState;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature;
      }; /* End of 'PIPELINE' structure */

      // Pipeline data
      PIPELINE Pipeline1, Pipeline2;

      // Depth buffer and render target
      Microsoft::WRL::ComPtr<ID3D12Resource> DepthBuffer {}, RenderTarget {};

      // Background color 
      vec4 BackColor = {0.30f, 0.47f, 0.8f, 1.0f};
      //BackColor = vec4(0.30f, 0.47f, 0.8f, 1.0f);
      
      // Scene camera
      camera SceneCam;
      //vertex triangleVertices[3] =
      //{
      //  {{1.0f, -1.0f, 0.0f},  {0.5f, 0.0f}},
      //  {{0.0f, 1.0f, 0.0f},   {1.0f, 1.0f}},
      //  {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
      //};
      //vertex triangleVertices[3] =
      //{
      //  {{1.0f, -1.0f, 0.0f},  {1.0f, 0.0f, 0.0f}},
      //  {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
      //  {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
      //};

      vertex triangleVertices[8] = 
      {
        { {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} }, // 0
        { {-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}  }, // 1
        { {1.0f,  1.0f, -1.0f},  {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}  }, // 2
        { {1.0f, -1.0f, -1.0f},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}  }, // 3
        { {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}  }, // 4
        { {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}  }, // 5
        { {1.0f, 1.0f,  1.0f},   {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }, // 6
        { {1.0f, -1.0f,  1.0f},  {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}  }  // 7
      };
      
      UINT IndexBufferData[36] =
      {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        4, 5, 1, 4, 1, 0,
        3, 2, 6, 3, 6, 7,
        1, 5, 6, 1, 6, 2,
        4, 0, 3, 4, 3, 7
      };

      //vertex triangleVertices[3] =
      //{
      //  {{1.0f, 1.0f, 0.0f},  {0.5f, 0.0f}},
      //  {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
      //  {{0.0f, -1.0f, 1.0f},   {1.0f, 1.0f}}
      //};

      // Texture sample

      Microsoft::WRL::ComPtr<ID3D12Resource> TextureSample;

    public:
      struct CONSTBUFFER
      {
        vec4 FieldSize;
        matr M;
        vec4 Poses[700];
      };

      vec4 Poses1[700];

      CONSTBUFFER ConstantBufferData[1] =
      {
        vec4(1.0f, 1.0f, 0, 0),
        matr::Identity(),
        *Poses1
      };
      BYTE *CBVDataBegin;

    public:
      ::ivdx::tetris Game;

      /* Rendering pipeline creation function.
       * ARGUMENTS: 
       *   - pipeline index:
       *       UINT PipeLineIndex;  
       * RETURNS: (PIPELINE) result pipeline filled structure.
       */
      PIPELINE CreatePipeline( UINT PipeLineIndex );

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
      Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuffer( UINT Size, VOID *Data, 
        D3D12_RESOURCE_STATES State );

      /* Depth buffer creation function.
       * ARGUMENTS:
       *   - buffer size:
       *       INT W, H;
       * RETURNS: NONE.
       */
      VOID CreateDepthBuffer( INT NewW, INT NewH );

      /* Depth texture resource creation function.
       * ARGUMENTS:
       *   - depth buffer size in pixels:
       *       UINT Width, Height;
       * RETURNS:
       *   (Microsoft::WRL::ComPtr<ID3D12Resource>) vidx::dx12::core depth buffer resource pointer.
       */
      Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthBuffer( UINT Width, UINT Height );

      /* D3D12 Render function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Render( INT Cnt );

      /* D3D12 depth buffer init function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID DepthBufferInit( VOID );

      /* D3D12 Responce function.
       * ARGUMENTS: 
       *   - State of keys on the current frame:
       *       BYTE *Keys;
       *   - Global interframe interval:
       *       FLT GlobalDeltaTime;
       *   - Mouse coordinates:
       *       FLT MouseDX, MouseDY;
       * RETURNS: None.
       */
      VOID Response( BYTE *Keys, FLT GlobalDeltaTime, FLT MouseDX, FLT MouseDY );

      /* Wait for single frame function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID WaitForPreviousFrame( VOID );

      /* D3D12 window resize function.
       * ARGUMENTS: 
       *   - new window size:
       *       UINT W, H;
       * RETURNS: None.
       */
      VOID Resize( UINT W, UINT H );

      /* D3D12 create render target function.
       * ARGUMENTS:
       *   - new window size:
       *       UINT W, H;
       * RETURNS: (Microsoft::WRL::ComPtr<ID3D12Resource>) pointer on result target.
       */
      Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTarget( UINT W, UINT H );

      /* Texture resource creation function.
       * ARGUMENTS:
       *   - new texture size in pixels:
       *       UINT Width, Height;
       *   - texture data pointer:
       *       VOID *TextureData;
       * RETURNS: (Microsoft::WRL::ComPtr<ID3D12Resource>) texture resource pointer.
       */
      Microsoft::WRL::ComPtr<ID3D12Resource>  CreateTexture( UINT Width, UINT Height, VOID *TextureData );

      /* Texture resource creation function.
       * ARGUMENTS:
       *   - new texture size in pixels:
       *       UINT W, H;
       *   - texture data pointer:
       *       VOID *TextureData;
       * RETURNS: (ID3D12Resource *) texture resource pointer.
       */
      VOID LoadTexture( std::string FileName );
      /* Synchronize CPU and GPU function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Synchronize( VOID );

      /* D3D12 window resize function.
       * ARGUMENTS: None.
       * RETURNS: (PIPELINE) result pipeline.
       */
      VOID ResourcesInit( VOID );

      /* Create texture.
       * ARGUMENTS: None.
       * RETURNS: std::vector<UINT8> data.
       */
      std::vector<UINT8> GenerateTextureData( INT W, INT H, INT TexturePixelSize );

      /* Round the byte size to a multiple of the minimum hardware allocation size nearest 256 bytes .
       * ARGUMENTS: None.
       * RETURNS: std::vector<UINT8> data.
       */
      UINT CalcConstantBufferByteSize( UINT byteSize );

    public:
      /* Class constructor 
       * ARGUMENTS:
       *   - work window handle:
       *       HWND hWnd;
       */
      core( HWND hWnd );

      /* Class destructor */
      ~core( VOID );

    }; /* End of 'core' class */

    /* DirectX12 core reference class */
    class coreref 
    {
      friend core;
    public:
      /* Pointer to Direct3D core */
      static core *D3D;
    }; /* End of 'coreref' class */

  } /* end of 'dx12' namespace */
} /* end of 'ivdx' namespace */

#endif /* __dx12_h_ */

/* END OF 'dx12.h' FILE */