/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : mth_camera.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Mathematics library.
 *               Space camera handle module.
 * PROGRAMMER  : CGSG'2021.
 *               Dmitriev Ivan.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __mth_camera_h_
#define __mth_camera_h_
 
#include "mth_vec3.h"
#include "mth_matr.h"
 
/* Math library namespace */
namespace math
{
  /* Space camera handle class */
  template<class type>
    class camera
    {
    private:
      /* Update camera project parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateProj( VOID )
      {
        type ratio_x = Size / 2, ratio_y = Size / 2;
 
        if (FrameW >= FrameH)
          ratio_x *= (type)FrameW / FrameH;
        else
          ratio_y *= (type)FrameH / FrameW;
        Proj = matr<type>::Frustum(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjDist, FarClip);
      } /* End of 'UpdateProj' function */
 
      /* Update camera view parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateView( VOID )
      {
        View = matr<type>::View(Loc, At, Up);
      } /* End of 'UpdateView' function */
 
    public:
      vec3<type>
        Loc,            // camera location
        Dir, Up, Right, // basis camera directions
        At;             // camera pivot point
      type
        ProjDist,       // near project plane distance
        FarClip,        // far project plane distance
        Size;           // inner project plane rectangle size
      INT
        FrameW, FrameH; // Camera frame size
      matr<type>
        View,           // view matrix
        Proj,           // projection matrix
        VP;             // View and Proj madtrix production
 
      /* Default constructor */
      camera( VOID ) :
        ProjDist(0.1f), FarClip(500.0f), Size(0.1f),
        Loc(13.3898449, 23.3018436, 53.4113731), Dir(0, 0, -1), Up(0, 1, 0), Right(1, 0, 0), 
        At(13.3898449, 23.3018436, 23.4113808), FrameW(30), FrameH(30)
      {
        UpdateProj();
        UpdateView();
        VP = View * Proj;
      } /* End of 'matr' function */
 
      /* Set project camera parameters function.
       * ARGUMENTS:
       *   - new near project plane size:
       *       type NewSize;
       *   - new project distance:
       *       type NewProjDist;
       *   - new far clip plane distance:
       *       type FarC;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & SetProj( type NewSize, type NewProjDist, type NewFarClip )
      {
        // Store new projection parameters
        ProjDist = NewProjDist;
        FarClip = NewFarClip;
        Size = NewSize;
 
        UpdateProj();
        VP = View * Proj;
        return *this;
      } /* End of 'SetProj' function */
 
      /* Set camera project pixel size function.
       * ARGUMENTS:
       *   - new frame size in pixels:
       *       type NewFrameW, NewFrameH;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & Resize( type NewFrameW, type NewFrameH )
      {
        FrameW = NewFrameW;
        FrameH = NewFrameH;
 
        UpdateProj();
        VP = View * Proj;
        return *this;
      } /* End of 'Resize' function */
 
      /* Set camera location and orientation function.
       * ARGUMENTS:
       *   - new camera location:
       *       const vec3<type> &L;
       *   - new camera pivot point:
       *       const vec3<type> &A;
       *   - new camera approx up direction:
       *       const vec3<type> &U;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & SetLocAtUp( const vec3<type> &L, const vec3<type> &A, const vec3<type> &U = vec3<type>(0, 1, 0) )
      {
        Loc = L;
        At = A;
        Up = U;
        Dir = (At - Loc).Normalizing();
        Right = (Dir % Up).Normalizing();
 
        UpdateView();
        VP = View * Proj;
        return *this;
      } /* End of 'SetLocAtUp' function */
 
      /* Camera rotate function.
       * ARGUMENTS:
       *   - rotate axis:
       *       const vec3<type> &Axis;
       *   - rotation angle (in degree):
       *       type AngleInDegree;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & Rotate( const vec3<type> &Axis, type AngleInDegree )
      {
        matr<type> m = matr<type>::Translate(-Loc) * matr<type>::Rotate(Axis, AngleInDegree) * matr<type>::Translate(Loc);
        At = m.TransformPoint(At);
        Up = m.TransformNormal(Up);
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Rotate' function */
 
      /* Camera movement function.
       * ARGUMENTS:
       *   - movement directions:
       *       const vec3<type> &Direction;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & Move( const vec3<type> &Direction )
      {
        Loc += Direction;
        At += Direction;
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Move' function */
    }; /* End of 'camera' class */
} /* end of 'math' namespace */
 
#endif /* __mth_camera_h_ */
 
/* END OF 'mth_camera.h' FILE */