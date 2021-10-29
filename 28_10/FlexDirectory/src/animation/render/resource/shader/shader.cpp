/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : shader.cpp
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               DirectX 12 core.
 *               Shader resource.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 26.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <bodx.h>

#include "../../../../bodx.h"

/* Class construcotr.
 * ARGUMENTS:
 *   - shader name:
 *       const std::wstring &SName;
 *   - shader type:
 *       SHADER_TYPE Type;
 */
bodx::dx12::shader::shader( const std::wstring &SName, SHADER_TYPE Type )
{
#if defined(_DEBUG)
  // Enable better shader debugging with the graphics debugging tools.
  UINT CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UINT CompileFlags = 0;
#endif

  std::string EntryPoint = Type == SHADER_TYPE::VERTEX ? "VSMain" : "PSMain";
  std::string Version    = Type == SHADER_TYPE::VERTEX ? "vs_5_0" : "ps_5_0";

  CompileStatus = D3DCompileFromFile(std::wstring(L"bin/shaders/" + SName + L".hlsl").c_str(), nullptr, nullptr,
                                     EntryPoint.c_str(), Version.c_str(), CompileFlags, 0, &Shader, &errors);
  if (CompileStatus >= 0)
  {
    ByteCode.pShaderBytecode = Shader->GetBufferPointer();
    ByteCode.BytecodeLength = Shader->GetBufferSize();
  }
  else
  {
    ByteCode.pShaderBytecode = {};
    ByteCode.BytecodeLength = 0;
  }
} /* End of 'bodx::dx12::shader::shader' function */

/* Get byte code function.
 * ARGUMENTS: None;
 * RETURNS: (D3D12_SHADER_BYTECODE) shader byte code.
 */
D3D12_SHADER_BYTECODE & bodx::dx12::shader::GetByteCode( VOID )
{
  return ByteCode;
} /* End of 'bodx::dx12::shader::GetByteCode' function */


/* Class construcotr.
 * ARGUMENTS:
 *   - shader name:
 *       const std::wstring &SName;
 */
bodx::dx12::shader_programm::shader_programm( const std::wstring &SName ) :
  Vertex(SName, SHADER_TYPE::VERTEX),
  Pixel(SName, SHADER_TYPE::PIXEL)
{
} /* End of 'bodx::dx12::shader_programm::shader_programm' function */

/* END OF 'shader.cpp' FILE */