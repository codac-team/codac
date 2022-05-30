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

#include <functional>
#include "codac2_TubeVectorComponent.h"
#include "codac2_TubeVector.h"
#include "codac2_SliceVector.h"
#include "ibex_Interval.h"

using namespace std;

namespace codac2
{
  TubeVectorComponent::TubeVectorComponent(TubeVector& tubevector, size_t i) :
    _i(i), _tubevector(tubevector)
  {
    assert(i >= 0 && i < tubevector.size());
  }

  TubeVectorComponent::TubeVectorComponent(const TubeVectorComponent& tubevector_i) :
    _i(tubevector_i._i), _tubevector(tubevector_i._tubevector)
  {

  }
  
  size_t TubeVectorComponent::size() const
  {
    return 1;
  }

  const TDomain& TubeVectorComponent::tdomain() const
  {
    return _tubevector.tdomain();
  }
  
  Interval TubeVectorComponent::t0_tf() const
  {
    return _tubevector.t0_tf();
  }
  
  Interval TubeVectorComponent::codomain() const
  {
    Interval codomain(Interval::EMPTY_SET);
    for(const auto& s : _tubevector)
      codomain |= s.codomain()[_i];
    return codomain;
  }

  bool TubeVectorComponent::contains(const Trajectory& value) const
  {
    assert(false);
    return true;
  }
  
  void TubeVectorComponent::set(const Interval& codomain)
  {
    for(auto& s : _tubevector)
      s._codomain[_i] = codomain;
  }

  const TubeVectorComponent& TubeVectorComponent::operator=(const TubeVectorComponent& x)
  {
    assert(&x.tdomain() == &tdomain());
    for(auto& s : _tubevector)
      s._codomain[_i] = s._it_tslice->_slices.at(&x._tubevector)._codomain[x._i];
    return *this;
  }

  const TubeVectorComponent& TubeVectorComponent::operator=(pair<function<Interval(const Interval&)>,const TubeVectorComponent> rel)
  {
    assert(&rel.second.tdomain() == &tdomain());
    for(auto& s : _tubevector)
      s._codomain[_i] = rel.first(s._it_tslice->_slices.at(&rel.second._tubevector)._codomain[_i]);
    return *this;
  }

  ostream& operator<<(ostream& os, const TubeVectorComponent& x)
  {
    os << "Component " << x._i << " of: " << x._tubevector << flush;
    return os;
  }

  pair<function<Interval(const Interval&)>,const TubeVectorComponent> cos(const TubeVectorComponent& x)
  {
    return make_pair(static_cast<Interval(*)(const Interval&)>(ibex::cos), x);
  }
  
  codac::Tube TubeVectorComponent::to_codac1() const
  {
    codac::Tube x(t0_tf());
    for(const auto& s : _tubevector)
      if(!s.t0_tf().is_unbounded())
        x.set(s.codomain()[_i], s.t0_tf());
    for(const auto& s : _tubevector) // setting gate (were overwritten)
      if(s.t0_tf().is_degenerated())
        x.set(s.codomain()[_i], s.t0_tf());
    return x;
  }

} // namespace codac