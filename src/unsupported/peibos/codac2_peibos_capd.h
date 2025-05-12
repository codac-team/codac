/** 
 *  \file codac2_peibos_capd.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_peibos.h"

#include <codac2_capd.h>
#include <capd/capdlib.h>

namespace codac2
{

  vector<Parallelepiped> PEIBOS2D(capd::IMap& gamma, double tf, AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset = Vector::Zero(2));
  vector<Parallelepiped> PEIBOS3D(capd::IMap& gamma, double tf, AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset = Vector::Zero(3));

  vector<Parallelepiped> PEIBOS(capd::IMap& gamma, double tf, AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon);
  vector<Parallelepiped> PEIBOS(capd::IMap& gamma, double tf, AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset);

}