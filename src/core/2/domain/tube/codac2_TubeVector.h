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
#include "codac2_TDomain.h"
#include "codac2_SliceVector.h"

#include "codac2_TubeAbstract_const.h"

namespace codac2
{
  class TDomain;
  //class SliceVector;

  class TubeVector : public TubeVector_const
  {
    public:

      explicit TubeVector(size_t n, TDomain& tdomain);
      size_t size() const;

      size_t nb_slices() const;
      const std::shared_ptr<SliceVector>& first_slice() const;
      std::shared_ptr<SliceVector>& first_slice();
      const std::shared_ptr<SliceVector>& last_slice() const;
      std::shared_ptr<SliceVector>& last_slice();

      //TubeVectorComponent operator[](size_t index);
      //const TubeVectorComponent operator[](size_t index) const;

      bool contains(const TrajectoryVector& value) const;

      TDomain& tdomain() const;
      Interval t0_tf() const;
      IntervalVector codomain() const;
      IntervalVector operator()(double t) const;
      IntervalVector operator()(const Interval& t) const;
      void set(const IntervalVector& codomain);

      friend std::ostream& operator<<(std::ostream& os, const TubeVector& x);


    protected:

      //friend class TubeVectorComponent;
      TDomain& _tdomain;


    public:

      struct Iterator
      {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SliceVector;
        using pointer           = std::shared_ptr<SliceVector>;
        using reference         = SliceVector&;

        public:

          Iterator(pointer ptr) : m_ptr(ptr) { }

          reference operator*() const
          {
            return *m_ptr;
          }

          pointer operator->()
          {
            return m_ptr;
          }

          Iterator& operator++()
          {
            if(m_ptr != nullptr)
              m_ptr = m_ptr->next_slice();
            return *this;
          }

          friend bool operator==(const Iterator& a, const Iterator& b)
          {
            return a.m_ptr == b.m_ptr;
          };

          friend bool operator!=(const Iterator& a, const Iterator& b)
          {
            return a.m_ptr != b.m_ptr;
          };

        private:

          pointer m_ptr;
      };

      Iterator begin() { return Iterator(first_slice()); }
      Iterator end()   { return Iterator(last_slice()); }

      struct ConstIterator
      {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SliceVector;
        using pointer           = const std::shared_ptr<SliceVector>;
        using reference         = const SliceVector&;

        public:

          ConstIterator(pointer ptr) : m_ptr(ptr) { }

          reference operator*() const
          {
            return *m_ptr;
          }

          pointer operator->()
          {
            return m_ptr;
          }

          const ConstIterator& operator++()
          { 
            m_ptr = m_ptr->next_slice();
            return *this;
          }

          friend bool operator==(const ConstIterator& a, const ConstIterator& b)
          {
            return a.m_ptr == b.m_ptr;
          };

          friend bool operator!=(const ConstIterator& a, const ConstIterator& b)
          {
            return a.m_ptr != b.m_ptr;
          };

        private:

          std::shared_ptr<SliceVector> m_ptr;
      };

      ConstIterator begin() const { return ConstIterator(first_slice()); }
      ConstIterator end()   const { return ConstIterator(last_slice()); }
  };
} // namespace codac

#endif