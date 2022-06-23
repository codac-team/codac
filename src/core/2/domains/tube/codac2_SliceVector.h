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
#include <variant>
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_TrajectoryVector.h"
#include "codac2_TSlice.h"
#include "codac2_TDomain.h"
#include "codac2_IParals.h"

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

      explicit SliceVector(size_t n, const TubeVector& tube_vector, const std::list<TSlice>::iterator& it_tslice);
      explicit SliceVector(const IntervalVector& box, const TubeVector& tube_vector, const std::list<TSlice>::iterator& it_tslice);
      explicit SliceVector(const IParals& ad, const TubeVector& tube_vector, const std::list<TSlice>::iterator& it_tslice);
      ~SliceVector();
      
      SliceVector(const SliceVector& s);

      const TubeVector& tube_vector() const;

      // SliceVector objects cannot be copyable or movable,
      // as they are supposed to be connected to other SliceVector objects
      SliceVector& operator=(const SliceVector&) = delete;
      SliceVector(SliceVector&&) = delete;
      SliceVector& operator=(SliceVector&&) = delete;

      size_t size() const;

      bool is_gate() const;
      bool is_empty() const;
      bool is_unbounded() const;
      bool contains(const TrajectoryVector& value) const;

      const SliceVector* prev_slice() const;
      SliceVector* prev_slice();
      const SliceVector* next_slice() const;
      SliceVector* next_slice();

      const Interval& t0_tf() const;
      const TSlice& tslice() const;

      const IntervalVector& codomain() const;
      IntervalVector input_gate() const;
      IntervalVector output_gate() const;

      const IParals& codomainI();
      IParals codomainI() const;
      IParals input_gateI() const;
      IParals output_gateI() const;

      void set(const IntervalVector& x);
      void set(const IParals& ip);
      void set_component(size_t i, const Interval& xi);

      friend std::ostream& operator<<(std::ostream& os, const SliceVector& x);


    protected:

      friend class TubeVector;
      friend class TubeVectorComponent;
      friend class TDomain;
      friend class TSlice;
      
      const TubeVector& _tubevector;
      std::list<TSlice>::iterator _it_tslice;

      // Several abstract domains related to guaranteed integration can be considered here
      std::variant<IntervalVector,IParals> _codomain;
  };
} // namespace codac

#endif
