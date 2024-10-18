/** 
 *  codac2_SepProj.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <list>
#include "codac2_SepProj.h"

using namespace std;

namespace codac2
{
  BoxPair SepProj::separate(const IntervalVector& x) const
  {
    // Default value for eps, for removing virtuality
    return separate(x, _default_eps);
  }

  BoxPair SepProj::separate(const IntervalVector& x, double eps) const
  {
    assert_release(eps > 0.);
    assert_release(x.size() == this->size());
    
    list<IntervalVector> l_stack { cart_prod_xy(x,_y) };
    list<IntervalVector> l_in, l_out;

    // The stack allows to explore along the y-column to be projected,
    // performing bisections along y if necesary
    while(!l_stack.empty())
    {
      auto w = l_stack.front(); // one box-guess in the projected column
      l_stack.pop_front();

      auto w_sep = _sep.front().separate(w);
      assert((w_sep.inner | w_sep.outer) == w);

      // If the guess box may contain some values
      if(!w_sep.outer.is_empty())
      {
        // Trying to find inner values...
        // A new guess is the y-middle of the previous one
        auto w_mid = cart_prod_xy(x,extract_y(w).mid());
        auto w_sep_mid = _sep.front().separate(w_mid);
        assert((w_sep_mid.inner | w_sep_mid.outer) == w_mid);

        // If inner values entirely cover the input projection box x,
        // the algorithm can terminate
        if(extract_x(w_sep_mid.inner).is_empty())
          return { IntervalVector::empty(x.size()), x };

        // Otherwise, the inner parts are stored temporarily
        l_in.push_back(extract_x(w_sep_mid.inner));

        // If the current guess w is not a leaf, proceed to a bisection of the guess
        auto y = extract_y(w);
        if(y.max_diam() > eps)
        {
          auto b = w.bisect(y_max_diam_index(y));
          l_stack.push_back(b.first);
          l_stack.push_back(b.second);
        }

        else // storing outer values for future reconstruction
          l_out.push_back(extract_x(w_sep.outer));
      }
    }

    // Reconstructing x_out from previous parts

      auto x_out = IntervalVector::empty(x.size());
      for(const auto& li_out : l_out)
        x_out |= li_out;

    // Reconstructing x_in from previous parts

      auto x_in = x;
      for(const auto& li_in : l_in)
        x_in &= li_in;

    assert((x_in | x_out) == x);
    return { x_in, x_out };
  }

  IntervalVector SepProj::extract_x(const IntervalVector& w) const
  {
    assert(w.size() == this->_sep.front().size());

    IntervalVector x(_xi.size());
    size_t k = 0;
    for(const auto& xi : _xi)
      x[k++] = w[xi];
    assert(k == _xi.size()); // all components have been reached
    return x;
  }

  IntervalVector SepProj::extract_y(const IntervalVector& w) const
  {
    assert(w.size() == this->_sep.front().size());
    
    IntervalVector y(_y.size());
    size_t k = 0;
    for(size_t j = 0 ; j < w.size() ; j++)
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

  IntervalVector SepProj::cart_prod_xy(const IntervalVector& x, const IntervalVector& y) const
  {
    size_t n = this->_sep.front().size();
    assert(x.size() == this->size());
    assert(y.size() == _y.size());
    assert(x.size()+y.size() == n);

    size_t ix = 0, iy = 0;
    IntervalVector w(n);

    for(size_t j = 0 ; j < n ; j++)
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

  size_t SepProj::y_max_diam_index(const IntervalVector& y) const
  {
    size_t k = 0, y_max = y.max_diam_index();
    for(size_t i = 0 ; i < this->_sep.front().size() ; i++)
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