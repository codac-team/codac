/** 
 *  Function class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_FUNCTION_H__
#define __TUBEX_FUNCTION_H__

#include "ibex_Function.h"
#include "tubex_Fnc.h"
#include "tubex_Trajectory.h"
#include "tubex_TrajectoryVector.h"

namespace tubex
{
  class Slice;
  class Trajectory;
  class TrajectoryVector;
  
  class Function : public Fnc
  {
    public:

      Function(const char* y);
      Function(const char* x1, const char* y);
      Function(const char* x1, const char* x2, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y);
      Function(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y);
      Function(int n, const char** x, const char* y);
      Function(const tubex::Function& f);
      ~Function();
      
      const tubex::Function& operator=(const tubex::Function& f);
      const tubex::Function operator[](int i) const;
      const ibex::Function& ibex_function() const;

      using Fnc::eval;
      // todo: using Fnc::eval_vector?
      // todo: keep using Fnc::eval?

      const Tube eval(const TubeVector& x) const;
      const Trajectory traj_eval(const TrajectoryVector& x) const;
      const ibex::Interval eval(const ibex::Interval& t) const;
      const ibex::Interval eval(const ibex::IntervalVector& x) const;
      const ibex::Interval eval(int slice_id, const TubeVector& x) const;
      const ibex::Interval eval(const ibex::Interval& t, const TubeVector& x) const;

      const TubeVector eval_vector(const TubeVector& x) const;
      const TrajectoryVector traj_eval_vector(const TrajectoryVector& x) const;
      const ibex::IntervalVector eval_vector(const ibex::Interval& t) const;
      const ibex::IntervalVector eval_vector(const ibex::IntervalVector& x) const;
      const ibex::IntervalVector eval_vector(int slice_id, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(const ibex::Interval& t, const TubeVector& x) const;
      const Function diff() const;

    protected:

      void construct_from_array(int n, const char** x, const char* y);

      ibex::Function *m_ibex_f = NULL;
  };
}

#endif
