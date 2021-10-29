/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : reader.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Utilities.
 *               Binary reader.
 * PROGRAMMER  : CGSG'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 02.10.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <fstream>
#include <windows.h>

#ifndef __reader_h_
#define __reader_h_

/* Project name space */
namespace bodx
{
  /* Unstructured data file reader class */
  class reader
  {
  private:
    // Memory pointer reference
    BYTE *Mem, *Ptr;
    std::fstream f;
    BOOL IsOpen;
  public:
    INT length;
    /* Class constructor.
     * ARGUMENTS:
     *   - texture name:
     *       const std::wstring &FileName;
     */
    reader( const std::wstring &FileName ) :
      f(FileName, std::fstream::in | std::fstream::binary),
      Mem{}, Ptr{}, IsOpen(f.is_open()), length(0)
    {
      if (IsOpen)
      {
        f.seekg(0, std::fstream::end);
        length = (INT)f.tellg();
        f.seekg(0, std::fstream::beg);
        Mem = new BYTE[length];
        f.read((CHAR *)Mem, length);
        f.close();
        Ptr = Mem;
      }
    } /* End of 'rdr' function */

    BOOL is_open( VOID )
    {
      return IsOpen;
    }

    ~reader( VOID )
    {
      if (Mem != nullptr)
        delete[] Mem;
    }
 
    /* Read data function.
     * ARGUMENT:
     *   - read data pointer:
     *       type *Data;
     *   - read data count:
     *       INT Count;
     * RETURNS: None.
     */
    template<typename type>
     VOID operator()( type *Data, const INT Count = 1 )
        {
          if (Count == 1)
            *Data = *(type *)Ptr, Ptr += sizeof(type);
          else
            memcpy(Data, Ptr, sizeof(type) * Count), Ptr += sizeof(type) * Count;
        } /* End of 'operator()' function */
  }; /* End of 'rdr' class */
} /* end of 'bodx' namespace */

#endif // __reader_h_

/* END OF 'reader.h' FILE */