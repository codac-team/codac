/** 
 *  \file codac2_Ellipsoid_utils.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ellipsoid.h"

namespace codac2
{
  /**
   * \brief ...
   *
   * \param f ...
   * \param alpha_max ...
   * \param e ...
   * \param e_out ...
   * \return ...
   */
  BoolInterval stability_analysis(const AnalyticFunction<VectorOpValue>& f, int alpha_max, Ellipsoid& e, Ellipsoid& e_out);

  /**
   * \brief ...
   *
   * \param a ...
   * \param q ...
   * \return ...
   */
  Matrix solve_discrete_lyapunov(const Matrix& a, const Matrix& q);
}