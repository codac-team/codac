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
#include "codac2_Slice.h"

namespace codac2
{
  class TubeBase : public Domain
  {
    public:

      TubeBase(const std::shared_ptr<TDomain>& tdomain)
        : _tdomain(tdomain)
      { }

      inline const std::shared_ptr<TDomain>& tdomain() const
      {
        return _tdomain;
      }

      inline Interval t0_tf() const
      {
        return _tdomain->t0_tf();
      }

    protected:

      const std::shared_ptr<TDomain> _tdomain;
  };
}