/** 
 *  codac2_CtcProj.cp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcProj.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  void CtcProj::contract(IntervalVector& x) const
  {
    contract(x, _default_eps);
  }

  void CtcProj::contract(IntervalVector& x, double eps) const
  {
    assert_release(eps > 0.);
    assert_release(x.size() == this->size());

    if(_y.size() == 0) // the set is projected onto itself
    {
      _ctc.front()->contract(x);
      return;
    }
    
    list<IntervalVector> l_stack { cart_prod_xy(x,_y) };
    x.set_empty();

    // The stack allows to explore along the y-column to be projected,
    // performing bisections along y if necesary
    while(!l_stack.empty())
    {
      auto w = l_stack.front(); // one box-guess in the projected column
      l_stack.pop_front();

      IntervalVector w_ctc(w);
      _ctc.front()->contract(w_ctc);

      // If the guess box may contain some values
      if(!w_ctc.is_empty())
      {
        // If the current guess w is not a leaf, proceed to a bisection of the guess
        auto y = extract_y(w);
        if(y.max_diam() > eps)
        {
          auto b = w.bisect(y_max_diam_index(y));
          l_stack.push_back(b.first);
          l_stack.push_back(b.second);
        }

        else
          x |= extract_x(w_ctc);
      }
    }
  }
}