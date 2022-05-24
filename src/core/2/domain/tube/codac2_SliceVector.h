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

#ifndef __CODAC2_SLICEVECTOR_H__
#define __CODAC2_SLICEVECTOR_H__

#include <list>
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_TrajectoryVector.h"
#include "codac2_TSlice.h"
#include "codac2_TDomain.h"

namespace codac2
{
  using codac::Interval;
  using codac::IntervalVector;
  using codac::TrajectoryVector;

  class TubeVector;
  class TSlice;

  class SliceVector
  {
    public:

      explicit SliceVector(size_t n, const TubeVector& tube_vector, std::list<std::shared_ptr<TSlice>>::iterator it_tslice);
      ~SliceVector();

      const TubeVector& tube_vector() const;

      // SliceVector objects cannot be copyable or movable,
      // as they are supposed to be connected to other SliceVector objects
      SliceVector& operator=(const SliceVector&) = delete;
      SliceVector(SliceVector&&) = delete;
      SliceVector& operator=(SliceVector&&) = delete;

      size_t size() const;

      bool contains(const TrajectoryVector& value) const;

      std::shared_ptr<const SliceVector> prev_slice() const;
      std::shared_ptr<SliceVector> prev_slice();
      std::shared_ptr<const SliceVector> next_slice() const;
      std::shared_ptr<SliceVector> next_slice();

      const Interval& tdomain() const;

      const IntervalVector& codomain() const;
      const IntervalVector input_gate() const;
      const IntervalVector output_gate() const;

      void set(const IntervalVector& codomain);

      friend std::ostream& operator<<(std::ostream& os, const SliceVector& x);


    protected:

      friend class TubeVector;
      //friend class TubeVectorComponent;
      
      const TubeVector& _tube_vector;
      std::list<std::shared_ptr<TSlice>>::iterator _it_tslice;
      IntervalVector _codomain;
  };
} // namespace codac

#endif