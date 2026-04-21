/** 
 *  \file codac2_TSlice.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>

namespace codac2
{
  class Interval;
  class SliceBase;
  class SlicedTubeBase;

  /**
   * \class TSlice
   * \brief Temporal slice shared by sliced tubes.
   *
   * A ``TSlice`` represents a temporal interval of a ``TDomain``.
   * It inherits from ``Interval`` and may correspond either to:
   * - a non-degenerate time slice \f$[t_i,t_{i+1}]\f$, or
   * - a degenerate interval \f$[t_i,t_i]\f$, referred to as a gate.
   *
   * For each sliced tube defined on the same temporal partition, this object
   * stores the corresponding ``SliceBase`` instance.
   */
  class TSlice : public Interval
  {
    public:

      /**
       * \brief Creates a temporal slice over the given time interval
       *
       * \param tdomain temporal interval associated with this slice
       */
      explicit TSlice(const Interval& tdomain);

      /**
       * \brief Creates a temporal slice from another one over a new time interval
       *
       * This constructor duplicates the slice container associated with each
       * tube by performing a deep copy of the underlying ``SliceBase`` objects.
       *
       * \param tslice source temporal slice
       * \param tdomain temporal interval associated with the copied slice
       */
      TSlice(const TSlice& tslice, const Interval& tdomain); // performs a deep copy on slices

      /**
       * \brief Tests whether this temporal slice is a gate
       *
       * A temporal slice is considered as a gate when its temporal interval is
       * degenerate, i.e., when \f$t^-=t^+\f$.
       *
       * \return ``true`` if this slice is a gate, ``false`` otherwise
       */
      bool is_gate() const;

      /**
       * \brief Returns the slices attached to this temporal slice
       *
       * The returned map associates each ``SlicedTubeBase`` defined on the same
       * temporal partition with its corresponding ``SliceBase`` over this
       * temporal interval.
       *
       * \return constant reference to the map of attached slices
       */
      const std::map<const SlicedTubeBase*,std::shared_ptr<SliceBase>>& slices() const;

      using Interval::operator==;

    protected:

      /**
       * \brief Slices attached to each sliced tube over this temporal interval
       */
      std::map<const SlicedTubeBase*,std::shared_ptr<SliceBase>> _slices;

      friend class TDomain;
      friend class SlicedTubeBase;
      template<typename T>
      friend class SlicedTube;
  };
}