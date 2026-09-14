/** 
 *  \file codac2_SlicedTubeBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Slice.h"
#include "codac2_TubeBase.h"

namespace codac2
{
  /**
   * \class SlicedTubeBase
   * \brief Base class for tubes defined over a sliced temporal domain
   *
   * A ``SlicedTubeBase`` is a tube whose temporal domain is represented by a
   * ``TDomain`` made of ``TSlice`` objects.
   *
   * This class provides common services for all sliced tubes:
   * - access to the number of temporal elements,
   * - access to the first and last codomain slices,
   * - cleanup of the links stored in the temporal partition when the tube is
   *   destroyed.
   *
   * Each ``TSlice`` of the associated ``TDomain`` stores a map from
   * ``SlicedTubeBase*`` to ``SliceBase``. The destructor of this class removes
   * the entries associated with the current tube.
   */
  class SlicedTubeBase : public TubeBase
  {
    public:

      /**
       * \brief Creates a sliced tube over a given temporal domain
       *
       * \param tdomain shared temporal domain of this tube
       */
      SlicedTubeBase(const std::shared_ptr<TDomain>& tdomain)
        : TubeBase(tdomain)
      { }

      /**
       * \brief Destroys this sliced tube
       *
       * All references to this tube stored in the ``TSlice`` objects of the
       * associated ``TDomain`` are removed.
       */
      inline ~SlicedTubeBase()
      {
        for(auto& s : *_tdomain)
          s._slices.erase(this);
      }

      /**
       * \brief Returns the number of temporal elements of this tube
       *
       * The count includes all elements stored in the temporal partition,
       * including explicit gates if they exist.
       *
       * \return number of temporal slices of the associated ``TDomain``
       */
      inline size_t nb_slices() const
      {
        return _tdomain->nb_tslices();
      }

      /**
       * \brief Returns the first slice of this tube
       *
       * \note This method assumes that the associated temporal domain is not
       *       empty and that its first ``TSlice`` contains a slice attached to
       *       this tube.
       *
       * \return shared pointer to the first slice
       */
      inline std::shared_ptr<const SliceBase> first_slice() const
      {
        const auto& slices = _tdomain->front().slices();
        assert(slices.find(this) != slices.end());
        return slices.at(this);
      }

      /**
       * \brief Returns the last slice of this tube
       *
       * \note This method assumes that the associated temporal domain is not
       *       empty and that its last ``TSlice`` contains a slice attached to
       *       this tube.
       *
       * \return shared pointer to the last slice
       */
      inline std::shared_ptr<const SliceBase> last_slice() const
      {
        const auto& slices = _tdomain->back().slices();
        assert(slices.find(this) != slices.end());
        return slices.at(this);
      }
  };
}