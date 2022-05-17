/** 
 *  CtcCartProd class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcCartProd.h"

using namespace std;

namespace codac
{
  void CtcCartProd::contract(IntervalVector& x)
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

  CtcCartProd cart_prod(const ibex::Array<Ctc>& array)
  {
    return CtcCartProd(array);
  }
}