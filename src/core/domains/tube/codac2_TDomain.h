/** 
 *  \file codac2_TDomain.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include <memory>
#include "codac2_TSlice.h"

namespace codac2
{
  class TDomain : public std::list<TSlice>
  {
    protected:

      explicit TDomain(const Interval& t0_tf);
      explicit TDomain(const Interval& t0_tf, double dt, bool with_gates = false);

    public:

      Interval t0_tf() const;
      size_t nb_tslices() const;
      size_t nb_tubes() const;
      bool all_gates_defined() const;
      std::vector<TSlice> tslices_vector() const;

      std::list<TSlice>::iterator tslice(double t); // returns it on last slice if t==t_f, not end
      std::list<TSlice>::iterator sample(double t, bool with_gate = false);
      void sample(const Interval& t0_tf, double dt, bool with_gates = false);
      void delete_gates();

      friend std::ostream& operator<<(std::ostream& os, const TDomain& x);
      static bool are_same(const std::shared_ptr<const TDomain>& tdom1, const std::shared_ptr<const TDomain>& tdom2);

    protected:
      
      friend std::shared_ptr<TDomain> create_tdomain(const Interval&);
      friend std::shared_ptr<TDomain> create_tdomain(const Interval&,double,bool);
  };

  std::shared_ptr<TDomain> create_tdomain(const Interval& t0_tf = Interval(-oo,oo));
  std::shared_ptr<TDomain> create_tdomain(const Interval& t0_tf, double dt, bool with_gates = false);
}