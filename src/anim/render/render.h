/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : render.h
 * PURPOSE     : Animation project.
 *               Render class implementation module.
 * PROGRAMMER  : Ivan Dmitriev.
 * LAST UPDATE : 25.10.2021
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */


#ifndef __render_h_
#define __render_h_

#include "dx/dx12.h"
#include "../../def.h"
#include "../../mth/mth.h"

namespace ivdx
{
  /* Debug output function.
   * ARGUMENTS:
   *   - source APi or device:
   *      UINT Source;
   *   - error type:
   *      UINT Type;
   *   - error message id:
   *      UINT Id, 
   *   - message severity:
   *      UINT severity, 
   *   - message text length:
   *      INT Length, 
   *   - message text:
   *      CHAR *Message, 
   *   - user addon parameters pointer:
   *      VOID *UserParam;
   * RETURNS: None.
   */
  VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                               INT Length, const CHAR *Message, const VOID *UserParam );
  /* Render class */
  class render 
  {
  private:
    HWND &hRndWnd;
 
    HDC hRndDC;
    HGLRC hRndGLRC;
  public:
    INT &FrameW, &FrameH;
 
  public:
 
    /* --> anim() : win(), render(win::hWnd, ... ) */
    render( HWND &hWnd, INT &W, INT &H ) : hRndWnd(hWnd), FrameW(W), FrameH(H) 
    {
    }
    ~render( VOID )
    {
    }
    VOID Init(VOID);
    VOID Close(VOID);
    VOID Render(VOID);
  }; /* End of 'render' class */

} /* end of 'ivdx' namespace */


#endif /* __render_h_ */

/* END OF 'render.h' FILE */
