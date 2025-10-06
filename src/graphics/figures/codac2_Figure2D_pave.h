/** 
 *  \file codac2_Figure2D_pave.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_template_tools.h"

namespace codac2
{
  inline std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)> cartesian_drawing()
  {
    return [](Figure2D& fig, const IntervalVector& x, const StyleProperties& s) {
      fig.draw_box(x,s);
    };
  }

  inline std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)> polar_drawing()
  {
    return [](Figure2D& fig, const IntervalVector& x, const StyleProperties& s) {
      fig.draw_pie({0,0},x[0],x[1],s);
    };
  }
  
  template<typename C>
    requires IsCtcBaseOrPtr<C,IntervalVector>
  inline void Figure2D::pave(const IntervalVector& x0, const C& c, double eps,
    const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
    const PavingStyle& style)
  {
    assert_release(eps > 0.);
    assert_release(c.size() >= 2 && "cannot reveal 1d contractors");

    if(x0.size() > 2)
      draw_box(*this, x0, style.outside);

    clock_t t_start = clock();
    std::list<IntervalVector> l { x0 };
    Index n = 0;

    while(!l.empty())
    {
      IntervalVector x = l.front(), prev_x = x;
      l.pop_front();

      ctc(c).contract(x);

      if(x0.size() == 2)
        for(const auto& bi : prev_x.diff(x))
          draw_box(*this, bi, style.outside);

      if(!x.is_empty())
      {
        if(x.max_diam() < eps)
        {
          n++;
          draw_box(*this, x, style.boundary);
        }

        else
        {
          auto p = x.bisect_largest();
          l.push_back(p.first); l.push_back(p.second);
        }
      }
    }

    printf("Computation time: %.4fs, %ld boxes\n", (double)(clock()-t_start)/CLOCKS_PER_SEC, n);
  }
  
  template<typename C>
    requires IsCtcBaseOrPtr<C,IntervalVector>
  inline void Figure2D::pave(const IntervalVector& x0, const C& c, double eps, const PavingStyle& style)
  {
    return pave(x0, c, eps, cartesian_drawing(), style);
  }

  template<typename S>
    requires IsSepBaseOrPtr<S>
  inline void Figure2D::pave(const IntervalVector& x0, const S& s, double eps,
    const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
    const PavingStyle& style)
  {
    assert_release(eps > 0.);
    assert_release(size_of(s) >= 2 && "cannot reveal 1d separators");

    clock_t t_start = clock();
    std::list<IntervalVector> l { x0 };

    while(!l.empty())
    {
      IntervalVector x = l.front();
      l.pop_front();

      auto x_sep = sep(s).separate(x);
      auto boundary = x_sep.inner & x_sep.outer;

      for(const auto& bi : x.diff(x_sep.inner))
        draw_box(*this, bi, style.inside);

      for(const auto& bi : x.diff(x_sep.outer))
        draw_box(*this, bi, style.outside);

      if(!boundary.is_empty())
      {
        if(boundary.max_diam() <= eps)
          draw_box(*this, boundary, style.boundary);

        else
        {
          auto p = boundary.bisect_largest();
          l.push_back(p.first); l.push_back(p.second);
        }
      }
    }
    
    printf("Computation time: %.4fs\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
  }

  template<typename S>
    requires IsSepBaseOrPtr<S>
  inline void Figure2D::pave(const IntervalVector& x0, const S& s, double eps,
    const PavingStyle& style)
  {
    return pave(x0, s, eps, cartesian_drawing(), style);
  }
}