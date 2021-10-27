/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12render.cpp
	* PURPOSE     : Animation project.
	*               DirectX realesation module.
	*               Render function realesetion.
	* PROGRAMMER  : CGSG-Sr'2021.
	*
	* No part of this file may be changed without agreement of
	* Computer Graphics Support Group of 30 Phys-Math Lyceum
	*/

#include <madx.h>
#include "../../madx.h"

#include "d3dx12.h"

/* Render function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID madx::core::Render( VOID )
{
	ComListAlloc->Reset();
	ComList->Reset(ComListAlloc.Get(), nullptr);
	CD3DX12_RESOURCE_BARRIER tmp = CD3DX12_RESOURCE_BARRIER::Transition(BackBuffers[CurrBackBuffer].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	ComList->ResourceBarrier(1, &tmp);
	ComList->RSSetViewports(1, &ScreenViewport);
	ComList->RSSetScissorRects(1, &ScissorRect);
	ComList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(RTVHeap->GetCPUDescriptorHandleForHeapStart(), CurrBackBuffer, RtvDescriptorSize), vec4(1, 0, 0, 0), 0, nullptr);
	ComList->ClearDepthStencilView(DSVHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	CD3DX12_CPU_DESCRIPTOR_HANDLE tmp2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(RTVHeap->GetCPUDescriptorHandleForHeapStart(), CurrBackBuffer, RtvDescriptorSize);
	D3D12_CPU_DESCRIPTOR_HANDLE tmp3 = DSVHeap->GetCPUDescriptorHandleForHeapStart();
	ComList->OMSetRenderTargets(1, &tmp2, true, &tmp3);
	CD3DX12_RESOURCE_BARRIER tmp1 = CD3DX12_RESOURCE_BARRIER::Transition(BackBuffers[CurrBackBuffer].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	ComList->ResourceBarrier(1, &tmp1);
	ComList->Close();
	ID3D12CommandList* cmdsLists[] =
	{
		ComList.Get()
	};
	ComQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	SwapChain->Present(0, 0);
	CurrBackBuffer = (CurrBackBuffer + 1) % NumOfBuffers;
	FlushCommandQueue();
}	/* End of 'Render' function */

/* END OF 'dx12render.cpp' FILE */