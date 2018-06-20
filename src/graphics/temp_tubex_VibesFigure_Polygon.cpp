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
    vector<double> v_x, v_y;

    for(int i = 0 ; i < p.nbVertices() ; i++)
    {
      v_x.push_back(VibesFigure::truncInf(p[i].x));
      v_y.push_back(VibesFigure::truncInf(p[i].y));
    } 

    if(v_x.size() > 0)
      vibes::drawPolygon(v_x, v_y, params);
  }
}