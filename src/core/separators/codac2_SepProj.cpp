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

    if(_y.size() == 0) // the set is projected onto itself
      return _sep.front()->separate(x);
    
    list<IntervalVector> l_stack { cart_prod_xy(x,_y) };
    
    auto result_out = IntervalVector::empty(x.size());
    auto result_in = x;

    // The stack allows to explore along the y-column to be projected,
    // performing bisections along y if necesary
    while(!l_stack.empty())
    {
      auto w = l_stack.front(); // one box-guess in the projected column
      l_stack.pop_front();

      auto w_sep = _sep.front()->separate(w);
      assert((w_sep.inner | w_sep.outer) == w);

      if(extract_x(w) == x)
        result_in &= extract_x(w_sep.inner);

      // If the guess box may contain some values
      if(!w_sep.outer.is_empty()) // i.e. extract_x(w_sep.outer).is_empty()
      {
        // If the current guess w is not a leaf, proceed to a bisection of the guess
        Index yi_max = y_max_diam_index(extract_y(w_sep.outer));
        if(w_sep.outer[yi_max].diam() > eps)
        {
          auto b = w_sep.outer.bisect(yi_max);
          l_stack.push_back(b.first);
          l_stack.push_back(b.second);
        }

        else // only leaves are considered for result_out
          result_out |= extract_x(w_sep.outer);

        if(!result_in.is_empty())
        {
          // Trying to find inner values...
          // A new guess is the y-middle of the previous one
          auto w_mid = cart_prod_xy(x,extract_y(w_sep.outer).mid());
          assert(!w_mid.is_empty());
          auto w_sep_mid = _sep.front()->separate(w_mid);
          assert((w_sep_mid.inner | w_sep_mid.outer) == w_mid);
          result_in &= extract_x(w_sep_mid.inner);
        }
      }
    }

    assert((result_in | result_out) == x);
    return { result_in, result_out };
  }
}