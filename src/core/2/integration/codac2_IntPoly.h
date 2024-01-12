/** 
 *  \file representation of polyhedron as inter M_i X_i
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_INTPOLY_H__
#define __CODAC2_INTPOLY_H__

#include <iostream>
#include <vector>
#include <memory>
#include "codac_TubeVector.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac_ConvexPolygon.h"
#include "codac2_Tube.h"
#include "codac2_expIMat.h"
#include <intflpl.h>

using namespace intflpl;


namespace codac2 
{

      /** generate a ConvexPolygon which is an overapproximation of the
       * projection of the polyhedron (basic form)
       */
      codac::ConvexPolygon over_polygon(const IntPoly &ip, const Matrix& M);

      codac::TubeVector to_codac1(codac2::Tube<IntPoly>& tube);

}

#endif
