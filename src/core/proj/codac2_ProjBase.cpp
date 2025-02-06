/** 
 *  codac2_ProjBase.cp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_ProjBase.h"

using namespace std;

namespace codac2
{
  ProjBase::ProjBase(const std::vector<Index>& proj_indices, const IntervalVector& y, double default_eps)
    : _n(proj_indices.size()+y.size()), _xi(proj_indices), _y(y), _default_eps(default_eps)
  {
    assert(default_eps > 0.);
  }

  IntervalVector ProjBase::extract_x(const IntervalVector& w) const
  {
    assert(w.size() == _n);

    IntervalVector x(_xi.size());
    Index k = 0;
    for(const auto& xi : _xi)
      x[k++] = w[xi];
    assert(k == (Index)_xi.size()); // all components have been reached
    return x;
  }

  IntervalVector ProjBase::extract_y(const IntervalVector& w) const
  {
    assert(w.size() == _n);
    
    IntervalVector y(_y.size());
    Index k = 0;
    for(Index j = 0 ; j < w.size() ; j++)
    {
      bool outside_proj = true;
      for(const auto& xi : _xi)
        if(xi == j)
        {
          outside_proj = false;
          break;
        }

      if(outside_proj)
        y[k++] = w[j];
    }

    assert(k == y.size()); // all components have been reached
    return y;
  }

  IntervalVector ProjBase::cart_prod_xy(const IntervalVector& x, const IntervalVector& y) const
  {
    assert(x.size() == (Index)_xi.size());
    assert(y.size() == _y.size());

    Index ix = 0, iy = 0;
    IntervalVector w(_n);

    for(Index j = 0 ; j < _n ; j++)
    {
      bool outside_proj = true;
      for(const auto& xi : _xi)
        if(xi == j)
        {
          outside_proj = false;
          break;
        }

      w[j] = outside_proj ? y[iy++] : x[ix++];
    }

    assert(ix+iy == w.size()); // all components have been reached
    return w;
  }

  Index ProjBase::y_max_diam_index(const IntervalVector& y) const
  {
    Index k = 0, y_max = y.max_diam_index();
    for(Index i = 0 ; i < _n ; i++)
    {
      bool outside_proj = true;
      for(const auto& xi : _xi)
        if(xi == i)
        {
          outside_proj = false;
          break;
        }

      if(outside_proj)
      {
        if(k == y_max)
          return i;
        k++;
      }
    }

    assert(false && "unable to find y-index");
    return 0;
  }
}