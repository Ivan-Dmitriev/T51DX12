/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : animation.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Animation interface decloration.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __animation_h_
#define __animation_h_

#include <initializer_list>
#include "../def.h"
#include "win/win_main.h"
#include "timer.h"
#include "input/input.h"
#include "render/render.h"

/* Project name space */
namespace bodx
{
  class scene;
  class animation;
  /* Unit class */
  class unit
  {
  public:
    /* Virtual destructor */
    virtual ~unit( VOID )
    {
    } /* End of destructor */

    /* Virtual unit render function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    virtual VOID Render( animation *Ani )
    {
    } /* End of 'Render' function */

    /* Virtual unit response function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    virtual VOID Response( animation *Ani )
    {
    } /* End of 'Response' function */
  }; /* End of 'unit' class */

  /* Scene class */
  class scene : public unit
  {
  protected:
    stock<unit *> Units;
  public:
    /* Constructor */
    scene( VOID ) : Units()
    {
    } /* End of constructor */

    /* Destructor */
    ~scene( VOID ) override
    {
      Units.Walk([]( unit *Uni ){  delete Uni; });
    } /* End of destructor */

    /* Virtual unit render function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    VOID Render( animation *Ani ) override
    {
      Units.Walk([Ani]( unit *Uni ){  Uni->Render(Ani);  });
    } /* End of 'Render' function */

    /* Virtual unit response function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    VOID Response( animation *Ani ) override
    {
      Units.Walk([Ani]( unit *Uni ){  Uni->Response(Ani); });
    } /* End of 'Response' function */

    /* Add unit to scene operator.
     * ARGUMENTS:
     *   - name of the unit:
     *       const std::string &Name;
     * RETURNS:
     *   (scene &) Self-reference.
     */
    scene & operator<<( const std::string &Name );
  }; /* End of 'scene' class */

  /* Animation class */
  class animation : public win_main, public render, public timer, public input
  {
  private:
    /* Hiden constructor */
    animation( VOID ) : render(win_main::hWnd, win_main::W, win_main::H), input(win_main::MouseWheel, win_main::hWnd), Game()
    {
    } /* End of constructor */

    static animation Instance; //Instance of animation interface
  public:    
    tetris Game;

    /* Destructor */
    ~animation( VOID )
    {
    } /* End of destructor */

    /* Get single instance of animation interface function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (animation&) instance of animation interface.
     */
    static animation & Get( VOID )
    {
      return Instance;
    } /* End of 'Get' function */

    /* Get pointer to single instance of animation interface function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (animation*) pointer to instance of animation interface.
     */
    static animation * GetPtr( VOID )
    {
      return &Instance;
    } /* End of 'GetPtr' function */

    /* Add unit to animation stock operator.
     * ARGUMENTS:
     *   - name of the unit:
     *       const std::string &Name;
     * RETURNS:
     *   (animation &) Self-reference.
     */
    animation & operator<<( const std::string &Name )
    {
      //*Scene << Name;
      return *this;
    } /* End of 'operator<<' function */

  private:
    /* Render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID );

    /* Resize window function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID AnimResize( VOID ) override;

    /* Draw animation function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID AnimDraw( VOID ) override;
  }; /* End of 'animation' class */
} /* end of 'bodx' name space */

#endif /* __animation_h_ */

/* END OF 'animation.h' FILE */