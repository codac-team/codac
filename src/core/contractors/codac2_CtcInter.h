/** 
 *  \file
 *  CtcInter class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCINTER__
#define __CODAC2_CTCINTER__

#include "codac2_CtcWrapper.h"
#include "codac2_CollectionCtc.h"

namespace codac2
{
  class CtcInter : public CollectionCtc, public Ctc_<IntervalVector>
  {
    public:

      template<typename... C>
      CtcInter(const C&... c) : CollectionCtc(c...)
      { }

      void contract(IntervalVector& x) const
      {
        for(auto& ci : _v_ctc_ptrs)
          ci->contract(x);
      }

      template<typename C, // C should be some Ctc class
        typename = typename std::enable_if<std::is_base_of<Ctc,C>::value>::type>
      CtcInter& operator&=(const C& c)
      {
        add_shared_ptr(std::make_shared<C>(c));
        return *this;
      }
  };

  template<typename C2, // C2 should be some Ctc class
    typename = typename std::enable_if<std::is_base_of<Ctc,C2>::value>::type>
  inline CtcInter operator&(const CtcInter& c1, const C2& c2)
  {
    return CtcInter(c1,c2);
  }
}

#endif