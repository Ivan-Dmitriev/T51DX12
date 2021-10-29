/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : stock.h
 * PURPOSE     : Animation DirectX 12 project.
 *               Animation system.
 *               Stock storage.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 26.07.2021.
 * NOTE        : Module namespace 'bodx'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __stock_h_
#define __stock_h_

/* Project name space */
namespace bodx
{
  /* Stock storage class */
  template<typename type>
  class stock : public std::vector<type>
  {
  public:
    /* Add data to stock function.
     * ARGUMENTS:
     *   - data to add:
     *       const type &NewData;
     * RETURNS: (stock &) Self-reference.
     */
    stock & Add( const type &NewData )
    {
      this->push_back(NewData);
      return *this;
    } /* End of 'Add' function */

    /* Add operator.
     * ARGUMENTS:
     *   - data to add:
     *       const type& NewData;
     * RETURNS: (stock &) Self-reference.
     */
    stock & operator<<( const type &NewData )
    {
      return Add(NewData);
    } /* End of 'operator<<' function */
 
    /* Call Walker function for each element of the stock.
     * ARGUMENTS:
     *   - walker function:
     *       const type2 &Walker;
     * RETURNS: None.
     */
    template<class WalkType>
    void Walk( WalkType Walk )
    {
      for (auto &x : *this)
        Walk(x);
    } /* End of 'Walk' function */
  }; /* End of 'stock' class */
} /* end of 'bodx' name space */

#endif /* __stock_h_ */

/* END OF 'stock.h' FILE */