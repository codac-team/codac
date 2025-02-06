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
}