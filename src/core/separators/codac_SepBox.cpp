/** 
 *  SepBox class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_SepBox.h"

namespace codac
{
  void SepBox::separate(IntervalVector& x_in, IntervalVector& x_out)
  {
    assert(m_b.size() == x_in.size());
    assert(m_b.size() == x_out.size());

    // Cout
    x_out &= m_b;

    // Cin
    IntervalVector intermediate_x_in(IntervalVector::empty(x_in.size()));
    for (int k=0; k<x_in.size(); ++k) {
      IntervalVector sx(x_in);
      if (!sx[k].is_subset(m_b[k])) {
        sx[k] = (sx[k] & Interval(NEG_INFINITY, m_b[k].lb())) | (sx[k] & Interval(m_b[k].ub(), POS_INFINITY));
        intermediate_x_in |= sx;
      }
    }
    x_in &= intermediate_x_in;
  }
}