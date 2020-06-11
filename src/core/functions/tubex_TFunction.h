/** 
 *  TFunction class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TFUNCTION_H__
#define __TUBEX_TFUNCTION_H__

#include <string>
#include "ibex_Function.h"
#include "tubex_TFnc.h"
#include "tubex_Trajectory.h"
#include "tubex_TrajectoryVector.h"

namespace tubex
{
  class Slice;
  class Trajectory;
  class TrajectoryVector;
  
  class TFunction : public TFnc
  {
    public:

      TFunction(const char* y);
      TFunction(const char* x1, const char* y);
      TFunction(const char* x1, const char* x2, const char* y);
      TFunction(const char* x1, const char* x2, const char* x3, const char* y);
      TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* y);
      TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y);
      TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y);
      TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y);
      TFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y);
      TFunction(int n, const char** x, const char* y);
      TFunction(const TFunction& f);
      ~TFunction();
      
      const TFunction& operator=(const TFunction& f);
      const TFunction operator[](int i) const;

      const std::string& expr() const;
      const std::string arg_name(int i) const;

      using TFnc::eval;
      // todo: using TFnc::eval_vector?
      // todo: keep using TFnc::eval?

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

      const TFunction diff() const;

    protected:

      void construct_from_array(int n, const char** x, const char* y);

      ibex::Function *m_ibex_f = NULL;
      std::string m_expr; // stored here because impossible to get this value from ibex::Function
  };
}

#endif