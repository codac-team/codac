/** 
 *  \file codac2_CtcInter.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  /**
   * \class CtcInter
   * \brief Sequential intersection of several contractors:
   * @f[
   * \mathcal{C}_{\mathrm{inter}} = \mathcal{C}_1 \cap \mathcal{C}_2 \cap \dots \cap \mathcal{C}_n.
   * @f]
   *
   * Each sub-contractor is applied successively to the current domain.
   * If two contractors implement two constraints, then their intersection corresponds to the
   * logical conjunction of these constraints. This corresponds
   * to the classical intersection/composition pattern of contractor programming.
   * 
   * ``CtcInter`` stores a collection of contractors acting on the same contracted types and
   * applies them one after the other. For instance, for contractors acting on a single box \f$[\mathbf{x}]\f$:
   * @f[
   * \left(\mathcal{C}_1 \cap \mathcal{C}_2\right)([\mathbf{x}])
   *   := \mathcal{C}_1([\mathbf{x}]) \cap \mathcal{C}_2([\mathbf{x}]).
   * @f]
   *
   * In practice, each stored contractor contracts the current domain in place; the next one
   * then works on the already reduced result. It does not perform any fixpoint iteration.
   * If the domain becomes empty at some step, the
   * contraction stops immediately.
   * 
   * A ``CtcInter`` object can be built explicitly, extended incrementally with ``&=``,
   * or constructed implicitly with the binary ``&`` operator between two contractors.
   *    
   * In C++, a variadic ``.contract(..)`` function is available when the intersected contractors
   * act on several domains rather than on a single box. Such contractors can be combined provided
   * they are homogeneous, *i.e.* they act on the same sequence of input types.
   * 
   * \tparam X Contracted domain type(s). For the common box-contractor case, this is typically
   *         ``IntervalVector``.
   */
  template<typename... X>
  class CtcInter : public Ctc<CtcInter<X...>,X...>
  {
    public:

      /**
       * \brief Builds a neutral intersection contractor with a prescribed domain size.
       *
       * This constructor is mainly useful when building a contractor incrementally with
       * ``operator&=``. Otherwise it has no effect.
       *
       * \param n Dimension of the contracted object.
       *
       * \note When ``X...`` is ``Interval``, the size must be ``1``.
       */
      explicit CtcInter(Index n)
        : Ctc<CtcInter<X...>,X...>(n)
      {
        if constexpr(std::is_same_v<X...,Interval>)
        {
          assert(n == 1);
        }
      }

      /**
       * \brief Builds an intersection contractor from a single contractor.
       *
       * The contractor is stored internally in the collection of composed contractors.
       *
       * \tparam C Type of the contractor or contractor pointer.
       * \param c Contractor to store.
       */
      template<typename C>
        requires (IsCtcBaseOrPtr<C,X...> && !std::is_same_v<CtcInter<X...>,C>)
      CtcInter(const C& c)
        : Ctc<CtcInter<X...>,X...>(size_of(c)), _ctcs(c)
      { }

      /**
       * \brief Builds an intersection contractor from several contractors.
       *
       * All contractors must act on contracted objects of the same size.
       *
       * \tparam C Contractor or contractor-pointer types.
       * \param c Contractors to compose sequentially.
       */
      template<typename... C>
        requires (IsCtcBaseOrPtr<C,X...> && ...)
      CtcInter(const C&... c)
        : Ctc<CtcInter<X...>,X...>(size_first_item(c...)), _ctcs(c...)
      {
        assert_release(all_same_size(c...));
      }

      /**
       * \brief Builds an intersection contractor from a collection of contractors.
       *
       * All contractors must act on contracted objects of the same size.
       *
       * \param ctcs Collection of contractors sequentially.
       */
      CtcInter(const Collection<CtcBase<X...>>& ctcs)
        : Ctc<CtcInter<X...>,X...>(ctcs.front()->size()), _ctcs(ctcs)
      {
        for(const auto& ci : _ctcs)
        {
          assert_release(ci->size() == this->size());
        }
      }

      /**
       * \brief Builds an intersection contractor from a std::initializer_list of contractors.
       *
       * All contractors must act on contracted objects of the same size.
       *
       * \param ctcs list of contractors.
       */
      template<typename C>
        requires IsCtcBaseOrPtr<C,X...>
      CtcInter(std::initializer_list<C> ctcs)
        : CtcInter(Collection<CtcBase<X...>>(ctcs))
      { }

      /**
       * \brief Returns the number of stored contractors.
       *
       * \return Size of the internal contractor collection.
       */
      size_t nb() const
      {
        return _ctcs.size();
      }

      /**
       * \brief Contracts the given domain(s) by applying all stored contractors in sequence.
       *
       * The domains are updated in place. If one contractor empties one of the domains, the remaining
       * contractors are not evaluated.
       * 
       * \param x Domain(s) to contract.
       */
      void contract(X&... x) const
      {
        for(const auto& ci : _ctcs)
        {
          ci->contract(x...);
          if((x.is_empty() | ...))
            return;
        }
      }

      /**
       * \brief Appends a contractor to the current intersection.
       *
       * The contractor is added at the end of the internal sequence, so it will be applied
       * after the already stored ones.
       *
       * \tparam C Type of the contractor or contractor pointer.
       * \param c Contractor to append.
       * \return A reference to ``*this``.
       */
      template<typename C>
        requires IsCtcBaseOrPtr<C,X...>
      CtcInter<X...>& operator&=(const C& c)
      {
        assert_release(size_of(c) == this->size());
        _ctcs.push_back(c);
        return *this;
      }

    protected:

      /** \brief Internal collection of contractors composing the intersection. */
      Collection<CtcBase<X...>> _ctcs;
  };

  /**
   * \brief Default ``CtcInter`` specialization for box contractors.
   *
   * ``CtcInter<>`` is an alias-like specialization inheriting from
   * ``CtcInter<IntervalVector>``.
   */
  template <>
  class CtcInter<> : public CtcInter<IntervalVector>
  { };

  /**
   * \brief Helper meta-function returning the appropriate ``CtcInter`` type from a tuple.
   *
   * \tparam Tuple Tuple of contracted types.
   */
  template<typename Tuple>
  struct CtcInterType;

  /**
   * \brief ``CtcInterType`` specialization for tuples of contracted types.
   *
   * \tparam T Contracted types extracted from the tuple.
   */
  template<typename... T>
  struct CtcInterType<std::tuple<T...>> {
    using Ctc = CtcInter<T...>;
  };

  /**
   * \brief Builds an intersection contractor from two contractors.
   *
   * This overload enables the natural syntax ``c1 & c2``.
   *
   * \tparam C1 Type of the first contractor.
   * \tparam C2 Type of the second contractor.
   * \param c1 First contractor.
   * \param c2 Second contractor.
   * \return The intersection contractor storing ``c1`` and ``c2``.
   */
  template<typename C1,typename C2>
  typename CtcInterType<typename C1::ContractedTypes>::Ctc operator&(const C1& c1, const C2& c2)
  {
    return { c1, c2 };
  }

  /**
   * \brief Builds an intersection contractor from two shared contractor pointers.
   *
   * \tparam C1 Type of the first pointed contractor.
   * \tparam C2 Type of the second pointed contractor.
   * \param c1 Shared pointer to the first contractor.
   * \param c2 Shared pointer to the second contractor.
   * \return The resulting intersection contractor.
   */
  template<typename C1,typename C2>
  typename CtcInterType<typename C1::ContractedTypes>::Ctc operator&(const std::shared_ptr<C1>& c1, const std::shared_ptr<C2>& c2)
  {
    return { c1, c2 };
  }

  /**
   * \brief Builds an intersection contractor from a shared contractor pointer and a contractor.
   *
   * \tparam C1 Type of the first pointed contractor.
   * \tparam C2 Type of the second contractor.
   * \param c1 Shared pointer to the first contractor.
   * \param c2 Second contractor.
   * \return The resulting intersection contractor.
   */
  template<typename C1,typename C2>
  typename CtcInterType<typename C1::ContractedTypes>::Ctc operator&(const std::shared_ptr<C1>& c1, const C2& c2)
  {
    return { c1, c2 };
  }

  /**
   * \brief Builds an intersection contractor from a contractor and a shared contractor pointer.
   *
   * \tparam C1 Type of the first contractor.
   * \tparam C2 Type of the second pointed contractor.
   * \param c1 First contractor.
   * \param c2 Shared pointer to the second contractor.
   * \return The resulting intersection contractor.
   */
  template<typename C1,typename C2>
  typename CtcInterType<typename C1::ContractedTypes>::Ctc operator&(const C1& c1, const std::shared_ptr<C2>& c2)
  {
    return { c1, c2 };
  }
  
  /**
   * \brief Intersects a box with a contractor by wrapping the box into a ``CtcWrapper``.
   *
   * This overload enables expressions such as ``box & ctc``.
   *
   * \tparam C2 Type of the contractor.
   * \param c1 Box interpreted as a wrapper contractor.
   * \param c2 Contractor.
   * \return The resulting box-contractor intersection.
   */
  template<typename C2>
    requires std::is_base_of_v<CtcBase<IntervalVector>,C2>
  inline CtcInter<IntervalVector> operator&(const IntervalVector& c1, const C2& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcInter<IntervalVector>(CtcWrapper(c1),c2);
  }

  /**
   * \brief Intersects a contractor with a box by wrapping the box into a ``CtcWrapper``.
   *
   * This overload enables expressions such as ``ctc & box``.
   *
   * \tparam C1 Type of the contractor.
   * \param c1 Contractor.
   * \param c2 Box interpreted as a wrapper contractor.
   * \return The resulting box-contractor intersection.
   */
  template<typename C1>
    requires std::is_base_of_v<CtcBase<IntervalVector>,C1>
  inline CtcInter<IntervalVector> operator&(const C1& c1, const IntervalVector& c2)
  {
    assert_release(c1.size() == c2.size());
    return CtcInter<IntervalVector>(c1,CtcWrapper(c2));
  }

  // Template deduction guides

  CtcInter(Index) -> CtcInter<IntervalVector>;

  template<typename... C>
    requires (IsCtcBaseOrPtr<C,IntervalVector> && ...)
  CtcInter(const C&...) -> CtcInter<IntervalVector>;

  template<typename C>
  CtcInter(std::initializer_list<C>) -> CtcInter<IntervalVector>;
}