/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : dx12_shader.h
  * PURPOSE     : Animation project.
  *               Direct X12.
  * PROGRAMMER  : Ivan Dmitriev.
  * LAST UPDATE : 25.10.2021
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __dx12_shader_h_
#define __dx12_shader_h_

#include <wrl/client.h>

#include "dx12.h"

/* Beginning of 'ivdx' namespace */
namespace ivdx
{
  /* Beginning of 'dx12' namespace */
  namespace dx12
  {
    /* Beginning of 'shader' class */
    class shader 
    {
    private:
      Microsoft::WRL::ComPtr<ID3DBlob> VSCode, Errors;
      D3D12_SHADER_BYTECODE ByteCode;
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
           const std::string &STarget, const UINT &CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION );
    
      /* Get shader byte code function.
       * ARGUMENTS: None;
       * RETURNS: (D3D12_SHADER_BYTECODE &) self reference on shader byte code.
       */
      D3D12_SHADER_BYTECODE & GetByteCode( VOID )
      {
        return ByteCode;
      } /* End of 'GetByteCode' function */

    }; /* End of 'shader' class */
  } /* end of 'dx12' namespace */
} /* end of 'ivdx' namespace */

#endif /* __dx12_shader_h_ */

/* END OF 'dx12_shader.h' FILE */
