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

namespace codac2
{
  class SepProj : public Sep<SepProj>
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep_,S> || std::is_same_v<std::shared_ptr<Sep_>,S>
        >::type>
      SepProj(const S& s, const std::vector<size_t>& proj_indices, double default_eps = 0.01)
        : SepProj(s, proj_indices, IntervalVector(size_of(s)-proj_indices.size()), default_eps)
      { }

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep_,S> || std::is_same_v<std::shared_ptr<Sep_>,S>
        >::type>
      SepProj(const S& s, const std::vector<size_t>& proj_indices, const IntervalVector& y, double default_eps = 0.01)
        : Sep<SepProj>(proj_indices.size()), _sep(s), _xi(proj_indices), _y(y), _default_eps(default_eps)
      {
        assert_release(_y.size() == size_of(s)-_xi.size());
        assert_release(*min_element(_xi.begin(),_xi.end()) >= 0);
        assert_release(*max_element(_xi.begin(),_xi.end()) < size_of(s));
        assert_release(size_of(s) >= _xi.size() && "cannot compute a projection of a set into a superset");
      }
      
      BoxPair separate(const IntervalVector& x) const;
      BoxPair separate(const IntervalVector& x, double eps) const;

    protected:

      IntervalVector extract_x(const IntervalVector& w) const;
      IntervalVector extract_y(const IntervalVector& w) const;
      IntervalVector cart_prod_xy(const IntervalVector& x, const IntervalVector& y) const;
      size_t y_max_diam_index(const IntervalVector& w) const;

      const Collection<Sep_> _sep;
      const std::vector<size_t> _xi;
      const IntervalVector _y;
      const double _default_eps;
  };
}