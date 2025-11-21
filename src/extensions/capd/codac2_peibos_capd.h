/** 
 *  \file codac2_peibos_capd.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_peibos.h"
#include "codac2_capd.h"
#include "codac2_OctaSym.h"
#include <capd/poincare/lib.h>

namespace codac2
{
  struct PEIBOS_CAPD_Key
  {
    IntervalVector box;
    AnalyticFunction<VectorType> psi_0;
    OctaSym sigma;
    Vector offset;
  };

  std::vector<std::pair<PEIBOS_CAPD_Key,std::pair<capd::ITimeMap::SolutionCurve,capd::ITimeMap::SolutionCurve>>> PEIBOS(const capd::IMap& i_map, double tf, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, bool verbose = false);
  std::vector<std::pair<PEIBOS_CAPD_Key,std::pair<capd::ITimeMap::SolutionCurve,capd::ITimeMap::SolutionCurve>>> PEIBOS(const capd::IMap& i_map, double tf, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const Vector& offset, bool verbose = false);

  std::vector<Parallelepiped> reach_set(const std::vector<std::pair<PEIBOS_CAPD_Key,std::pair<capd::ITimeMap::SolutionCurve,capd::ITimeMap::SolutionCurve>>>& peibos_output, double t);
}