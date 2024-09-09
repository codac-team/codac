/** 
 *  codac2_pave.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_pave.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps)
  {
    assert_release(eps > 0.);
    assert_release(!x.is_empty());
    assert_release(c.size() >= 2 && "cannot reveal 1d contractors");
    
    PavingOut p(x);
    // In order to be able to reconstruct the initial box, the first level represents the
    // initial domain x (the left node is x, the right one is an empty box).
    p.tree()->bisect();
    p.tree()->left()->boxes() = { x };
    get<0>(p.tree()->right()->boxes()).set_empty();

    std::shared_ptr<PavingOut_Node> n;
    list<std::shared_ptr<PavingOut_Node>> l { p.tree()->left() };

    while(!l.empty())
    {
      n = l.front();
      l.pop_front();

      c.contract(get<0>(n->boxes()));

      if(!get<0>(n->boxes()).is_empty() && get<0>(n->boxes()).max_diam() > eps)
      {
        n->bisect();
        l.push_back(n->left());
        l.push_back(n->right());
      }
    }

    return p;
  }
  
  PavingInOut pave(const IntervalVector& x, const SepBase& s, double eps)
  {
    assert_release(eps > 0.);
    assert_release(!x.is_empty());
    assert_release(s.size() >= 2 && "cannot reveal 1d separators");
    
    PavingInOut p(x);
    std::shared_ptr<PavingInOut_Node> n;
    list<std::shared_ptr<PavingInOut_Node>> l { p.tree() };

    while(!l.empty())
    {
      n = l.front();
      l.pop_front();

      auto xs = s.separate(get<1>(n->boxes()));
      auto boundary = (xs.inner & xs.outer);
      n->boxes() = { xs.inner, xs.outer };

      if(!boundary.is_empty() && boundary.max_diam() > eps)
      {
        n->bisect();
        l.push_back(n->left());
        l.push_back(n->right());
      }
    }

    return p;
  }

  void init_fig(Figure2D& fig, const IntervalVector& x0)
  {
    fig.set_axes(axis(0,x0[0],"x_1"), axis(1,x0[1],"x_2"));

    Vector w = fig.window_size();
    if(x0[0].diam() > x0[1].diam())
      w[1] *= x0[1].diam()/x0[0].diam();
    else
      w[0] *= x0[0].diam()/x0[1].diam();

    fig.set_window_properties(fig.pos(), w);
  }

  void draw_while_paving(const IntervalVector& x0, shared_ptr<const CtcBase<IntervalVector>> c, double eps, std::shared_ptr<Figure2D> fig)
  {
    draw_while_paving(x0, *c, eps, fig);
  }

  void draw_while_paving(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, std::shared_ptr<Figure2D> fig)
  {
    assert_release(eps > 0.);
    assert_release(c.size() >= 2 && "cannot reveal 1d contractors");

    if(!fig)
      fig = DefaultView::selected_fig();
    init_fig(*fig, x0);

    clock_t t_start = clock();

    if(x0.size() > 2)
      fig->draw_box(x0, StyleProperties::outside());

    list<IntervalVector> l { x0 };
    size_t n = 0;

    while(!l.empty())
    {
      IntervalVector x = l.front(), prev_x = x;
      l.pop_front();

      c.contract(x);

      if(x0.size() == 2)
        for(const auto& bi : prev_x.diff(x))
          fig->draw_box(bi, StyleProperties::outside());

      if(!x.is_empty())
      {
        if(x.max_diam() < eps)
        {
          n++;
          fig->draw_box(x, StyleProperties::boundary());
        }

        else
        {
          auto p = x.bisect_largest();
          l.push_back(p.first); l.push_back(p.second);
        }
      }
    }

    printf("Computation time: %.4fs, %zd boxes\n", (double)(clock()-t_start)/CLOCKS_PER_SEC, n);
  }

  void draw_while_paving(const IntervalVector& x0, std::shared_ptr<const SepBase> s, double eps, std::shared_ptr<Figure2D> fig)
  {
    draw_while_paving(x0, *s, eps, fig);
  }

  void draw_while_paving(const IntervalVector& x0, const SepBase& s, double eps, std::shared_ptr<Figure2D> fig)
  {
    assert_release(eps > 0.);
    assert_release(s.size() >= 2 && "cannot reveal 1d separators");

    if(!fig)
      fig = DefaultView::selected_fig();
    init_fig(*fig, x0);
    
    clock_t t_start = clock();

    list<IntervalVector> l { x0 };
    size_t n_inner = 0, n_boundary = 0;

    while(!l.empty())
    {
      IntervalVector x = l.front();
      l.pop_front();

      auto x_sep = s.separate(x);
      auto boundary = x_sep.inner & x_sep.outer;

      for(const auto& bi : x.diff(x_sep.inner))
      {
        n_inner++;
        fig->draw_box(bi, StyleProperties::inside());
      }

      for(const auto& bi : x.diff(x_sep.outer))
        fig->draw_box(bi, StyleProperties::outside());

      if(!boundary.is_empty())
      {
        if(boundary.max_diam() < eps)
        {
          n_boundary++;
          fig->draw_box(boundary, StyleProperties::boundary());
        }

        else
        {
          auto p = boundary.bisect_largest();
          l.push_back(p.first); l.push_back(p.second);
        }
      }
    }
    
    printf("Computation time: %.4fs, %zd inner boxes, %zd boundary boxes\n",
      (double)(clock()-t_start)/CLOCKS_PER_SEC, n_inner, n_boundary);
  }
}