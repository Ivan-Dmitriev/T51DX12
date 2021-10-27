/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : def.h
 * PURPOSE     : Animation project.
 *               Common definitions handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __def_h_
#define __def_h_

#pragma comment(lib, "opengl32")

#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>

#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else
#include <commondf.h>
#endif

#include <utility>
#include <vector>
#include <cassert>
#include <string>

#include "mth/mth.h"

typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( VOID ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;
#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

/* Project namespace */
namespace ivgl
{
  typedef math::vec3<FLT> vec3;
  typedef math::matr<FLT> matr;
  typedef math::vec2<FLT> vec2;
  typedef math::vec4<FLT> vec4;
}

template<typename type>
  /* Stock system class */
  class stock
  {
  private:
    // Array of values
    std::vector<type> Array; 
    //type *Array;
 
  public:
    /* Default constructor of stock function.
    * ARGUMENTS: NONE.
    * RETURNS: NONE.
    */
    stock( VOID )
    {
    } /* End of 'default constructor' function */

    /* Destructor of stock function.
    * ARGUMENTS: NONE.
    * RETURNS: NONE.
    */
    ~stock( VOID )
    {
    } /* End of 'destructor' function */
 
    /* Copy constructor of stock function.
    * ARGUMENTS: 
    *   - Data to be copied:
    *       const stock<type> &CopyData;
    * RETURNS: NONE.
    */
    stock( const stock<type> &CopyData )
    {
      for (INT i = 0; i < CopyData.Array.size(); i++)
        Array[i] = CopyData.Array[i];
    } /* End of 'copy constructor' function */
 
    /* Overload equal operator of stock function.
    * ARGUMENTS: 
    *   - Data to be copied:
    *       const stock<type> &CopyData;
    * RETURNS: (stock &) link on created stock.  
    */
    stock & operator=( const stock<type> &CopyData )
    {
      //Array(CopyData.RealSize);
      for (INT i = 0; i < CopyData.Array.size(); i++)
        Array[i] = CopyData.Array[i];
      return *this;
    } /* End of 'operator=' function */
 
    /* Add new member to stock function.
    * ARGUMENTS: 
    *   - Data to be added:
    *       const type &NewData;
    * RETURNS: (stock &) link on created stock.
    */
    stock & Add( const type &NewData )
    {
      this->Array.push_back(NewData);
      return *this;
    } /* End of 'Add' function */

    /* Overload << operator of stock function.
    * ARGUMENTS: 
    *   - Data to be added:
    *       - const type &NewData;
    * RETURNS: (stock &) link on created stock.
    */
    stock & operator<<( const type &NewData )
    {
      return Add(NewData);
    } /* End of 'operator<<' function */
 
    /* Get element by index in stock function.
    * ARGUMENTS: 
    *   - Index of element in stock:
    *       - INT Index;
    * RETURNS: (type &) link on result value.
    */
    type & operator[]( INT Index )
    {
      assert(Index >= 0 && Index < this->Array.size);
      return Array[Index];
    } /* End of 'operator[]' function */

    /* Get element by index in stock function.
    * ARGUMENTS: 
    *   - Index of element in stock:
    *       - INT Index;
    * RETURNS: (type &) link on result value.
    */
    type & operator[]( INT Index ) const
    {
      assert(Index >= 0 && Index < this->Array.size);
      return Array[Index];
    } /* End of 'operator[]' function */
 
    /* Clear stock function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Clear( VOID )
    {
      this->Array.size = 0;
    } /* End of 'Clear' function */
 
    template<typename type2>
      /* Walk on stock function.
      * ARGUMENTS: 
      *   - function which is called on each stock member:
      *       type2 &Walker;
      * RETURNS: None.  
      */
      VOID Walk( const type2 &Walker )
      {
        for (INT i = 0; i < this->Array.size(); i++)
          Walker(Array[i]);
      } /* End of 'Walk' function */
  }; /* End of 'stock' class */

#endif /* __def_h_ */

/* END OF 'def.h' FILE */