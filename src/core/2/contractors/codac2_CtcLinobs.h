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

#ifndef __CODAC2_CTCLINOBS_H__
#define __CODAC2_CTCLINOBS_H__

#include <map>
#include <vector>
#include <functional>
#include "codac_DynCtc.h"
#include "codac2_Tube.h"
#include "codac_ConvexPolygon.h"
#include "codac_IntervalMatrix.h"

namespace codac2
{
  /**
   * \class CtcLinobs
   */
  class CtcLinobs : public codac::DynCtc
  {
    public:

      CtcLinobs(const codac::Matrix& A, const codac::Vector& b); // /!\ auto evaluation of e^At not reliable
      ~CtcLinobs();

      // to be removed:
      void contract(codac::TubeVector& x, const codac::Tube& u, codac::TimePropag t_propa);

      void contract(std::vector<codac::Domain*>& v_domains);
      void contract(Tube<codac::ConvexPolygon>& x, const Tube<Interval>& u, codac::TimePropag t_propa = codac::TimePropag::FORWARD | codac::TimePropag::BACKWARD, bool compute_envelopes = true);
      void contract(Slice<codac::ConvexPolygon>& x, const Slice<codac::Interval>& u, codac::TimePropag t_propa = codac::TimePropag::FORWARD | codac::TimePropag::BACKWARD, bool compute_envelope = true);
  

    protected:

      void ctc_fwd_gate(codac::ConvexPolygon& p_k, const codac::ConvexPolygon& p_km1, double dt_km1_k, const codac::Interval& u_km1);
      void ctc_bwd_gate(codac::ConvexPolygon& p_k, const codac::ConvexPolygon& p_kp1, double dt_k_kp1, const codac::Interval& u_k);
      codac::ConvexPolygon polygon_envelope(const codac::ConvexPolygon& p_k, double dt_k_kp1, const codac::Interval& u_k);


    protected:

      const codac::Matrix _A;
      const codac::Vector _b;

      const int m_polygon_max_edges = 15;

      static const std::string m_ctc_name; //!< class name (mainly used for CN Exceptions)
      static std::vector<std::string> m_str_expected_doms; //!< allowed domains signatures (mainly used for CN Exceptions)
      friend class ContractorNetwork;
  };
}

#endif