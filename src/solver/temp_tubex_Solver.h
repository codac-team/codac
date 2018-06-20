/* ============================================================================
 *  tubex-lib - Solver class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef Solver_HEADER
#define Solver_HEADER

#include <vector>
#include "tubex_Tube.h"

namespace tubex
{
  class Solver
  {
    public:

      Solver();
      double tubeVectorVolume(const std::vector<Tube>& v_x) const;
      double tubeVectorIsEmpty(const std::vector<Tube>& v_x) const;
      std::vector<std::vector<Tube> > solve(std::vector<Tube>& v_x,
                                            void (*ctc_func)(std::vector<Tube>&),
                                            float max_thickness,
                                            float refining_ratio = 0.005,
                                            float fixed_point_ratio = 0.005);

    protected:
      
  };
}

#endif