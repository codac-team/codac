/** 
 *  \file codac2_SepProj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"
#include "codac2_ProjBase.h"

namespace codac2
{
  class SepProj : public Sep<SepProj>, protected ProjBase
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepProj(const S& s, const std::vector<size_t>& proj_indices, double default_eps = 0.01)
        : SepProj(s, proj_indices, IntervalVector(size_of(s)-proj_indices.size()), default_eps)
      { }

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepProj(const S& s, const std::vector<size_t>& proj_indices, const IntervalVector& y, double default_eps = 0.01)
        : Sep<SepProj>(proj_indices.size()), ProjBase(proj_indices,y,default_eps), _sep(s)
      {
        assert_release(_y.size() == size_of(s)-_xi.size());
        assert_release(*min_element(_xi.begin(),_xi.end()) >= 0);
        assert_release(*max_element(_xi.begin(),_xi.end()) < size_of(s));
        assert_release(size_of(s) >= _xi.size() && "cannot compute a projection of a set into a superset");
        assert_release(default_eps > 0.);
      }
      
      BoxPair separate(const IntervalVector& x) const;
      BoxPair separate(const IntervalVector& x, double eps) const;

    protected:

      const Collection<SepBase> _sep;
  };
}