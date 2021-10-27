/***************************************************************
 * Copyright (C) 2020-2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE NAME   : dx12.cpp
  * PURPOSE     : T51DX project.
  *               Direct X 12 render module.
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

/* Render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID vldx::core::Render( VOID )
{
  CommandAllocator->Reset();
  CommandList->Reset(CommandAllocator.Get(), PipelineState.Get());

  CommandList->SetGraphicsRootSignature(RootSignature.Get());
  CommandList->SetComputeRootSignature(RootSignature.Get());

  CommandList->RSSetViewports(1, &ViewPort);
  CommandList->RSSetScissorRects(1, &ScissorRect);

  ID3D12DescriptorHeap* ppHeaps[] = { SRVHeap.Get() };
  CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

  CommandList->SetGraphicsRootDescriptorTable(1, SRVHeap->GetGPUDescriptorHandleForHeapStart());

  D3D12_RESOURCE_BARRIER barrier = {};
  barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrier.Transition.pResource = RenderTargets[FrameIndex].Get();
  barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
  barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
  barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  CommandList->ResourceBarrier(1, &barrier);

  D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};
  rtvHandle.ptr = SIZE_T(INT64(RTVHeap->GetCPUDescriptorHandleForHeapStart().ptr) + INT64(FrameIndex) * INT64(RTVDescriptorSize));
  CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
  CommandList->SetGraphicsRoot32BitConstants(0, 16, matr::Identity(), 0);

  CommandList->ClearRenderTargetView(rtvHandle, vec4(0.3, 0.47, 0.8, 1), 0, nullptr);
  CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  CommandList->IASetVertexBuffers(0, 1, &VBufferView);
  CommandList->DrawInstanced(3, 1, 0, 0);

  barrier = {};
  barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrier.Transition.pResource = RenderTargets[FrameIndex].Get();
  barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
  barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
  barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  CommandList->ResourceBarrier(1, &barrier);

  CommandList->Close();

  ID3D12CommandList* CommandLists[] = { CommandList.Get() };
  
  CommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);
  SwapChain->Present(1, 0);

  WaitForPreviousFrame();
} /* End of 'vldx::core::Render' funnction */

/* END OF 'dx12.cpp' FILE */