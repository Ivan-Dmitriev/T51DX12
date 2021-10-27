 // Create root signature.
  D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
  rootSignatureDesc.NumParameters = 0;
  rootSignatureDesc.pParameters = nullptr;
  rootSignatureDesc.NumStaticSamplers = 0;
  rootSignatureDesc.pStaticSamplers = nullptr;
  rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

  Microsoft::WRL::ComPtr<ID3DBlob> signature;
  Microsoft::WRL::ComPtr<ID3DBlob> error;
  D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
  Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&RootSignature));

  // Create the pipeline
  Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
  Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
  UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UINT compileFlags = 0;
#endif
  D3DCompileFromFile(L"bin\\shaders\\shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
  D3DCompileFromFile(L"bin\\shaders\\shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);

  // Define the vertex input layout.
  D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
  };

  // Create the graphics pipeline state object (PSO).
  D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
  psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
  psoDesc.pRootSignature = RootSignature.Get();
  psoDesc.VS = { (BYTE *)vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
  psoDesc.PS = { (BYTE *)pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
  psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  psoDesc.DepthStencilState.DepthEnable = FALSE;
  psoDesc.DepthStencilState.StencilEnable = FALSE;
  psoDesc.SampleMask = UINT_MAX;
  psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  psoDesc.NumRenderTargets = 1;
  psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
  psoDesc.SampleDesc.Count = 1;
  Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&PipelineState));

  // Create the command list.
  Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), PipelineState.Get(), IID_PPV_ARGS(&CommandList));
  CommandList->Close();

  FLT m_aspectRatio = (FLT)Width / (FLT)Height;
  // Define the geometry for a triangle.
  Vertex triangleVertices[] =
  {
    { vec3(0.0, 0.25 * m_aspectRatio, 0.0), vec4(1.0, 0.0, 0.0, 1.0) },
    { vec3(0.25, -0.25 * m_aspectRatio, 0.0), vec4(0.0, 1.0, 1.0, 1.0) },
    { vec3(-0.25, -0.25 * m_aspectRatio, 0.0), vec4(0.0, 0.0, 1.0, 1.0) }
  };

  const UINT vertexBufferSize = sizeof(triangleVertices);

  D3D12_HEAP_PROPERTIES heap;
  heap.Type = D3D12_HEAP_TYPE_UPLOAD;
  heap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  heap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
  heap.CreationNodeMask = 1;
  heap.VisibleNodeMask = 1;

  D3D12_RESOURCE_DESC buffer;
  buffer.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  buffer.Alignment = 0;
  buffer.Width = vertexBufferSize;
  buffer.Height = 1;
  buffer.DepthOrArraySize = 1;
  buffer.MipLevels = 1;
  buffer.Format = DXGI_FORMAT_UNKNOWN;
  buffer.SampleDesc.Count = 1;
  buffer.SampleDesc.Quality = 0;
  buffer.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  buffer.Flags = D3D12_RESOURCE_FLAG_NONE;

  Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &buffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&VBuffer));

  // Copy the triangle data to the vertex buffer.
  UINT8* pVertexDataBegin;
  D3D12_RANGE readRange;
  readRange.Begin = 0;
  readRange.End = 0;
  VBuffer->Map(0, &readRange, (VOID **)&pVertexDataBegin);
  memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
  VBuffer->Unmap(0, nullptr);

  // Initialize the vertex buffer view.
  VBufferView.BufferLocation = VBuffer->GetGPUVirtualAddress();
  VBufferView.StrideInBytes = sizeof(Vertex);
  VBufferView.SizeInBytes = vertexBufferSize;
  
  // Create Fence
  Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));
  FenceValue = 1;

  // Create an event handle to use for frame synchronization.
  FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  if (FenceEvent == nullptr)
    HRESULT_FROM_WIN32(GetLastError());

  WaitForPreviousFrame();

VOID vldx::core::WaitForPreviousFrame( VOID )
{
  // Signal and increment the Fence value.
  const UINT64 fenceV = FenceValue;
  CommandQueue->Signal(Fence.Get(), fenceV);
  FenceValue++;

  // Wait until the previous frame is finished.
  if (Fence->GetCompletedValue() < fenceV)
  {
    Fence->SetEventOnCompletion(fenceV, FenceEvent);
    WaitForSingleObject(FenceEvent, INFINITE);
  }

  FrameIndex = SwapChain->GetCurrentBackBufferIndex();
} /* End of 'vldx::core::WaitForPreviousFrame' function */