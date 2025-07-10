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
  bool contains (const vector<OctaSym>& symmetries, const OctaSym& symmetry, const AnalyticFunction<VectorType>& psi_0);
  
  vector<OctaSym> generate_symmetries (const vector<vector<int>>& generators, const AnalyticFunction<VectorType>& psi_0);

  double error(const IntervalMatrix& JJf, const IntervalMatrix& JJf_punc, const AnalyticFunction<VectorType>& psi_0, const OctaSym& symmetry, const IntervalVector& X);

  double split (const IntervalVector& X, double eps, vector<IntervalVector>& boxes);

  Matrix inflate_flat_parallelepiped (const Matrix& Jz, double epsilon, double rho);

  Parallelepiped parallelepiped_inclusion(const Vector& z, const IntervalMatrix& JJf, const IntervalMatrix& JJf_point, const AnalyticFunction<VectorType>& psi_0, const OctaSym& symmetry, const IntervalVector& X, double true_eps);

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, bool verbose = false);
  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset, bool verbose = false);  
}