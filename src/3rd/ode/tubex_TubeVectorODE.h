/**
 *  \file
 *  TubeVectorODE class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Julien Damers
 *  \copyright  Copyright 2020 Tubex
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TUBEVECTORODE_H__
#define __TUBEX_TUBEVECTORODE_H__

#define DEFAULT_TIMESTEP 0
#define CAPD_MODE 0

#include "tubex_TubeVector.h"
#include "ibex_IntervalVector.h"
#include "tubex_capd2tubex.h"

namespace tubex
{
  TubeVector TubeVectorODE(const ibex::Interval& domain, const TFunction& f, const ibex::IntervalVector& x0,
                           double timestep=DEFAULT_TIMESTEP, int mode=CAPD_MODE);

}


#endif
