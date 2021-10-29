/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : resource.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               Resources.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 26.10.2021.
 * NOTE        : Module namespace 'bodx::dx12'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __resource_h_
#define __resource_h_

#include "../../../def.h"

/* Project name space */
namespace bodx
{
  /* DirectX 12 namecpace */
  namespace dx12
  { 
    /****
     *  Shaders
     ****/

    /* Shader types enum */
    enum struct SHADER_TYPE
    {
      VERTEX,
      PIXEL
    };

    /* Shader class */
    class shader
    {
    private:
      mswrl::ComPtr<ID3DBlob> Shader {}; // shader
      mswrl::ComPtr<ID3DBlob> errors {}; // errors
      D3D12_SHADER_BYTECODE ByteCode;    // shader byte code
      HRESULT CompileStatus;             // shader status

    public:    
      /* Default construcotr. */
      shader( VOID ) = default;

      /* Class construcotr.
       * ARGUMENTS:
       *   - shader name:
       *       const std::wstring &SName;
       *   - shader type:
       *       SHADER_TYPE Type;
       */
      shader( const std::wstring &SName, SHADER_TYPE Type );

      /* Get byte code function.
       * ARGUMENTS: None;
       * RETURNS: (D3D12_SHADER_BYTECODE) shader byte code.
       */
      D3D12_SHADER_BYTECODE & GetByteCode( VOID );
    }; /* End of 'shader' class */

    class shader_programm
    {
    public:
      /* Shaders */
      shader Vertex;
      shader Pixel;

      /* Default construcotr. */
      shader_programm( VOID ) = default;

      /* Class construcotr.
       * ARGUMENTS:
       *   - shader name:
       *       const std::wstring &SName;
       */
      shader_programm( const std::wstring &SName );
    }; /* End of 'shader_programm' class. */


    /****
     *  Textures
     ****/

    /* Texture class interface */
    class texture : public coreref
    {
    private:
      BYTE *Bits;
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
       * RETURNS: (texture &) Self-refference.
       */
      texture & load( const std::wstring &TName );

      /* Create texture function.
       * ARGUMENTS: None.
       * RETURNS: (texture &) Self-refference.
       */
      texture & create( VOID );

      /* Free texture function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID free( VOID );

      /* Default destructor. */
      ~texture( VOID );
    }; /* End of 'texture' class */
  } /* end of 'dx12' namespace */
} /* end of 'bodx' namespace */

#endif // __resource_h_
      
/* END OF 'resource.h' FILE */