/** 
 *  \file
 *  CtcCartProd class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCCARTPROD_H__
#define __CODAC_CTCCARTPROD_H__

#include "codac_Ctc.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \class CtcCartProd
   * \brief Cartesian product of two contractors
   */
  template <typename T1, typename T2>
  class CtcCartProd : public Ctc
  {
    public:

      /**
       * \brief Creates the contractor based on the cartesian product of the two others.
       * 
       * \param C1 The first cotractor
       * \param C2 The second contractor
       */
      CtcCartProd(T1 &C1, T2 &C2) : Ctc(C1.nb_var + C2.nb_var), C1(C1), C2(C2) {};

      /**
       * \brief \f$\mathcal{C}_{\mathcal{C_1}\times\mathcal{C_2}}\big([\mathbf{x}]\big)\f$
       *
       * \param x the (n+m)-d box of domains
       */
      void contract(IntervalVector& x);

      /**
       * \param C1 the first contractor
       */
      Ctc &C1;

      /**
       * \param C2 the second contractor
       */
      Ctc &C2;

  };

  template <typename T1, typename T2>
  void CtcCartProd<T1, T2>::contract(IntervalVector& x)
  {
    // Checking the dimension of the input box
    assert(C1.nb_var + C2.nb_var == x.size());

    // Contracting the projection of x along the n first dimensions using C1
    IntervalVector x1 = x.subvector(0, C1.nb_var);
    C1.contract(x1);
    x.put(0, x1);

    // Contracting the projeciton of x over the m last dimensions using C2
    IntervalVector x2 = x.subvector(C1.nb_var, C1.nb_var+C2.nb_var);
    C2.contract(x2);
    x.put(C1.nb_var, x2);
  }

  /**
   * \fn template <typename T1, typename T2> CtcCartProd<T1, T2> cart_prod(T1 &C1, T2 &C2)
   * \brief Cartesian product of two contractors
   * 
   * \param C1 the first contractor
   * \param C2 the second contractor
   */
  template <typename T1, typename T2>
  CtcCartProd<T1, T2> cart_prod(T1 &C1, T2 &C2)
  {
    return CtcCartProd<T1, T2>(C1, C2);
  }
}

#endif