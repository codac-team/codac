/** 
 *  \file codac2_Matrix.h
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
   * \brief Alias for a dynamic-size matrix of doubles.
   *
   * Represents a matrix with dynamic number of rows and columns, where
   * each element is a double precision floating point number.
   *
   * This type alias is based on Eigen's matrix template and corresponds to ``Eigen::Matrix<double,-1,-1>``.
   *
   * \see Vector, Row, Eigen::Matrix
   */
  using Matrix = Eigen::Matrix<double,-1,-1>;

  /**
   * \brief Stream output operator for ``Matrix`` objects.
   *
   * \param os The output stream to write to.
   * \param x The matrix whose contents are to be printed.
   * \return A reference to the modified output stream.
   */
  inline std::ostream& operator<<(std::ostream& os, const Matrix& x)
  {
    os << x.format(codac_matrix_fmt());
    return os;
  }
}