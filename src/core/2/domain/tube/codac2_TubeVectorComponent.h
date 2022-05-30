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

#ifndef __CODAC2_TUBEVECTORCOMPONENT_H__
#define __CODAC2_TUBEVECTORCOMPONENT_H__

#include <memory>
#include "codac_Interval.h"
#include "codac_Trajectory.h"
#include "codac2_TubeVector.h"
#include "codac2_TubeAbstract_const.h"
#include "ibex_Interval.h"

namespace codac2
{
  using codac::Interval;
  using codac::Trajectory;

  class TubeVector;

  class TubeVectorComponent : public Tube_const
  {
    public:

      TubeVectorComponent(const TubeVectorComponent& tubevector_i);

      size_t size() const;
      const TDomain& tdomain() const;
      Interval t0_tf() const;
      Interval codomain() const;

      bool contains(const Trajectory& value) const;

      void set(const Interval& codomain);
      const TubeVectorComponent& operator=(const TubeVectorComponent& x);
      const TubeVectorComponent& operator=(std::pair<std::function<Interval(const Interval&)>,const TubeVectorComponent> x);

      friend std::ostream& operator<<(std::ostream& os, const TubeVectorComponent& x);
      

    protected:

      friend class TubeVector;
      explicit TubeVectorComponent(TubeVector& tubevector, size_t i);

      size_t _i;
      TubeVector& _tubevector;
  };

  std::pair<std::function<Interval(const Interval&)>,const TubeVectorComponent> cos(const TubeVectorComponent& x);


} // namespace codac

#endif