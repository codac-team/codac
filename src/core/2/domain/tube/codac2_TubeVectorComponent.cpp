/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_TubeVectorComponent.h"
#include "codac2_TubeVector.h"
#include "codac2_SliceVector.h"

using namespace std;

namespace codac2
{
  /*const Interval_& TubeVectorComponent::tdomain() const
  {
    return m_tubevector.tdomain();
  }

  bool TubeVectorComponent::contains(const Trajectory_& value) const
  {
    return true;
  }

  TubeVectorComponent::TubeVectorComponent(T_TubeVector& tubevector, size_t i) :
    m_i(i), m_tubevector(tubevector)
  {
    assert(i >= 0 && i < tubevector.size());

  }
  
  void TubeVectorComponent::set_codomain(const Interval_& codomain)
  {
    for(auto& s : m_tubevector.m_slices)
      s->m_codomain[m_i] = codomain;
  }*/

} // namespace codac