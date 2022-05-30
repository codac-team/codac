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

#ifndef __CODAC2_TSLICE_H__
#define __CODAC2_TSLICE_H__

#include <map>
#include <list>
#include <vector>
#include <memory>

#include "codac_Interval.h"

namespace codac2
{
  using codac::Interval;

  class SliceVector;
  class TubeVector;

  class TSlice
  {
    public:

      explicit TSlice(const Interval& tdomain);
      TSlice(const TSlice& tslice, const Interval& tdomain); // performs a deep copy on slices
      const Interval& t0_tf() const;
      const std::map<const TubeVector*,SliceVector>& slices() const;
      friend std::ostream& operator<<(std::ostream& os, const TSlice& x);

    protected:

      void set_tdomain(const Interval& tdomain);

      friend class TubeVector;
      friend class TubeVectorComponent;
      friend class TubeVectorEvaluation;
      friend class TDomain;
      Interval _t0_tf;
      std::map<const TubeVector*,SliceVector> _slices;
  };
} // namespace codac

#endif