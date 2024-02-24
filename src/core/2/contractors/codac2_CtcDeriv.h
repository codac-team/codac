/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCDERIV_H__
#define __CODAC2_CTCDERIV_H__

#include "codac2_Ctc.h"
#include "codac2_Interval.h"
#include "codac_predef_contractors.h"

namespace codac2
{
  class CtcDeriv : public Ctc
  {
    public:

      CtcDeriv()
      { }

      void contract(Slice<Interval>& x, const Slice<Interval>& v)
      {
        if(x.is_gate()) // the slice may be on a degenerated temporal domain, i.e. a gate
          return;

        codac::Slice x_codac1(x.t0_tf(), x.codomain());
        x_codac1.set_input_gate(x.input_gate());
        x_codac1.set_output_gate(x.output_gate());

        codac::Slice v_codac1(v.t0_tf(), v.codomain());
        v_codac1.set_input_gate(v.input_gate());
        v_codac1.set_output_gate(v.output_gate());

        codac::ctc::deriv.contract(x_codac1, v_codac1);
        x.set(x_codac1.codomain());

        if(x.prev_slice_ptr() && x.prev_slice_ptr()->is_gate())
          x.prev_slice_ptr()->set(x_codac1.input_gate());
        if(x.next_slice_ptr() && x.next_slice_ptr()->is_gate())
          x.next_slice_ptr()->set(x_codac1.output_gate());
      }

      make_available_to_tubes();

      void contract(Tube<Interval>& x, Tube<Interval>& v, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD)
      {
        contract_fwdbwd(t_propa, x, v);
      }

      make_available_to_cn();
  };
}

#endif