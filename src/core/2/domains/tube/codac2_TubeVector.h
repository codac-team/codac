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

#ifndef __CODAC2_TUBEVECTOR_H__
#define __CODAC2_TUBEVECTOR_H__

#include <list>
#include <memory>
#include "codac_TFnc.h"
#include "codac2_TDomain.h"
#include "codac2_SliceVector.h"
#include "codac2_TubeVectorComponent.h"
#include "codac2_TubeVectorEvaluation.h"
#include "codac2_TubeAbstract_const.h"
#include "codac_TubeVector.h" // to be removed

namespace codac2
{
  using codac::TFnc;
  class TDomain;
  class TubeVectorComponent;
  class TubeVectorEvaluation;

  class TubeVector : public TubeVector_const
  {
    public:

      explicit TubeVector(size_t n, TDomain& tdomain);
      explicit TubeVector(size_t n, TDomain& tdomain, const TFnc& f);
      explicit TubeVector(const TubeVector& x);
      ~TubeVector();
      size_t size() const;

      size_t nb_slices() const;
      const SliceVector& first_slice() const;
      SliceVector& first_slice();
      const SliceVector& last_slice() const;
      SliceVector& last_slice();

      TubeVectorComponent operator[](size_t i);
      //const TubeVectorComponent operator[](size_t i) const;

      bool is_empty() const;
      bool is_unbounded() const;
      bool contains(const TrajectoryVector& value) const;

      TDomain& tdomain() const;
      Interval t0_tf() const;
      IntervalVector codomain() const;
      TubeVectorEvaluation operator()(double t);
      TubeVectorEvaluation operator()(const Interval& t);
      IntervalVector eval(double t) const;
      IntervalVector eval(const Interval& t) const;
      void set(const IntervalVector& codomain);

      friend std::ostream& operator<<(std::ostream& os, const TubeVector& x);

      codac::TubeVector to_codac1() const; // to be removed


    protected:

      friend class TubeVectorComponent;
      friend class TubeVectorEvaluation;
      TDomain& _tdomain;


    public:

      using base_container = std::list<TSlice>;

      struct iterator : public base_container::iterator
      {
        using iterator_category = typename base_container::iterator::iterator_category;
        using difference_type   = typename base_container::iterator::difference_type;

        using value_type        = SliceVector;
        using pointer           = SliceVector*;
        using reference         = SliceVector&;

        public:
          
          iterator(const TubeVector& tube_vector, base_container::iterator it) : 
            base_container::iterator(it), _tube_vector(tube_vector) { }

          reference operator*()
          {
            return ((*this)->_slices.at(&_tube_vector));
          }

        protected:

          const TubeVector& _tube_vector;
      };

      iterator begin() { return iterator(*this, _tdomain._tslices.begin()); }
      iterator end()   { return iterator(*this, _tdomain._tslices.end()); }


      struct const_iterator : public base_container::const_iterator
      {
        using iterator_category = typename base_container::const_iterator::iterator_category;
        using difference_type   = typename base_container::const_iterator::difference_type;

        using value_type        = SliceVector;
        using pointer           = const SliceVector*;
        using reference         = const SliceVector&;

        public:
          
          const_iterator(const TubeVector& tube_vector, base_container::const_iterator it) : 
            base_container::const_iterator(it), _tube_vector(tube_vector) { }

          reference operator*() const
          {
            return ((*this)->_slices.at(&_tube_vector));
          }

        protected:

          const TubeVector& _tube_vector;
      };

      const_iterator begin() const { return const_iterator(*this, _tdomain._tslices.cbegin()); }
      const_iterator end() const   { return const_iterator(*this, _tdomain._tslices.cend()); }
  };
} // namespace codac

#endif