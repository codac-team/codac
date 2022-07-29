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

#ifndef __CODAC2_ABSTRACTCONSTTUBE_H__
#define __CODAC2_ABSTRACTCONSTTUBE_H__

#include <list>
#include <memory>

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_TrajectoryVector.h"

namespace codac2
{
  using codac::Trajectory;
  using codac::TrajectoryVector;
  using codac::Interval;
  using codac::IntervalVector;

  template<typename W, typename T>
  class AbstractConstTube
  {
    public:

      AbstractConstTube()
      {

      }

      explicit AbstractConstTube(const T& x);

      virtual size_t size() const = 0;
      virtual bool contains(const TrajectoryVector& value) const = 0;
      virtual Interval t0_tf() const = 0;
      virtual W codomain() const = 0;
      // virtual W operator()(double t) const = 0;
      //virtual W operator()(const Interval& t) const = 0;

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

} // namespace codac

#endif