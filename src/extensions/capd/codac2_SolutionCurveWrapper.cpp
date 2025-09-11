/**
 *  codac2_SolutionCurveWrapper.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SolutionCurveWrapper.h"

using namespace std;

using namespace std;
using namespace codac2;

namespace codac2
{

  SolutionCurveWrapper::SolutionCurveWrapper(const capd::ITimeMap::SolutionCurve& base) : capd::ITimeMap::SolutionCurve(base)
  {}

  std::vector<capd::Interval> SolutionCurveWrapper::get_domain() const
  {
    return this->domain;
  }

  double SolutionCurveWrapper::t0() const
  {
    assert_release(this->domain.size() > 0 && "SolutionCurveWrapper::t0() - no pieces in the curve.");
    return this->domain.front().rightBound();
  }

  double SolutionCurveWrapper::tf() const
  {
    assert_release(this->domain.size() > 0 && "SolutionCurveWrapper::tf() - no pieces in the curve.");
    return this->domain.back().leftBound();
  }

  int SolutionCurveWrapper::dimension() const
  {
    assert_release(this->pieces.size() > 0 && "SolutionCurveWrapper::dimension() - no pieces in the curve.");
    return this->pieces[0]->dimension();
  }

}
