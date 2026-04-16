/** 
 *  \file codac2_SliceBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include <memory>

namespace codac2
{
  class Interval;
  class TSlice;
  class SlicedTubeBase;

  /**
   * \class SliceBase
   * \brief Base class for codomain slices attached to a temporal slice
   *
   * A ``SliceBase`` represents the codomain of a sliced tube over one temporal
   * slice of a ``TDomain``.
   *
   * This abstract class stores:
   * - a reference to the parent ``SlicedTubeBase``,
   * - an iterator to the corresponding ``TSlice`` in the temporal partition.
   *
   * Concrete derived classes, such as ``Slice<T>``, provide the actual codomain
   * representation and the associated operations.
   */
  class SliceBase
  {
    public:

      /**
       * \brief Virtual destructor
       */
      virtual ~SliceBase() = default;

      /**
       * \brief Duplicates this slice
       *
       * \return shared pointer to a copy of this slice
       */
      virtual std::shared_ptr<SliceBase> copy() const = 0;

      /**
       * \brief Initializes this slice to its unbounded codomain
       */
      virtual void init() = 0;

      /**
       * \brief Sets this slice to the empty codomain
       */
      virtual void set_empty() = 0;

      /**
       * \brief Returns the temporal domain of this slice
       *
       * \return temporal interval associated with this slice
       */
      const Interval& t0_tf() const;

      /**
       * \brief Returns the temporal slice associated with this object
       *
       * \return reference to the underlying ``TSlice``
       */
      const TSlice& tslice() const;

      /**
       * \brief Returns the previous slice of the same tube
       *
       * The previous slice is searched in the temporal element preceding the
       * current ``TSlice``.
       *
       * \return shared pointer to the previous slice, or ``nullptr`` if this
       *         slice is the first one
       */
      std::shared_ptr<const SliceBase> prev_slice() const;

      /**
       * \brief Returns the next slice of the same tube
       *
       * The next slice is searched in the temporal element following the current
       * ``TSlice``.
       *
       * \return shared pointer to the next slice, or ``nullptr`` if this slice
       *         is the last one
       */
      std::shared_ptr<const SliceBase> next_slice() const;

    protected:

      /**
       * \brief Creates a slice attached to a tube and a temporal slice
       *
       * \param tube sliced tube owning this slice
       * \param it_tslice iterator to the associated temporal slice
       */
      SliceBase(const SlicedTubeBase& tube, const std::list<TSlice>::iterator& it_tslice);
        
      /**
       * \brief Parent sliced tube
       */
      const SlicedTubeBase& _tube;
      
      /**
       * \brief Iterator to the associated temporal slice
       */
      std::list<TSlice>::iterator _it_tslice;

      friend class TDomain;
  };
}