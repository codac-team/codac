/** 
 *  \file codac2_Row.h
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
   * \brief Alias for a dynamically-sized row vector of doubles.
   *
   * Defines a convenient shorthand for representing row vectors with dynamic size.
   * This type alias is based on Eigen's matrix template and corresponds to ``Eigen::Matrix<double,1,-1>``.
   *
   * \see Eigen::Matrix
   */
  using Row = Eigen::Matrix<double,1,-1>;

  /**
   * \brief Stream output operator for ``Row`` objects.
   *
   * \param os The output stream to write to.
   * \param x The row whose contents are to be printed.
   * \return A reference to the modified output stream.
   */
  inline std::ostream& operator<<(std::ostream& os, const Row& x)
  {
    os << x.format(codac_row_fmt());
    return os;
  }
}