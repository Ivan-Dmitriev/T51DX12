/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : texture.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core.
 *               Texture resource.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 25.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>

#include "../../../../bodx.h"

/* Class construcotr.
 * ARGUMENTS:
 *   - texture name:
 *       const std::swtring &TName;
 */
bodx::dx12::texture::texture( const std::wstring &TName )
{
  load(TName).create().free();
} /* End of 'bodx::dx12::texture::texture' function */

/* Load texture function.
 * ARGUMENTS:
 *   - texture name:
 *       const std::wstring &FileName;
 * RETURNS: (texture &) Self-refference.
 */
bodx::dx12::texture & bodx::dx12::texture::load( const std::wstring &TName )
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

  Bits = new BYTE[W * H * 4];

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

  return *this;
} /* End of 'bodx::dx12::texture::load' function */

/* Load texture function.
 * ARGUMENTS:
 *   - texture name:
 *       const std::wstring &FileName;
 * RETURNS: (texture &) Self-refference.
 */
bodx::dx12::texture & bodx::dx12::texture::create( VOID )
{
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

  D3D->ComList->Close();
  ID3D12CommandList *ppCommandLists[] = { D3D->ComList.Get() };
  D3D->ComQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  D3D->Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&D3D->Fence));
  D3D->FenceValue++;

  D3D->FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

  D3D->WaitForPreviousFrame();
  
  return *this;
} /* End of 'bodx::dx12::texture::create' function */

/* Free texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::dx12::texture::free( VOID )
{
  if (Bits != nullptr)
    delete[] Bits;
} /* End of 'bodx::dx12::texture::free' function */

/* Default destructor. */
bodx::dx12::texture::~texture( VOID )
{
  free();
} /* End of 'bodx::dx12::texture::~texture' function */

/* END OF 'texture.cpp' FILE */