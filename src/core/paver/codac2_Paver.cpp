/** 
 *  Paver classes
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_Paver.h"
#include "codac2_Vector.h"

using namespace std;
using namespace codac2;

Paver::Paver(const IntervalVector& x0)
  : _x0(x0)
{ }

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

void Paver::pave(const Ctc_<IntervalVector>& c, double eps)
{
  init_figure();
  clock_t t_start = clock();

  if(_figure && _x0.size() > 2)
    _figure->draw_box(_x0, WithGraphicOutput::outer_style);

  list<IntervalVector> l { _x0 };
  size_t n = 0;

  while(!l.empty())
  {
    IntervalVector x = l.front(), prev_x = x;
    l.pop_front();

    c.contract(x);

    if(_figure && _x0.size() == 2)
      for(const auto& bi : prev_x.diff(x))
        _figure->draw_box(bi, WithGraphicOutput::outer_style);

    if(!x.is_empty())
    {
      if(x.max_diam() < eps)
      {
        n++;
        if(_figure)
    			_figure->draw_box(x, WithGraphicOutput::boundary_style);
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

void Paver::pave(const Sep& s, double eps)
{
  init_figure();
  clock_t t_start = clock();

  list<IntervalVector> l { _x0 };
  size_t n_inner = 0, n_boundary = 0;

  while(!l.empty())
  {
    IntervalVector x = l.front();
    l.pop_front();

    auto x_sep = s.separate(x);
    auto boundary = x_sep.in & x_sep.out;

    if(_figure)
    {
      for(const auto& bi : x.diff(x_sep.in))
      {
        n_inner++;
        _figure->draw_box(bi, WithGraphicOutput::inner_style);
      }

      for(const auto& bi : x.diff(x_sep.out))
        _figure->draw_box(bi, WithGraphicOutput::outer_style);
    }

    if(!boundary.is_empty())
    {
      if(boundary.max_diam() < eps)
      {
        n_boundary++;
        if(_figure)
          _figure->draw_box(boundary, WithGraphicOutput::boundary_style);
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