/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12_render.cpp
  * PURPOSE     : Animation project.
  *               Direct X 12 .
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#include <ivdx.h>
#include "../../../ivdx.h"

#include <dxgidebug.h>

#include "d3dx12.h"

/* D3D12 Render function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ivdx::dx12::core::Render( VOID )
{
  FLOAT Col[] = {BackColor[0], BackColor[1], BackColor[2], BackColor[3]};

  INT q = 0;
  //for (INT y = 0; y < T.H + 2; y++)
  //{
  //  for (INT x = 0; x < T.W + 2; x++)
  //    if (x == 0 || x == T.W + 1 || y == 0 || y == T.H + 1 || T.Frame[(y - 1) * T.W + x - 1])
  //      ConstantBufferData[0].Poses[q++] = vec4((FLT)(-T.W / 2 + x), (FLT)(T.H / 2 - y), 0, 1);
  //}
  for (INT i = 0; i < T.NumOfCubes; i++)
    ConstantBufferData[0].Poses[i] = T.TransArray[i];
  //ConstantBufferData[0].M = SceneCam.VP;

  memcpy(CBVDataBegin, &ConstantBufferData, sizeof(ConstantBufferData));

  ComAllocator->Reset();
  ComList->Reset(ComAllocator.Get(), Pipeline1.PipelineState.Get());                        /* ??? */

  ComList->SetPipelineState(Pipeline1.PipelineState.Get());

  ComList->SetGraphicsRootSignature(Pipeline1.RootSignature.Get());

  //ID3D12DescriptorHeap *ppHeaps[] = { SRVHeap.Get() };

  //ComList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
  //ComList->SetGraphicsRootDescriptorTable(0, SRVHeap->GetGPUDescriptorHandleForHeapStart());

  ComList->SetDescriptorHeaps(1, SRVHeap.GetAddressOf());
  ComList->SetGraphicsRootDescriptorTable(1, SRVHeap->GetGPUDescriptorHandleForHeapStart());

  ComList->RSSetViewports(1, &ScreenViewport);
  ComList->RSSetScissorRects(1, &ScissorRect);
  
  // Indicate that the back buffer will be used as a render target
  D3D12_RESOURCE_BARRIER barrier = {};
  barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrier.Transition.pResource = BackBuffers[FrameIndex].Get();
  barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
  barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
  barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
  ComList->ResourceBarrier(1, &barrier);
  
  //D3D12_CPU_DESCRIPTOR_HANDLE tmp3 = DSVHeap->GetCPUDescriptorHandleForHeapStart();

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart(), FrameIndex, RtvDescriptorSize);
  rtvHandle.ptr = RTVHeap->GetCPUDescriptorHandleForHeapStart().ptr +
    (FrameIndex * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

  ComList->ClearRenderTargetView(rtvHandle, Col, 0, nullptr);
  ComList->ClearDepthStencilView(DSVHeap->GetCPUDescriptorHandleForHeapStart(),
    D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

  //CD3DX12_CPU_DESCRIPTOR_HANDLE tmp2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(RTVHeap->GetCPUDescriptorHandleForHeapStart(), CurrBackBuffer, RtvDescriptorSize);
  D3D12_CPU_DESCRIPTOR_HANDLE tmp4 = DSVHeap->GetCPUDescriptorHandleForHeapStart();
  ComList->OMSetRenderTargets(1, &rtvHandle, FALSE, &tmp4);

  matr rot = SceneCam.VP;

  ComList->SetGraphicsRoot32BitConstants(0, 16, rot, 0);

  ComList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  ComList->IASetVertexBuffers(0, 1, &VertexBufferView);
  ComList->IASetIndexBuffer(&IndexBufferView);

  ComList->DrawIndexedInstanced(_countof(IndexBufferData), T.NumOfCubes, 0, 0, 0);

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

  ID3D12CommandList *cmdsLists[] =
  {
    ComList.Get()
  };

  ComQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
  SwapChain->Present(1, 0);
  CurrBackBuffer = (CurrBackBuffer + 1) % NumOfBuffers;
#ifdef _DEBUG
  if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, 
      DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
#endif

  WaitForPreviousFrame();
} /* End of 'ivdx::dx12::core::Render' function */

/* END OF 'dx12_render.cpp' FILE */