/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "codac2_SepProj.h"

using namespace std;

namespace codac2
{
  std::shared_ptr<Sep> SepProj::copy() const
  {
    return std::make_shared<SepProj>(*this);
  }

  BoxPair SepProj::separate(const IntervalVector& x) const
  {
    // Default value for eps, for removing virtuality
    return separate(x, 0.01);
  }

  BoxPair SepProj::separate(const IntervalVector& x, double eps) const
  {
    list<IntervalVector> l_stack { cart_prod(x,_y) };
    list<IntervalVector> l_in, l_out;

    // The stack allows to explore along the y-column to be projected,
    // performing bisections along y if necesary
    while(!l_stack.empty())
    {
      auto w = l_stack.front(); // one box-guess in the projected column
      l_stack.pop_front();

      auto w_sep = _sep.front().separate(w);
      assert((w_sep.in | w_sep.out) == w);

      // If the guess box may contain some values
      if(!w_sep.out.is_empty())
      {
        // Trying to find inner values...
        // A new guess is the y-middle of the previous one
        auto w_mid = cart_prod(x,extract_y(w).mid());
        auto w_sep_mid = _sep.front().separate(w_mid);
        assert((w_sep_mid.in | w_sep_mid.out) == w_mid);

        // If inner values entirely cover the input projection box x,
        // the algorithm can terminate
        if(extract_x(w_sep_mid.in).is_empty())
          return { IntervalVector::empty(x.size()), x };

        // Otherwise, the inner parts are stored temporarily
        l_in.push_back(extract_x(w_sep_mid.in));

        // If the current guess w is not a leaf, proceed to a bisection of the guess
        if(extract_y(w).max_diam() > eps)
        {
          auto b = w.bisect(x.size() + extract_y(w).max_diam_index());
          l_stack.push_back(b.first);
          l_stack.push_back(b.second);
        }

        else // storing outer values for future reconstruction
          l_out.push_back(extract_x(w_sep.out));
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
    return w.subvector(0,w.size()-_y.size()-1);
  }

  IntervalVector SepProj::extract_y(const IntervalVector& w) const
  {
    return w.subvector(w.size()-_y.size(),w.size()-1);
  }

}