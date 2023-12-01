/** 
 *  SIVIA
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou, Julien Damers
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include <iostream>
#include <ctime>

#include "codac_sivia.h"
#include "codac_VIBesFig.h"
#include <ibex_LargestFirst.h>
#include "vibes.h"

using namespace std;

namespace codac
{
  static bool _vibes_initialized = false;

  vector<IntervalVector> box_diff(const IntervalVector& x0, const IntervalVector& x)
  {
    vector<IntervalVector> v;
    IntervalVector* boxes;
    int n = x0.diff(x, boxes);
    v.assign(boxes, boxes + n);
    return v;
  }

  map<SetValue,list<IntervalVector>> _SIVIA(
    const IntervalVector& x0, const IntervalVector* y0, Ctc& ctc, float precision, bool regular_paving, 
    bool display_result, const string& fig_name, bool return_result, const SetColorMap& color_map)
  {
    assert(x0.size() >= 2);

    if(display_result)
    {
      // Some values in the desired color map may not have been defined by the user
      // We select default colors in this case

        SetColorMap cm = DEFAULT_SET_COLOR_MAP;

        if(display_result)
        {
          if(color_map.find(SetValue::OUT) != color_map.end())
            cm[SetValue::OUT] = color_map.at(SetValue::OUT);

          if(color_map.find(SetValue::UNKNOWN) != color_map.end())
            cm[SetValue::UNKNOWN] = color_map.at(SetValue::UNKNOWN);
        }

      if(!_vibes_initialized)
      {
        _vibes_initialized = true;
        vibes::beginDrawing();
        // will not be ended in case the init has been done outside this SIVIA function
      }

      if(!fig_name.empty())
        vibes::newFigure(fig_name);

      vibes::drawBox(x0.subvector(0,1));
      vibes::newGroup("boxes_out", cm.at(SetValue::OUT));
      vibes::newGroup("boxes_unknown", cm.at(SetValue::UNKNOWN));
      
      vibes::drawBox(x0.subvector(0,1), vibesParams("figure", fig_name));
      vibes::axisAuto();
    }

    map<SetValue,int> n_boxes;
    map<SetValue,list<IntervalVector>> boxes{
      // SetValue::IN is not possible for SIVIA using Ctc
      {SetValue::OUT, {}},
      {SetValue::UNKNOWN, {}},
    };

    ibex::LargestFirst bisector(0.);
    deque<IntervalVector> stack = { y0 ? cart_prod(x0,*y0) : x0 };
    int k = 0;

    clock_t t_start = clock();
    
    while(!stack.empty())
    {
      k++;
      IntervalVector x_before_ctc = stack.front();
      stack.pop_front();
      IntervalVector x(x_before_ctc);

      ctc.contract(x);

      // Out values

        vector<IntervalVector> x_out_l;

        if(regular_paving)
        {
          if(x.is_empty())
            x_out_l.push_back(x_before_ctc);
        }

        else
          x_out_l = box_diff(x_before_ctc, x);

        for(const auto& o : x_out_l)
        {
          if(display_result)
          {
            vibes::drawBox(o.subvector(0,1), vibesParams("group", "boxes_out"));
            n_boxes[SetValue::OUT] ++;
          }

          if(return_result)
            boxes[SetValue::OUT].push_front(o);
        }

      // Remaining solutions

        if(x.is_empty())
          continue;

        else
        {
          IntervalVector& x_remaining = regular_paving ? x_before_ctc : x;

          if((!y0 && x_remaining.max_diam() < precision) ||
             ( y0 && x_remaining.subvector(0,x0.size()-1).max_diam() < precision))
          {
            if(display_result)
            {
              vibes::drawBox(x_remaining.subvector(0,1), vibesParams("group", "boxes_unknown"));
              n_boxes[SetValue::UNKNOWN] ++;
            }

            if(return_result)
              boxes[SetValue::UNKNOWN].push_front(x_remaining);
          }

          else
          {
            if(y0)
            {
              pair<IntervalVector,IntervalVector> p = bisector.bisect(x_remaining.subvector(0,x0.size()-1));
              stack.push_back(cart_prod(p.first,*y0));
              stack.push_back(cart_prod(p.second,*y0));
            }

            else
            {
              pair<IntervalVector,IntervalVector> p = bisector.bisect(x_remaining);
              stack.push_back(p.first);
              stack.push_back(p.second);
            }
          }
        }
    }

    if(display_result)
    {
      printf( "Computation time: %.4fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
      cout << "  Contractions:   " << k << endl;
      cout << "  OUT boxes:      " << n_boxes[SetValue::OUT] << endl;
      cout << "  UNKNOWN boxes:  " << n_boxes[SetValue::UNKNOWN] << endl;
    }

    return boxes;
  }

  map<SetValue,list<IntervalVector>> SIVIA(
    const IntervalVector& x0, const IntervalVector& y0, Ctc& ctc, float precision, bool regular_paving, 
    bool display_result, const string& fig_name, bool return_result, const SetColorMap& color_map)
  {
    return _SIVIA(x0, &y0, ctc, precision, regular_paving, display_result, fig_name, return_result, color_map);
  }

  map<SetValue,list<IntervalVector>> SIVIA(
    const IntervalVector& x0, Ctc& ctc, float precision, bool regular_paving, 
    bool display_result, const string& fig_name, bool return_result, const SetColorMap& color_map)
  {
    return _SIVIA(x0, nullptr, ctc, precision, regular_paving, display_result, fig_name, return_result, color_map);
  }

  map<SetValue,list<IntervalVector>> SIVIA(
    const IntervalVector& x0, ibex::Sep& sep, float precision, bool regular_paving, 
    bool display_result, const string& fig_name, bool return_result, const SetColorMap& color_map)
  {
    assert(x0.size() >= 2);

    if(display_result)
    {
      // Some values in the desired color map may not have been defined by the user
      // We select default colors in this case

        SetColorMap cm = DEFAULT_SET_COLOR_MAP;

        if(display_result)
        {
          if(color_map.find(SetValue::OUT) != color_map.end())
            cm[SetValue::OUT] = color_map.at(SetValue::OUT);

          if(color_map.find(SetValue::UNKNOWN) != color_map.end())
            cm[SetValue::UNKNOWN] = color_map.at(SetValue::UNKNOWN);

          if(color_map.find(SetValue::IN) != color_map.end())
            cm[SetValue::IN] = color_map.at(SetValue::IN);
        }

      if(!_vibes_initialized)
      {
        _vibes_initialized = true;
        vibes::beginDrawing();
        // will not be ended in case the init has been done outside this SIVIA function
      }

      if(!fig_name.empty())
        vibes::newFigure(fig_name);

      vibes::drawBox(x0.subvector(0,1));
      vibes::newGroup("boxes_out", cm.at(SetValue::OUT));
      vibes::newGroup("boxes_unknown", cm.at(SetValue::UNKNOWN));
      vibes::newGroup("boxes_in", cm.at(SetValue::IN));
      vibes::axisAuto();
    }

    map<SetValue,int> n_boxes;
    map<SetValue,list<IntervalVector>> boxes{
      {SetValue::IN, {}},
      {SetValue::OUT, {}},
      {SetValue::UNKNOWN, {}},
    };

    ibex::LargestFirst bisector(0.);
    deque<IntervalVector> stack = { x0 };
    int k = 0;

    clock_t t_start = clock();

    while(!stack.empty())
    {
      k++;
      IntervalVector x_before_ctc = stack.front();
      stack.pop_front();
      IntervalVector x_in(x_before_ctc), x_out(x_before_ctc);

      sep.separate(x_in, x_out);

      IntervalVector x = x_in & x_out;

      // Out and In values

        vector<IntervalVector> x_in_l, x_out_l;

        if(regular_paving)
        {
          if(x_in.is_empty())
            x_in_l.push_back(x_before_ctc);
          if(x_out.is_empty())
            x_out_l.push_back(x_before_ctc);
        }

        else
        {
          x_in_l = box_diff(x_before_ctc, x_in);
          x_out_l = box_diff(x_before_ctc, x_out);
        }

        for(const auto& i : x_in_l)
        {
          if(display_result)
          {
            vibes::drawBox(i.subvector(0,1), vibesParams("group", "boxes_in"));
            n_boxes[SetValue::IN] ++;
          }

          if(return_result)
            boxes[SetValue::IN].push_front(i);
        }

        for(const auto& o : x_out_l)
        {
          if(display_result)
          {
            vibes::drawBox(o.subvector(0,1), vibesParams("group", "boxes_out"));
            n_boxes[SetValue::OUT] ++;
          }

          if(return_result)
            boxes[SetValue::OUT].push_front(o);
        }

      // Remaining values

        if(x.is_empty())
          continue;

        else
        {
          IntervalVector& x_remaining = regular_paving ? x_before_ctc : x;

          if(x_remaining.max_diam() < precision)
          {
            if(display_result)
            {
              vibes::drawBox(x_remaining.subvector(0,1), vibesParams("group", "boxes_unknown"));
              n_boxes[SetValue::UNKNOWN] ++;
            }

            if(return_result)
              boxes[SetValue::UNKNOWN].push_front(x_remaining);
          }

          else
          {
            pair<IntervalVector,IntervalVector> p = bisector.bisect(x_remaining);
            stack.push_back(p.first);
            stack.push_back(p.second);
          }
        }
    }

    if(display_result)
    {
      printf( "Computation time: %.4fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
      cout << "  Contractions:   " << k << endl;
      cout << "  IN boxes:       " << n_boxes[SetValue::IN] << endl;
      cout << "  OUT boxes:      " << n_boxes[SetValue::OUT] << endl;
      cout << "  UNKNOWN boxes:  " << n_boxes[SetValue::UNKNOWN] << endl;
    }

    return boxes;
  }
}