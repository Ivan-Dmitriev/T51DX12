/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : render.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               Render class.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 16.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>
#include "../../bodx.h"

bodx::timer T;

/* Render system type constructor.
 * ARGUMENTS:
 *   - window handle reference:
 *       const HWND &hWnd;
 *   - window size reference:
 *       const INT &W, &H;
 */
bodx::render::render( const HWND &hWnd, const INT &W, const INT &H ) :
  core(hWnd, W, H), hWnd(hWnd), hDC(GetDC(hWnd)), FrameW(W), FrameH(H)
{
  Camera.SetLocAtUp(vec3(0, 0, -40), vec3(0));
} /* End of 'bodx::render::render' constructor */

/* Render system type destructor.
 * ARGUMENTS: None.
 */
bodx::render::~render( VOID )
{      
} /* End of 'bodx::render::~render' destructor */

/* Rendring resizing function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID bodx::render::Resize( VOID )
{
  dx12::core::Resize();
  Camera.Resize(FrameW, FrameH);
} /* End of 'bodx::render::Resize' function */

/* Rendring copy frame function.
 * ARGUMENTS:
 *   - WINDOW HANDLE:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID bodx::render::CopyFrame( VOID )
{
} /* End of 'bodx::render::CopyFrame' function */

/* Rendring start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID bodx::render::Start( INT cnt )
{
  char buf[30];
  sprintf(buf, "%lf", T.FPS);
  SetWindowText(hWnd, (LPCSTR)buf);
  T.Response();

  ConstantBufferData[0].M = Camera.VP;

  memcpy(CBVDataBegin, &ConstantBufferData, sizeof(ConstantBufferData));

  ComAllocator->Reset();

  ComList->Reset(ComAllocator.Get(), PipelineState.Get());

  // Set necessary state.
  ComList->SetGraphicsRootSignature(RootSignature.Get());

  ID3D12DescriptorHeap *ppHeaps[] = { SRVHeap.Get() };

  ComList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
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

  ComList->DrawIndexedInstanced(36, cnt, 0, 0, 0);

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
} /* End of 'bodx::render::Start' function */

/* Rendring end function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID bodx::render::End( VOID )
{
} /* End of 'bodx::render::End' function */

/* END OF 'render.cpp' FILE */