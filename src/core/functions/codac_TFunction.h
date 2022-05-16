/** 
 *  TFunction class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_TFUNCTION_H__
#define __CODAC_TFUNCTION_H__

#include <string>
#include "codac_Function.h"
#include "codac_TFnc.h"
#include "codac_Trajectory.h"
#include "codac_TrajectoryVector.h"

namespace codac
{
  class Slice;
  class Trajectory;
  class TrajectoryVector;

  std::string to_string(const Function& f);
  
  class TFunction : public TFnc
  {
    public:

      TFunction(const Function& f);
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
      const std::string expr(int i) const;
      const Function& getFunction() const;
      const std::string arg_name(int i) const;

      using TFnc::eval;
      // todo: using TFnc::eval_vector?
      // todo: keep using TFnc::eval?

      const Tube eval(const TubeVector& x) const;
      const Trajectory traj_eval(const TrajectoryVector& x) const;
      const Interval eval(const Interval& t) const;
      const Interval eval(const IntervalVector& x) const;
      const Interval eval(int slice_id, const TubeVector& x) const;
      const Interval eval(const Interval& t, const TubeVector& x) const;

      const TubeVector eval_vector(const TubeVector& x) const;
      const TrajectoryVector traj_eval_vector(const TrajectoryVector& x) const;
      const IntervalVector eval_vector(const Interval& t) const;
      const IntervalVector eval_vector(const IntervalVector& x) const;
      const IntervalVector eval_vector(int slice_id, const TubeVector& x) const;
      const IntervalVector eval_vector(const Interval& t, const TubeVector& x) const;

      const TFunction diff() const;

    protected:

      void construct_from_array(int n, const char** x, const char* y);

      Function *m_ibex_f = nullptr;
      std::string m_expr; // stored here because impossible to get this value from Function
  };
}

#endif