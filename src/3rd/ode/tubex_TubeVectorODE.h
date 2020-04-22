/**
 *  \file
 *  CAPD to Tubex
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef TUBEX_TUBEX_TUBEVECTORODE_H
#define TUBEX_TUBEX_TUBEVECTORODE_H

#define DEFAULT_TIMESTEP 0
#define CAPD_MODE 0


#include "tubex_TubeVector.h"
#include "ibex_IntervalVector.h"
#include "tubex_capd2tubex.h"

namespace tubex
{
    int TubeVectorODE(tubex::TubeVector& result, const tubex::Function& f, const ibex::IntervalVector& x0,
                      const double timestep=DEFAULT_TIMESTEP, const int mode=CAPD_MODE);

}


#endif //TUBEX_TUBEX_TUBEVECTORODE_H
