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
    int TubeVectorODE(TubeVector& result, const tubex::Function& f, const ibex::IntervalVector& x0,
                                    const double timestep, const int mode)
    {
        int success = 1;
        if (mode==CAPD_MODE)
        {
            success = capd2tubex(result,f,x0,timestep);

        }
        // Additional integration tools might be added in the future


        if(success==0){ return EXIT_SUCCESS;}
        else{ return EXIT_FAILURE;}
    }
}

