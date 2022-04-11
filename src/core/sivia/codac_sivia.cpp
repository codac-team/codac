/** 
 *  SIVIA
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>

#include "codac_sivia.h"
#include "codac_VIBesFig.h"
#include <ibex_LargestFirst.h>
#include "vibes.h"

using namespace std;

namespace codac
{
  vector<IntervalVector> box_diff(const IntervalVector& x0, const IntervalVector& x)
  {
    vector<IntervalVector> v;
    IntervalVector* boxes;
    int n = x0.diff(x, boxes);
    v.assign(boxes, boxes + n);
    return v;
  }

  void SIVIA(const IntervalVector& x0, Ctc* ctc, float precision, const SetColorMap& color_map)
  {
    assert(x0.size() >= 2);

    ibex::LargestFirst bisector(0.);
    deque<IntervalVector> stack = { x0 };
    int k = 0;

    while(!stack.empty())
    {
      k++;
      IntervalVector x = stack.front();
      stack.pop_front();
      IntervalVector x_before_ctc(x);

      ctc->contract(x);

      vector<IntervalVector> x_out_l = box_diff(x_before_ctc.subvector(0,1), x.subvector(0,1));
      for(const auto& o : x_out_l)
        vibes::drawBox(o, color_map.at(SetValue::OUT));

      if(x.is_empty())
        continue;

      else
      {
        if(x.max_diam() < precision)
          vibes::drawBox(x.subvector(0,1), color_map.at(SetValue::UNKNOWN));

        else
        {
          pair<IntervalVector,IntervalVector> p = bisector.bisect(x.subvector(0,1));
          stack.push_back(cart_prod(p.first,x[2]));
          stack.push_back(cart_prod(p.second,x[2]));
        }
      }
    }

    cout << "Number of contractions: " << k << endl;
    //cout << "Number of boxes:        " << 0 << endl;
  }

  void SIVIA(const IntervalVector& x0, ibex::Sep* sep, float precision, const SetColorMap& color_map)
  {
    assert(x0.size() >= 2);

    ibex::LargestFirst bisector(0.);
    deque<IntervalVector> stack = { x0 };
    int k = 0;

    while(!stack.empty())
    {
      k++;
      IntervalVector x_before_ctc = stack.front();
      stack.pop_front();
      IntervalVector x_in(x_before_ctc), x_out(x_before_ctc);

      sep->separate(x_in, x_out);

      IntervalVector x = x_in & x_out;

      vector<IntervalVector> x_in_l = box_diff(x_before_ctc, x_in);
      for(const auto& i : x_in_l)
        vibes::drawBox(i, color_map.at(SetValue::IN));

      vector<IntervalVector> x_out_l = box_diff(x_before_ctc, x_out);
      for(const auto& o : x_out_l)
        vibes::drawBox(o, color_map.at(SetValue::OUT));

      if(x.is_empty())
        continue;

      else
      {

        if(x.max_diam() < precision)
          vibes::drawBox(x.subvector(0,1), color_map.at(SetValue::UNKNOWN));

        else
        {
          pair<IntervalVector,IntervalVector> p = bisector.bisect(x);
          stack.push_back(p.first);
          stack.push_back(p.second);
        }
      }
    }

    cout << "Number of contractions: " << k << endl;
    //cout << "Number of boxes:        " << 0 << endl;
  }
}