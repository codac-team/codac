/** 
 *  \file representation of polyhedron as inter M_i X_i
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */


#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cstdio>
#include <cstring>
#include <intflpl.h>
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac2_expIMat.h"
#include "codac_polygon_arithmetic.h"
#include "codac_ConvexPolygon.h"
#include "codac2_IntPoly.h"

using namespace intflpl;
using namespace codac;

namespace codac2 {

      /** generate a ConvexPolygon which is an overapproximation of the
       * projection of the polyhedron (basic form)
       */
    codac::ConvexPolygon over_polygon(const IntPoly &ip, const Matrix& M) {
       if (ip.get_dim()==2) {
          std::vector<double> lx;
          std::vector<double> ly;
          ip.vertices2D(lx,ly);
//          std::cout << lx.size() << " " << ly.size() << "\n";
          std::vector<codac::ThickPoint> vert;
          for (unsigned int i=0;i<lx.size();i++) {
//              std::cout << " i :" << i << "\n";
              Vector a(2);
              a[0]=lx[i]; a[1]=ly[i];
              Vector b = M*a;
//              std::cout << " a : " << a << " b : " << b << "\n";
              vert.push_back(ThickPoint(b));
          }
          return codac::ConvexPolygon(vert);
       } else {
          /* FIXME : traiter le cas dim=3, puis dim>=3 */
         return codac::ConvexPolygon(M*ip.box());
       }
    }


    codac::TubeVector to_codac1(codac2::Tube<IntPoly>& tube)
    {
      codac::TubeVector x(tube.t0_tf(), tube.size());
      for(const auto& s : tube)
        if(!s.t0_tf().is_unbounded())
          x.set(s.codomain().box(), s.t0_tf());
      for(const auto& s : tube) // setting gate (were overwritten)
        if(s.t0_tf().is_degenerated())
          x.set(s.codomain().box(), s.t0_tf());
      return x;
    }


}

