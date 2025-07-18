/** 
 *  \file codac2_IntervalMatrix.h
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
   * \brief Alias for a dynamic-size matrix of intervals.
   *
   * Represents a matrix with a dynamic number of rows and columns,
   * where each element is an Interval object.
   *
   * This type alias is based on Eigen's matrix template and corresponds to ``Eigen::Matrix<Interval,-1,-1>``.
   */
  using IntervalMatrix = Eigen::Matrix<Interval,-1,-1>;

  /**
   * \brief Stream output operator for ``IntervalMatrix`` objects.
   *
   * \param os The output stream to write to.
   * \param x The interval matrix whose contents are to be printed.
   * \return A reference to the modified output stream.
   */
  inline std::ostream& operator<<(std::ostream& os, const IntervalMatrix& x)
  {
    if(x.is_empty())
      return os << "[ empty " << x.rows() << "x" << x.cols() << " mat ]";

    else
    {
      os << x.format(codac_matrix_fmt());
      return os;
    }
  }
}