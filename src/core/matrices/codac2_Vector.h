/** 
 *  \file codac2_Vector.h
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
   * \brief Alias for a dynamically-sized column vector of doubles.
   *
   * Defines a convenient shorthand for representing column vectors with dynamic size.
   * This type alias is based on Eigen's matrix template and corresponds to ``Eigen::Matrix<double,-1,1>``.
   *
   * \see Eigen::Matrix
   */
  using Vector = Eigen::Matrix<double,-1,1>;

  /**
   * \brief Stream output operator for ``Vector`` objects.
   *
   * \param os The output stream to write to.
   * \param x The vector whose contents are to be printed.
   * \return A reference to the modified output stream.
   */
  inline std::ostream& operator<<(std::ostream& os, const Vector& x)
  {
    os << x.format(codac_vector_fmt());
    return os;
  }
}