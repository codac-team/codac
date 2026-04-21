/** 
 *  \file codac2_TubeBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Domain.h"
#include "codac2_TDomain.h"

namespace codac2
{
  /**
   * \class TubeBase
   * \brief Base class for tubes defined over a temporal domain
   *
   * A ``TubeBase`` stores the temporal domain shared by a tube.
   * This temporal domain is represented by a ``TDomain`` object and defines the
   * global time interval \f$[t_0,t_f]\f$ on which the tube is defined.
   *
   * This class provides common services for all tube types:
   * - access to the underlying temporal domain,
   * - access to its global temporal interval.
   */
  class TubeBase : public Domain
  {
    public:

      /**
       * \brief Creates a tube over a given temporal domain
       *
       * \param tdomain shared temporal domain of this tube
       */
      explicit TubeBase(const std::shared_ptr<TDomain>& tdomain)
        : _tdomain(tdomain)
      {
        assert_release(tdomain != nullptr);
      }

      /**
       * \brief Returns the temporal domain of this tube
       *
       * \return shared pointer to the associated ``TDomain``
       */
      inline const std::shared_ptr<TDomain>& tdomain() const
      {
        return _tdomain;
      }

      /**
       * \brief Returns the global temporal interval of this tube
       *
       * This method is a shortcut to ``tdomain()->t0_tf()``.
       *
       * \return temporal interval \f$[t_0,t_f]\f$ of this tube
       */
      inline Interval t0_tf() const
      {
        return _tdomain->t0_tf();
      }

    protected:

      /**
       * \brief Shared temporal domain of this tube
       */
      const std::shared_ptr<TDomain> _tdomain;
  };
}