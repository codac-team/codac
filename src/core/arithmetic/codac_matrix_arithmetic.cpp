/** 
 *  Arithmetic operations on matrices
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_matrix_arithmetic.h"

using namespace std;
using namespace ibex;

namespace codac
{
  const IntervalMatrix operator&(const IntervalMatrix& m1, const IntervalMatrix& m2)
  {    
    assert(m1.nb_cols() == m2.nb_cols() && m1.nb_rows() == m2.nb_rows());
    IntervalMatrix m3(m1);

    for(int i = 0; i < m1.nb_rows(); i++)
      for(int j = 0; j < m2.nb_cols(); j++)
        m3[i][j] &= m2[i][j];

    return m3;
  }

  const IntervalMatrix operator|(const IntervalMatrix& m1, const IntervalMatrix& m2)
  {    
    assert(m1.nb_cols() == m2.nb_cols() && m1.nb_rows() == m2.nb_rows());
    IntervalMatrix m3(m1);

    for(int i = 0; i < m1.nb_rows(); i++)
      for(int j = 0; j < m2.nb_cols(); j++)
        m3[i][j] |= m2[i][j];

    return m3;
  }
}