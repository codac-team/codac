/** 
 *  \file
 *  CtcUnion class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCUNION_H__
#define __CODAC_CTCUNION_H__

#include <memory>
//#include "ibex_CtcUnion.h"
#include "codac_Ctc.h"

namespace codac
{
  class CtcUnion : public Ctc
  {
    public:

      CtcUnion(int nb_var) : Ctc(nb_var)
      { }

      template<typename C1>
      CtcUnion(const C1& c1) : Ctc(c1.nb_var)
      {
        _v_ctc.push_back(std::make_shared<C1>(c1));
      }

      template<typename C1,typename... C>
      CtcUnion(const C1& c1, const C&... c) : CtcUnion(c1)
      {
        (_v_ctc.push_back(std::make_shared<C>(c)), ...);
        for(const auto& ci : _v_ctc) { assert(ci->nb_var == nb_var); }
      }

      void contract(IntervalVector& x)
      {
        IntervalVector result(nb_var, Interval::empty_set());

        for(auto& ci : _v_ctc)
        {
          IntervalVector saved_x = x;
          ci->contract(saved_x);
          result |= saved_x;
        }

        for(auto& ci : _v_ctc_ptrs)
        {
          IntervalVector saved_x = x;
          ci->contract(saved_x);
          result |= saved_x;
        }

        x = result;
      }

      template<typename C>
      CtcUnion& operator|=(const C& c)
      {
        assert(c.nb_var == nb_var);
        _v_ctc.push_back(std::make_shared<C>(c));
        return *this;
      }

      CtcUnion& add_raw_ptr(Ctc *c)
      {
        _v_ctc_ptrs.push_back(c);
        return *this;
      }

    protected:

      std::vector<std::shared_ptr<Ctc>> _v_ctc;
      std::vector<Ctc*> _v_ctc_ptrs;
  };
}

#endif