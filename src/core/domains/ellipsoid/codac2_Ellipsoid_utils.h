/** 
 *  \file codac2_Ellipsoid_utils.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louedec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Ellipsoid.h"

namespace codac2
{
  BoolInterval stability_analysis(const AnalyticFunction<VectorOpValue> &f, int alpha_max, Ellipsoid &e, Ellipsoid &e_out);

  Matrix solve_discrete_lyapunov(const Matrix& a,const Matrix& q);
}