/** 
 *  \file codac2_CtcProj.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_Ctc.h"
#include "codac2_Collection.h"
#include "codac2_IntervalVector.h"
#include "codac2_template_tools.h"
#include "codac2_ProjBase.h"

namespace codac2
{  
  class CtcProj : public Ctc<CtcProj,IntervalVector>, protected ProjBase
  {
    public:
      
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcProj(const C& c, const std::vector<Index>& proj_indices, double default_eps = 0.01)
        : CtcProj(c, proj_indices, IntervalVector(size_of(c)-proj_indices.size()), default_eps)
      { }

      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      CtcProj(const C& c, const std::vector<Index>& proj_indices, const IntervalVector& y, double default_eps = 0.01)
        : Ctc<CtcProj,IntervalVector>(proj_indices.size()), ProjBase(proj_indices,y,default_eps), _ctc(c)
      {
        assert_release(_y.size() == size_of(c)-(Index)_xi.size());
        assert_release(*min_element(_xi.begin(),_xi.end()) >= 0);
        assert_release(*max_element(_xi.begin(),_xi.end()) < size_of(c));
        assert_release(size_of(c) >= (Index)_xi.size() && "cannot compute a projection of a set into a superset");
        assert_release(default_eps > 0.);
      }
      
      void contract(IntervalVector& x) const;
      void contract(IntervalVector& x, double eps) const;

    protected:

      const Collection<CtcBase<IntervalVector>> _ctc;
  };
}