/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : render.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               Render class decloration.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __render_h_
#define __render_h_

#include "../../def.h"
#include "DirectX12/tetris.h"

/* Project name space */
namespace bodx
{
  // Render system type
  class render : private dx12::core
  {
  private:
    const HWND &hWnd;            // window to render descriptor
    const HDC hDC;               // window to render device context handle
    const INT &FrameW, &FrameH;  // window to render frame size

  public:
    vec2 GetScreen( VOID )
    {
      return vec2((FLT)FrameW, (FLT)FrameH);
    }

    // User camera
    camera Camera;

    /* Render system type constructor.
     * ARGUMENTS:
     *   - window handle ref:
     *       HWND &hWnd;
     *   - window size ref:
     *       INT &W, &H;
     */
    render( const HWND &hWnd, const INT &W, const INT &H );

    /* Render system type destructor.
     * ARGUMENTS: None.
     */
    ~render( VOID );

    /* Rendring resizing function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Resize( VOID );

    /* Rendring copy frame function.
     * ARGUMENTS:
     *   - WINDOW HANDLE:
     *       HWND hWnd;
     * RETURNS: None.
     */
    VOID CopyFrame( VOID );

    /* Rendring start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Start( INT cnt );

    /* Rendring end function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */ 
    VOID End( VOID );
  }; /* End of 'render' class */
} /* end of 'bodx' namespace */

#endif /* __render_h_ */


/* END OF 'render.h' FILE */