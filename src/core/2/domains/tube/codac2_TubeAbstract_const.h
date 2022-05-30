/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_TUBEVECTOR_CONST_H__
#define __CODAC2_TUBEVECTOR_CONST_H__

#include <list>
#include <memory>

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_TrajectoryVector.h"

namespace codac2
{
  using codac::Tube;
  using codac::Trajectory;
  using codac::TrajectoryVector;
  using codac::Interval;
  using codac::IntervalVector;

  template<typename I, typename T, typename V>
  class TubeAbstract_const
  {
    public:

      TubeAbstract_const()
      {

      }

      explicit TubeAbstract_const(const T& x);

      virtual size_t size() const = 0;
      virtual bool contains(const V& value) const = 0;
      virtual Interval t0_tf() const = 0;
      virtual I codomain() const = 0;
      // virtual I operator()(double t) const = 0;
      //virtual I operator()(const Interval& t) const = 0;

      //TubeVectorComponent operator[](size_t index);
      //const TubeVectorComponent operator[](size_t index) const;

      //friend std::ostream& operator<<(std::ostream& os, const TubeVector_& x);
      void print(std::ostream& os) const
      {
        os << t0_tf()
           << "↦" << codomain()
           << std::flush;
      }
  };

  class Tube_const : public TubeAbstract_const<Interval,Tube_const,Trajectory>
  {
    
  };

  class TubeVector_const : public TubeAbstract_const<IntervalVector,TubeVector_const,TrajectoryVector>
  {
    
  };

  /*class TubeMatrix_const : public TubeAbstract_const<IntervalMatrix_,TubeMatrix_,TrajectoryMatrix_>
  {
    
  };*/
} // namespace codac

#endif