/* ============================================================================
 *  tubex-lib - VibesFigure_Polygon class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_VibesFigure_Polygon.h"
#include "tubex_VibesFigure.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  void VibesFigure_Polygon::draw(const Polygon& p, const vibes::Params& params)
  {
    vector<double> v_t, v_x;

    for(int i = 0 ; i < p.nb_vertices() ; i++)
    {
      v_t.push_back(VibesFigure::trunc_inf(p[i].t().mid()));
      v_x.push_back(VibesFigure::trunc_inf(p[i].x().mid()));
    } 

    if(v_t.size() > 0)
      vibes::drawPolygon(v_t, v_x, params);
  }
}