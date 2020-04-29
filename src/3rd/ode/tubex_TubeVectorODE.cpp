/**
 *  CAPD to Tubex
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TubeVectorODE.h"

using namespace std;
using namespace ibex;
using namespace tubex;


namespace tubex
{
    TubeVector TubeVectorODE(const Interval& domain, const tubex::Function& f, const ibex::IntervalVector& x0,
                                    const double timestep, const int mode)
    {

        if (mode==CAPD_MODE)
        {
            return (capd2tubex(domain,f,x0,timestep));
        }
        // Additional integration tools might be added in the future

    }
}

