/** 
 *  \file codac2_IntervalVector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_matrices.h"
#include "codac2_Vector.h"

namespace codac2
{
  /**
   * \brief Alias for a dynamic-size column vector of intervals.
   *
   * Represents a column vector with a dynamic number of rows,
   * where each element is an Interval object.
   *
   * This type alias is based on Eigen's matrix template and corresponds to ``Eigen::Matrix<Interval,-1,1>``.
   */
  using IntervalVector = Eigen::Matrix<Interval,-1,1>;

  /**
   * \brief Stream output operator for ``IntervalVector`` objects.
   *
   * \param os The output stream to write to.
   * \param x The interval vector whose contents are to be printed.
   * \return A reference to the modified output stream.
   */
  inline std::ostream& operator<<(std::ostream& os, const IntervalVector& x)
  {
    if(x.is_empty())
      return os << "[ empty vector ]";

    else
    {
      os << x.format(codac_vector_fmt());
      return os;
    }
  }
}