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

#ifndef __TUBEX_SOLVER_H__
#define __TUBEX_SOLVER_H__

#include <list>
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_VibesFigure_TubeVector.h"

namespace tubex
{
  class Solver
  {
    public:

      Solver(const ibex::Vector& max_thickness,
             float refining_fxpt_ratio = 0.005,
             float propa_fxpt_ratio = 0.005,
             float cid_fxpt_ratio = 0.005);
      ~Solver();
      const std::list<TubeVector> solve(const TubeVector& x0, void (*ctc_func)(TubeVector&));
      VibesFigure_TubeVector* figure();
      bool solutions_contain(const std::list<TubeVector>& v_solutions, const TrajectoryVector& truth);

    protected:
      
      bool stopping_condition_met(const TubeVector& x);
      bool fixed_point_reached(int n, double volume_before, double volume_after, float fxpt_ratio);
      void propagation(TubeVector &x, void (*ctc_func)(TubeVector&), float propa_fxpt_ratio);
      void cid(TubeVector &x, void (*ctc_func)(TubeVector&));

      ibex::Vector m_max_thickness = ibex::Vector(1);
      float m_refining_fxpt_ratio;
      float m_propa_fxpt_ratio;
      float m_cid_fxpt_ratio;

      // Embedded graphics
      VibesFigure_TubeVector *m_fig = NULL;
  };
}

#endif