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
#include "codac_predef_values.h"

namespace codac2
{
  using codac::Interval;
  class TSlice;

  class TDomain
  {
    public:

      explicit TDomain(const Interval& t0_tf);
      explicit TDomain(const Interval& t0_tf, double dt, bool with_gates = false);
      const Interval t0_tf() const; // todo: keep this method?
      std::list<TSlice>::iterator iterator_tslice(double t); // returns it on last slice if t==t_f, not end
      size_t nb_tslices() const;
      size_t nb_tubes() const;
      bool all_gates_defined() const;
      std::list<TSlice>::iterator sample(double t, bool with_gate = false);
      void sample(const Interval& t0_tf, double dt, bool with_gates = false);
      friend std::ostream& operator<<(std::ostream& os, const TDomain& x);
      const std::list<TSlice>& tslices() const;
      std::list<TSlice>& tslices();
      void delete_gates();
      static bool are_same(const std::shared_ptr<TDomain>& tdom1, const std::shared_ptr<TDomain>& tdom2);


    protected:
      
      std::list<TSlice> _tslices;

      template<typename U>
      friend class Tube;
  };

  std::shared_ptr<TDomain> create_tdomain(const Interval& t0_tf = Interval(-oo,oo));
  std::shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, double dt, bool with_gates = false);

} // namespace codac

#endif