/** 
 *  TPlane class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TPLANE_H__
#define __TUBEX_TPLANE_H__

#include "tubex_Paving.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  class TPlane : public Paving
  {
    public:

      TPlane(const ibex::IntervalVector& tbox);
      void compute(float precision, const TubeVector& x, const TubeVector& v);

    protected:

  };
}

#endif