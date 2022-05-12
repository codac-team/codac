/** 
 *  \file
 *  CtcCartProd class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCCARTPROD_H__
#define __CODAC_CTCCARTPROD_H__

#include <vector>
#include <numeric>
#include <type_traits>

#include "codac_Ctc.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "ibex_Array.h"

namespace codac
{
  /**
   * \class CtcCartProd
   * \brief Cartesian product of contractors \f$\mathcal{C_1}\times\dots\times\mathcal{C_n}\f$
   */
  template <typename ...Args>
  class CtcCartProd : public Ctc
  {
    public:

      /**
       * \brief Creates the contractor based on the Cartesian product of other.
       * 
       * \param args list of contractors
       */
      CtcCartProd(Args &... args) : Ctc(ibex::Array<Ctc>({args...})), m_v({args...}) {};

      /**
       * \brief \f$\mathcal{C}_{\mathcal{C_1}\times\dots\times\mathcal{C_n}}\big([\mathbf{x}]\big)\f$
       *
       * \param x a m-d box of domains
       */
      void contract(IntervalVector& x);

    protected:

      ibex::Array<Ctc> m_v; //!< vector containing the contractors
  };

  template <typename ...Args>
  void CtcCartProd<Args...>::contract(IntervalVector& x)
  {
    std::size_t index = 0;
    for (int i=0; i < m_v.size(); i++) {
      IntervalVector sx(m_v[i].nb_var);
      for (int k=0; k < m_v[i].nb_var; k++) {
        sx[k] = x[index + k];
      }
      m_v[i].contract(sx);
      x.put(index, sx);
      index += m_v[i].nb_var;
    }
  }

  /**
   * \fn template <typename ...Args> CtcCartProd<Args...> cart_prod(Args &...args)
   * \brief Cartesian product of contractors
   * 
   * \param args list of contractors
   * \return the Cartesian product of the contractors \f$\mathcal{C_1}\times\dots\times\mathcal{C_n}\f$
   */
  template <typename ...Args>
  CtcCartProd<Args...> cart_prod(Args &...args)
  {
    return CtcCartProd<Args...>(args...);
  }
}

#endif