/**
 *  TubeVectorODE class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TubeVectorODE.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;
using namespace tubex;


namespace tubex
{
  TubeVector TubeVectorODE(const Interval& domain, const TFunction& f, const IntervalVector& x0,
                           double timestep, int mode)
  {
    switch(mode)
    {
      case CAPD_MODE:
        return capd2tubex(domain, f, x0, timestep);

      // Additional integration tools might be added in the future

      default:
        throw Exception("TubeVectorODE", "integration tool not available");
    }
  }
}