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
#include "tubex_VIBesFigTubeVector.h"
#include "ibex_BoolInterval.h"

namespace tubex
{
  class Solver
  {
    public:

      Solver(const ibex::Vector& max_thickness);
      ~Solver();

      // Ratio:
      // 0 = no iteration (feature disabled)
      // 1 = fixed point reached up to the floating point representation
      void set_refining_fxpt_ratio(float refining_fxpt_ratio);
      void set_propa_fxpt_ratio(float propa_fxpt_ratio);
      void set_cid_fxpt_ratio(float cid_fxpt_ratio);
      
      const std::list<TubeVector> solve(const TubeVector& x0, void (*ctc_func)(TubeVector&));
      VIBesFigTubeVector* figure();
      static const ibex::BoolInterval solutions_contain(const std::list<TubeVector>& l_solutions, const TrajectoryVector& truth);

    protected:
      
      void clustering(std::list<std::pair<int,TubeVector> >& l_tubes);
      bool stopping_condition_met(const TubeVector& x);
      bool fixed_point_reached(double volume_before, double volume_after, float fxpt_ratio);
      void propagation(TubeVector &x, void (*ctc_func)(TubeVector&), float propa_fxpt_ratio);
      void cid(TubeVector &x, void (*ctc_func)(TubeVector&));

      ibex::Vector m_max_thickness = ibex::Vector(1);
      float m_refining_fxpt_ratio = 0.005;
      float m_propa_fxpt_ratio = 0.005;
      float m_cid_fxpt_ratio = 0.005;

      // Embedded graphics
      VIBesFigTubeVector *m_fig = NULL;
  };
}

#endif