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

  /**
   * \struct VectorCompare
   * \brief Comparison functor for ``codac2::Vector`` objects.
   *
   * This functor defines a strict weak ordering on ``codac2::Vector`` objects so
   * that they can be used as keys in associative containers such as ``std::map``.
   * The ordering is based on increasing real values, inspected sequentially from
   * index 0 to the last. If all corresponding components are equal, the operator
   * returns ``false`` to indicate that neither vector is strictly less than the other.
   */
  struct VectorCompare
  {
    /**
     * \brief Defines an increasing-order comparison between two vectors of real numbers.
     *
     * \param a The first vector to compare.
     * \param b The second vector to compare.
     * \return ``true`` if ``a`` is considered strictly smaller than ``b``,
     *         ``false`` otherwise.
     */
    bool operator()(const Vector& a, const Vector& b) const
    {
      if(a.size() != b.size())
        return a.size() < b.size();

      for(Index i = 0 ; i < a.size() ; i++)
        if(a[i] != b[i])
          return a[i] < b[i];

      return false;
    }
  };
}