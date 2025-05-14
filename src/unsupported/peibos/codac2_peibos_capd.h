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

  vector<Parallelepiped> PEIBOS(const capd::IMap& gamma, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon);
  vector<Parallelepiped> PEIBOS(const capd::IMap& gamma, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset);

}