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

#ifndef __CODAC2_ABSTRACTSLICE_H__
#define __CODAC2_ABSTRACTSLICE_H__

#include <list>
#include <variant>
#include <memory>
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_TrajectoryVector.h"
#include "codac_Exception.h"
#include "codac2_AbstractSlicedTube.h"

namespace codac2
{
  using codac::Interval;
  using codac::IntervalVector;
  using codac::TrajectoryVector;

  class TSlice;

  class AbstractSlice
  {
    public:

      AbstractSlice(const AbstractSlicedTube& tubevector, const std::list<TSlice>::iterator& _it_tslice);
      virtual std::shared_ptr<AbstractSlice> duplicate() const = 0;
      virtual size_t size() const = 0;

      const Interval& t0_tf() const;
      const TSlice& tslice() const;

      const std::shared_ptr<AbstractSlice> prev_abstract_slice_ptr() const;
      const std::shared_ptr<AbstractSlice> next_abstract_slice_ptr() const;


    protected:
        
      const AbstractSlicedTube& _tubevector;
      std::list<TSlice>::iterator _it_tslice;

      friend class TDomain;
  };
}

#endif