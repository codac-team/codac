/** 
 *  \file
 *  CtcLinobs class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCLINOBS_H__
#define __CODAC_CTCLINOBS_H__

#include <map>
#include <vector>
#include <functional>
#include "codac_DynCtc.h"
#include "codac_ConvexPolygon.h"
#include "codac_IntervalMatrix.h"

namespace codac
{
  /**
   * \class CtcLinobs
   */
  class CtcLinobs : public DynCtc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_\textrm{linobs}\f$
       */
      CtcLinobs(const Matrix& A, const Vector& b); // /!\ auto evaluation of e^At not reliable
      ~CtcLinobs();

      void contract(std::vector<Domain*>& v_domains);
      void contract(Tube& x1, Tube& x2, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(TubeVector& x, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(TubeVector& x, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      // todo: contraction of the observations
      void contract(double& t, IntervalVector& y, TubeVector& x, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(double& t, IntervalVector& y, TubeVector& x, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      // todo: contraction of the observations
      void contract(std::vector<double>& v_t, std::vector<IntervalVector>& v_y, TubeVector& x, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(std::vector<double>& v_t, std::vector<IntervalVector>& v_y, TubeVector& x, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(std::vector<double>& v_t, std::vector<IntervalVector>& v_y, Tube& x1, Tube& x2, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      ConvexPolygon polygon_envelope(const ConvexPolygon& p_k, double dt_k_kp1, const Matrix& A, const Vector& b, const Interval& u_k);


    protected:

      void ctc_fwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_km1, double dt_km1_k, const Matrix& A, const Vector& b, const Interval& u_km1);
      void ctc_bwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_kp1, double dt_k_kp1, const Matrix& A, const Vector& b, const Interval& u_k);


    protected:

      const Matrix* m_A;
      const Vector* m_b;

      const int m_polygon_max_edges = 15;

      static const std::string m_ctc_name; //!< class name (mainly used for CN Exceptions)
      static std::vector<std::string> m_str_expected_doms; //!< allowed domains signatures (mainly used for CN Exceptions)
      friend class ContractorNetwork;
  };
}

#endif