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
       * \brief Creates the contractor based on the Cartesian product of the two others.
       * 
       * \param c1 The first cotractor
       * \param c2 The second contractor
       */
      CtcCartProd(T1 &c1, T2 &c2) : Ctc(c1.nb_var + c2.nb_var), m_c1(c1), m_c2(c2) {};

      /**
       * \brief \f$\mathcal{C}_{\mathcal{C_1}\times\mathcal{C_2}}\big([\mathbf{x}]\big)\f$
       *
       * \param x the (n+m)-d box of domains
       */
      void contract(IntervalVector& x);


    protected:

      Ctc &m_c1; //!< the first contractor
      Ctc &m_c2; //!< the second contractor
  };

  template <typename T1, typename T2>
  void CtcCartProd<T1, T2>::contract(IntervalVector& x)
  {
    // Checking the dimension of the input box
    assert(m_c1.nb_var + m_c2.nb_var == x.size());

    // Contracting the projection of x along the n first dimensions using m_c1
    IntervalVector x1 = x.subvector(0, m_c1.nb_var);
    m_c1.contract(x1);
    x.put(0, x1);

    // Contracting the projeciton of x over the m last dimensions using m_c2
    IntervalVector x2 = x.subvector(m_c1.nb_var, m_c1.nb_var+m_c2.nb_var);
    m_c2.contract(x2);
    x.put(m_c1.nb_var, x2);
  }

  /**
   * \fn template <typename T1, typename T2> CtcCartProd<T1, T2> cart_prod(T1 &c1, T2 &c2)
   * \brief Cartesian product of two contractors
   * 
   * \param c1 the first contractor
   * \param c2 the second contractor
   * \return the Cartesian product of c1 and c2
   */
  template <typename T1, typename T2>
  CtcCartProd<T1, T2> cart_prod(T1 &c1, T2 &c2)
  {
    return CtcCartProd<T1, T2>(c1, c2);
  }
}

#endif