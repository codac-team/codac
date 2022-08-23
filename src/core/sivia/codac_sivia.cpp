/** 
 *  SIVIA
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou (revised by Julien Damers)
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

    bool check_empty(const IntervalVector& x0)
    {
      bool empty = false;
      for (int i=0; i<x0.size(); i++)
      {
          if (x0[i].is_empty())
          {
              empty = true;
              break;
          }
      }
      return empty;
    }

    vector<IntervalVector> box_diff(const IntervalVector &x0, const IntervalVector &x)
    {
        vector<IntervalVector> v;
        IntervalVector *boxes;
        int n = x0.diff(x, boxes);
        v.assign(boxes, boxes + n);
        return v;
    }


    map<SetValue, list<IntervalVector>>
    SIVIA(const IntervalVector &x0, Ctc &ctc, float precision,
          bool regular, bool display_result,
          const string &fig_name, bool return_result,
          const SetColorMap &color_map)
    {
        assert(x0.size() >= 2);

        if ( display_result )
        {
            if ( !_vibes_initialized )
            {
                _vibes_initialized = true;
                vibes::beginDrawing();
                vibes::axisAuto();
                // will not be ended in case the init has been done outside this SIVIA function
            }

            if ( !fig_name.empty())
                vibes::selectFigure(fig_name);

            vibes::drawBox(x0);
        }

        map<SetValue, int> n_boxes;
        map<SetValue, list<IntervalVector>> boxes{
                {SetValue::OUT,     {}},
                {SetValue::UNKNOWN, {}}
        };

        ibex::LargestFirst bisector(0.);
        deque<IntervalVector> stack = {x0};
        int k = 0;

        // Some values in the desired color map may not have been defined by the user
        // We select default colors in this case

        SetColorMap cm = DEFAULT_SET_COLOR_MAP;

        if ( display_result )
        {

            if ( color_map.find(SetValue::OUT) != color_map.end())
                cm[SetValue::OUT] = color_map.at(SetValue::OUT);

            if ( color_map.find(SetValue::UNKNOWN) != color_map.end())
                cm[SetValue::UNKNOWN] = color_map.at(SetValue::UNKNOWN);
        }

        bool empty = false;
        clock_t t_start = clock();

        while ( !stack.empty())
        {
            empty = false;
            k++;
            IntervalVector x = stack.front();
            stack.pop_front();
            IntervalVector x_before_ctc(x);

            ctc.contract(x);
            empty = check_empty(x);


            if ( regular )
            {
                if ( empty )
                {
                    if ( display_result )
                    {
                        vibes::drawBox(x_before_ctc.subvector(0, 1), cm.at(SetValue::OUT));
                        n_boxes[SetValue::OUT]++;
                    }

                    if ( return_result )
                        boxes[SetValue::OUT].push_front(x_before_ctc);
                }
                else
                {
                    if (x_before_ctc.max_diam()>precision)
                    {
                        int i = x_before_ctc.subvector(0,1).extr_diam_index(false);
                        pair<IntervalVector, IntervalVector> p = x_before_ctc.bisect(i);
                        stack.push_back(p.first);
                        stack.push_back(p.second);
                    }
                    else
                    {
                        if ( display_result )
                        {
                            vibes::drawBox(x_before_ctc.subvector(0, 1), cm.at(SetValue::UNKNOWN));
                            n_boxes[SetValue::UNKNOWN]++;
                        }

                        if ( return_result )
                            boxes[SetValue::UNKNOWN].push_front(x_before_ctc);
                    }
                }

            }
            else
            {
                vector<IntervalVector> x_out_l = box_diff(x_before_ctc, x);
                for ( const auto &o: x_out_l )
                {
                    if ( display_result )
                    {
                        vibes::drawBox(o.subvector(0, 1), cm.at(SetValue::OUT));
                        n_boxes[SetValue::OUT]++;
                    }

                    if ( return_result )
                        boxes[SetValue::OUT].push_front(o);
                }

                if ( empty )
                {
                    continue;
                }

                else
                {
                    if ( x.max_diam() < precision )
                    {
                        if ( display_result )
                        {
                            vibes::drawBox(x.subvector(0, 1), cm.at(SetValue::UNKNOWN));
                            n_boxes[SetValue::UNKNOWN]++;
                        }

                        if ( return_result )
                        {
                            boxes[SetValue::UNKNOWN].push_front(x);
                        }
                    }

                    else
                    {
                        pair<IntervalVector, IntervalVector> p = bisector.bisect(x);
                        stack.push_back(p.first);
                        stack.push_back(p.second);
                    }

                }
            }

        }

        if ( display_result )
        {
            printf("Computation time: %.2fs\n", (double) (clock() - t_start) / CLOCKS_PER_SEC);
            cout << "  Contractions:   " << k << endl;
            cout << "  OUT boxes:      " << n_boxes[SetValue::OUT] << endl;
            cout << "  UNKNOWN boxes:  " << n_boxes[SetValue::UNKNOWN] << endl;
        }

        return boxes;
    }


    map<SetValue, list<IntervalVector>> SIVIA(const IntervalVector &x0, ibex::Sep &sep, float precision,
          bool regular, bool display_result, const string &fig_name, bool return_result,
          const SetColorMap &color_map)
    {
        assert(x0.size() >= 2);

        if ( display_result )
        {
            if ( !_vibes_initialized )
            {
                _vibes_initialized = true;
                vibes::beginDrawing();
                vibes::axisAuto();
                // will not be ended in case the init has been done outside this SIVIA function
            }

            if ( !fig_name.empty())
                vibes::selectFigure(fig_name);

            vibes::drawBox(x0);
        }

        map<SetValue, int> n_boxes;
        map<SetValue, list<IntervalVector>> boxes{
                {SetValue::IN,      {}},
                {SetValue::OUT,     {}},
                {SetValue::UNKNOWN, {}},
        };

        ibex::LargestFirst bisector(0.);
        deque<IntervalVector> stack = {x0};
        int k = 0;

        // Some values in the desired color map may not have been defined by the user
        // We select default colors in this case

        SetColorMap cm = DEFAULT_SET_COLOR_MAP;

        if ( display_result )
        {
            if ( color_map.find(SetValue::IN) != color_map.end())
                cm[SetValue::IN] = color_map.at(SetValue::IN);

            if ( color_map.find(SetValue::OUT) != color_map.end())
                cm[SetValue::OUT] = color_map.at(SetValue::OUT);

            if ( color_map.find(SetValue::UNKNOWN) != color_map.end())
                cm[SetValue::UNKNOWN] = color_map.at(SetValue::UNKNOWN);
        }

        bool empty = false;
        clock_t t_start = clock();

        while ( !stack.empty())
        {
            k++;
            empty = false;
            IntervalVector x_before_ctc = stack.front();
            stack.pop_front();
            IntervalVector x_in(x_before_ctc), x_out(x_before_ctc);

            sep.separate(x_in, x_out);

            if (regular)
            {
                if ( check_empty(x_out))
                {
                    if ( display_result )
                    {
                        vibes::drawBox(x_before_ctc, cm.at(SetValue::OUT));
                        n_boxes[SetValue::OUT]++;
                    }

                    if ( return_result )
                    {
                        boxes[SetValue::OUT].push_front(x_before_ctc);
                    }

                }
                else if(check_empty(x_in))
                {
                    if ( display_result )
                    {
                        vibes::drawBox(x_before_ctc, cm.at(SetValue::IN));
                        n_boxes[SetValue::IN]++;
                    }

                    if ( return_result )
                    {
                        boxes[SetValue::IN].push_front(x_before_ctc);
                    }
                }
                else
                {
                    if (x_before_ctc.max_diam()>precision)
                    {
                        int i = x_before_ctc.subvector(0,1).extr_diam_index(false);
                        pair<IntervalVector, IntervalVector> p = x_before_ctc.bisect(i);
                        stack.push_back(p.first);
                        stack.push_back(p.second);
                    }
                    else
                    {
                        if ( display_result )
                        {
                            vibes::drawBox(x_before_ctc.subvector(0, 1), cm.at(SetValue::UNKNOWN));
                            n_boxes[SetValue::UNKNOWN]++;
                        }

                        if ( return_result )
                            boxes[SetValue::UNKNOWN].push_front(x_before_ctc);
                    }
                }
            }
            else
            {
                IntervalVector x = x_in & x_out;
                empty = check_empty(x);

                vector<IntervalVector> x_in_l = box_diff(x_before_ctc, x_in);
                for ( const auto &i: x_in_l )
                {
                    if ( display_result )
                    {
                        vibes::drawBox(i, cm.at(SetValue::IN));
                        n_boxes[SetValue::IN]++;
                    }

                    if ( return_result )
                        boxes[SetValue::IN].push_front(i);
                }

                vector<IntervalVector> x_out_l = box_diff(x_before_ctc, x_out);
                for ( const auto &o: x_out_l )
                {
                    if ( display_result )
                    {
                        vibes::drawBox(o, cm.at(SetValue::OUT));
                        n_boxes[SetValue::OUT]++;
                    }

                    if ( return_result )
                        boxes[SetValue::OUT].push_front(o);
                }
                if ( empty)
                    continue;

                else
                {
                    if ( x.max_diam() < precision )
                    {
                        if ( display_result )
                        {
                            vibes::drawBox(x.subvector(0, 1), cm.at(SetValue::UNKNOWN));
                            n_boxes[SetValue::UNKNOWN]++;
                        }

                        if ( return_result )
                            boxes[SetValue::UNKNOWN].push_front(x);
                    }

                    else
                    {
                        pair<IntervalVector, IntervalVector> p = bisector.bisect(x);
                        stack.push_back(p.first);
                        stack.push_back(p.second);
                    }
                }
            }
        }
        if ( display_result )
        {
            printf("Computation time: %.2fs\n", (double) (clock() - t_start) / CLOCKS_PER_SEC);
            cout << "  Contractions:   " << k << endl;
            cout << "  IN boxes:       " << n_boxes[SetValue::IN] << endl;
            cout << "  OUT boxes:      " << n_boxes[SetValue::OUT] << endl;
            cout << "  UNKNOWN boxes:  " << n_boxes[SetValue::UNKNOWN] << endl;
        }

        return boxes;
    }
}