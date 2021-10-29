/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : model.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Render system.
 *               Resources.
 *               Uniform buffers.
 * PROGRAMMER  : Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __buffers_h_
#define __buffers_h_

#include "resource.h"

/* Project name space */
namespace bodx
{
  // class buffers
  class buffer
  {
    friend class buffer_manager;
    template<typename entry_type, typename index_type>
    friend class resource_manager;
    friend class material;
  private:
    UINT Id;        // open gl id
    UINT BindPoint; // binding point for shader
    INT NumOfQuads; // number of vec4

    /* Create buffer function
     * ARGUMENTS: 
     *   - binding point:
     *       UINT BindingPoint;
     *   - data structure:
     *       const data_structure *Ptr;
     * RETURNS:
     *   (buffer &) pointer to buffer
     */
    template<typename data_structure>
      buffer & Create( UINT BindingPoint, const data_structure *Ptr )
      {
        Free();
        BindPoint = BindingPoint;
        NumOfQuads = sizeof(*Ptr) / 16;

        glGenBuffers(1, &Id);
        glBindBuffer(GL_UNIFORM_BUFFER, Id);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(*Ptr), Ptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        Apply();
        return *this;
      } // end of 'bodx::buffer::Create' function
  public:
    /* default constructor */
    buffer( VOID ) : Id(0), BindPoint(0), NumOfQuads(0)
    {
    } // end of default consructor

    /* Apply buffers 
     * ARGUMENTS: None.
     * RETURNS: None.
     */ 
    VOID Apply( VOID );
   
    /* Update buffers 
     * ARGUMENTS:
     *   - data structure:
     *       const data_structure &Ptr;
     * RETURNS: None.
     */ 
    template<typename data_structure>
      VOID Update( const data_structure *Ptr )
      {
        glBindBuffer(GL_UNIFORM_BUFFER, Id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(*Ptr), Ptr);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
      } // end of 'bodx::buffer::Update' function
   
    /* Free buffers 
     * ARGUMENTS: None.
     * RETURNS: None.
     */ 
     VOID Free( VOID );

     /* Denstructor */
     ~buffer( VOID )
     {
     }
  };

  class buffer_manager : public resource_manager<buffer>
  {

  protected:
    /* Create buffer function
     * ARGUMENTS: 
     *   - binding point:
     *       UINT BindingPoint;
     *   - data structure:
     *       const data_structure *Ptr;
     *   - number of vec4;
     * RETURNS:
     *   (buffer &) pointer to buffer
     */
    template<typename data_structure>
      buffer * CreateBuffer( UINT BindingPoint, const data_structure *Ptr )
      {
        return Add(buffer().Create(BindingPoint, Ptr));
      } /* End of 'bodx::buffer_manager::CreateBuffers' function */

    /* Update buffers 
     * ARGUMENTS:
     *   - data structure:
     *       const data_structure &Ptr;
     * RETURNS: None.
     */
    //template<typename data_structure>
    //  VOID UpdateBuffers( const data_structure &Ptr )
    //  {
    //    for (auto &b : Stock)
    //      b.second.Update(Ptr);
    //  } /* End of 'bodx::buffer_manager::UpdateBuffers' function */
  };
} // end of namespace

#endif /* __buffers_h_ */

/* END OF 'buffers.h' FILE */
