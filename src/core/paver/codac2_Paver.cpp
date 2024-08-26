/** 
 *  codac2_Paver.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Paver.h"
#include "codac2_Vector.h"

using namespace std;
using namespace codac2;

Paver::Paver(const IntervalVector& x0)
  : _x0(x0)
{ }

void Paver::set_figure(Figure2D *g)
{
  _figure = g;
}

void Paver::init_figure()
{
  if(_figure && _figure->is_default())
  {
    // If the figure is automatically built by default
    _figure->set_axes(axis(0,_x0[0],"x_1"), axis(1,_x0[1],"x_2"));

    Vector w = _figure->window_size();
    if(_x0[0].diam() > _x0[1].diam())
      w[1] *= _x0[1].diam()/_x0[0].diam();
    else
      w[0] *= _x0[0].diam()/_x0[1].diam();

    _figure->set_window_properties(_figure->pos(), w);
  }
}

std::list<IntervalVector> Paver::pave(const Ctc_<IntervalVector>& c, double eps)
{
  assert_release(eps > 0.);
  assert_release(c.size() >= 2 && "cannot reveal 1d contractors");

  init_figure();
  clock_t t_start = clock();

  std::list<IntervalVector> l_output;

  if(_figure && _x0.size() > 2)
    _figure->draw_box(_x0, outer_style);

  list<IntervalVector> l { _x0 };
  size_t n = 0;

  while(!l.empty())
  {
    IntervalVector x = l.front(), prev_x = x;
    l.pop_front();

    c.contract(x);

    if(_figure && _x0.size() == 2)
      for(const auto& bi : prev_x.diff(x))
        _figure->draw_box(bi, outer_style);

    if(!x.is_empty())
    {
      if(x.max_diam() < eps)
      {
        n++;
        l_output.push_back(x);
        if(_figure)
    			_figure->draw_box(x, boundary_style);
      }

      else
      {
        auto p = x.bisect_largest();
        l.push_back(p.first); l.push_back(p.second);
      }
    }
  }

  printf("Computation time: %.4fs, %zd boxes\n", (double)(clock()-t_start)/CLOCKS_PER_SEC, n);
  return l_output;
}

std::list<IntervalVector> Paver::pave(const std::shared_ptr<Ctc_<IntervalVector>>& c, double eps)
{
  return pave(*c,eps);
}

void Paver::pave(const Sep_& s, double eps)
{
  assert_release(eps > 0.);
  assert_release(s.size() >= 2 && "cannot reveal 1d separators");
  
  init_figure();
  clock_t t_start = clock();

  list<IntervalVector> l { _x0 };
  size_t n_inner = 0, n_boundary = 0;

  while(!l.empty())
  {
    IntervalVector x = l.front();
    l.pop_front();

    auto x_sep = s.separate(x);
    auto boundary = x_sep.inner & x_sep.outer;

    if(_figure)
    {
      for(const auto& bi : x.diff(x_sep.inner))
      {
        n_inner++;
        _figure->draw_box(bi, inner_style);
      }

      for(const auto& bi : x.diff(x_sep.outer))
        _figure->draw_box(bi, outer_style);
    }

    if(!boundary.is_empty())
    {
      if(boundary.max_diam() < eps)
      {
        n_boundary++;
        if(_figure)
          _figure->draw_box(boundary, boundary_style);
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

void Paver::pave(const std::shared_ptr<Sep_>& s, double eps)
{
  return pave(*s,eps);
}