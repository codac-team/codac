/** 
 *  \file codac2_peibos.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Parallelepiped.h"
#include "codac2_AnalyticFunction.h"
#include "codac2_OctaSym.h"
#include "codac2_Figure2D.h"
#include "codac2_Figure3D.h"

namespace codac2
{
  double distance_from_line_to_origin(Eigen::Matrix<double,3,1> a, Eigen::Matrix<double,3,1> b);
  bool contains (vector<OctaSym> symmetries, OctaSym symmetry, const AnalyticFunction<VectorType>& psi_0);
  vector<OctaSym> generate_symmetries (vector<vector<int>> generators, const AnalyticFunction<VectorType>& psi_0);

  double error(IntervalMatrix JJf, IntervalMatrix JJf_punc, const AnalyticFunction<VectorType>& psi_0, OctaSym symmetry, IntervalVector X);

  double split (IntervalVector X, double eps, const vector<IntervalVector>& boxes);

  Matrix inflate_flat_parallelepiped (Matrix Jz, double epsilon, double rho);

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon);
  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset);  
}