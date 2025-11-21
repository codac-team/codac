/** 
 *  \file codac2_peibos_tools.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Parallelepiped.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  /**
   * \brief Recursively split a box until the maximum diameter of each box is less than eps. Note that the resulting boxes are stored in place in the vector boxes.
   * 
   * \param X The box to split
   * \param eps The maximum diameter of each box
   * \param boxes The vector to store the resulting boxes
   * 
   * \return The maximum diameter of the resulting boxes (lower or equal to eps)
   */
  double split(const IntervalVector& X, double eps, std::vector<IntervalVector>& boxes);

  /**
   * \brief Compute the error term for the parallelepiped inclusion. The error is later used to inflate the flat parallelepiped \f$\mathbf{z}+\mathbf{A}\cdot(\left[\mathbf{x}\right]-\bar{\mathbf{x}})\f$
   * 
   * \param Y The box enclosing \f$\mathbf{f}(\bar{\mathbf{x}})\f$
   * \param z Any Vector with the same dimension as z. Usually we pick an approximation of \f$\mathbf{f}(\bar{\mathbf{x}})\f$
   * \param Jf The interval Jacobian matrix containing \f$\frac{d\mathbf{f}}{d\mathbf{x}}([\mathbf{x}])\f$
   * \param A Any Matrix with the same dimension as Jf. Usually we pick an approximation of \f$\frac{d\mathbf{f}}{d\mathbf{x}}(\bar{\mathbf{x}})\f$
   * \param X The box \f$[\mathbf{x}]\f$
   */
  double error_peibos(const IntervalVector& Y, const Vector& z, const IntervalMatrix& Jf, const Matrix& A, const IntervalVector& X);

  /**
   * \brief Inflate the flat parallelepiped \f$\mathbf{A}\cdot e_{vec}\f$ by \f$\rho\f$.
   * 
   * \param A The shape matrix of the flat parallelepiped (n x m matrix with m < n)
   * \param e_vec The vector of scaling along each generator of the flat parallelepiped (m-dimensional vector)
   * \param rho The inflation factor
   * 
   * \return The shape matrix of the inflated parallelepiped (n x n matrix)
   */
  Matrix inflate_flat_parallelepiped(const Matrix& A, const Vector& e_vec, double rho);

}