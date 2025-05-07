/** 
 *  \file codac2_transformations.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_Vector.h"
#include "codac2_SampledTraj.h"

namespace codac2
{
  Eigen::Affine2d affine_transformation(const std::vector<Vector>& src, const std::vector<Vector>& dst);
  Eigen::Affine2d affine_transformation(const SampledTraj<Vector>& src, const SampledTraj<Vector>& dst);
}