/**
 *  TubeVectorODE class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_capd_integrateODE.h"
#include "tubex_capd2tubex.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;
using namespace tubex;

namespace tubex
{
  TubeVector CAPD_integrateODE(const Interval& tdomain, const TFunction& f, const IntervalVector& x0, double dt)
  {
    string capd_string = tubexFnc2capdString(f);

    try
    {
      capd::IMap vector_field(capd_string);
      vector<IntervalVector> ibex_curve = capd2ibex(tdomain, vector_field, x0, dt);
      return ibex2tubex(ibex_curve);
    }

    catch(exception& e)
    {
      throw Exception(__func__, e.what());
    }
  }
}