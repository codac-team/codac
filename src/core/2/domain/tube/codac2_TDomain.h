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

#ifndef __CODAC2_TDOMAIN_H__
#define __CODAC2_TDOMAIN_H__

#include <map>
#include <list>
#include <vector>
#include <memory>

#include "codac_Interval.h"
#include "codac2_TSlice.h"

namespace codac2
{
  using codac::Interval;

  class TDomain
  {
    public:

      explicit TDomain(const Interval& t0_tf, double dt, bool with_gates = false);
      const Interval t0_tf() const; // todo: keep this method?
      std::list<TSlice>::iterator iterator_tslice(double t);
      size_t nb_tslices() const;
      size_t nb_tubes() const;
      std::list<TSlice>::iterator sample(double t, bool allow_gate = true);
      friend std::ostream& operator<<(std::ostream& os, const TDomain& x);


    protected:

      const std::list<TSlice>& tslices() const;

      friend class SliceVector;
      friend class TubeVector;
      std::list<TSlice> _tslices;
  };
} // namespace codac

#endif