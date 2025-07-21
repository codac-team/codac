/** 
 *  \file codac2_IntervalRow.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_matrices.h"

namespace codac2
{
  /**
   * \brief Alias for a dynamic-size row vector of intervals.
   *
   * Represents a row vector with a dynamic number of columns,
   * where each element is an Interval object.
   *
   * This type alias is based on Eigen's matrix template and corresponds to ``Eigen::Matrix<Interval,1,-1>``.
   */
  using IntervalRow = Eigen::Matrix<Interval,1,-1>;

  /**
   * \brief Stream output operator for ``IntervalRow`` objects.
   *
   * \param os The output stream to write to.
   * \param x The interval row whose contents are to be printed.
   * \return A reference to the modified output stream.
   */
  inline std::ostream& operator<<(std::ostream& os, const IntervalRow& x)
  {
    if(x.is_empty())
      return os << "[ empty row ]";

    else
    {
      os << x.format(codac_row_fmt());
      return os;
    }
  }
}