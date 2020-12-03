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

#ifndef __TUBEX_CAPDINTEGRATEODE_H__
#define __TUBEX_CAPDINTEGRATEODE_H__

#include "tubex_TubeVector.h"
#include "tubex_TFunction.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  TubeVector CAPD_integrateODE(
    const ibex::Interval& tdomain, const TFunction& f, const ibex::IntervalVector& x0, double dt = 0.);
}

#endif