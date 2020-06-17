/** 
 *  \file
 *  CtcLinobs class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCLINOBS_H__
#define __TUBEX_CTCLINOBS_H__

#include <map>
#include <vector>
#include <functional>
#include "tubex_DynCtc.h"
#include "tubex_ConvexPolygon.h"

namespace tubex
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
      // CtcLinobs(const ibex::Matrix& A, const ibex::Vector& b); // not yet available since auto evaluation of e^At not at hand
      CtcLinobs(const ibex::Matrix& A, const ibex::Vector& b, ibex::IntervalMatrix (*exp_At)(const ibex::Matrix& A, const ibex::Interval& t));

      void contract(std::vector<Domain*>& v_domains);
      void contract(TubeVector& x, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(TubeVector& x, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      // todo: contraction of the observations
      void contract(double& t, ibex::IntervalVector& y, TubeVector& x, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(double& t, ibex::IntervalVector& y, TubeVector& x, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);

      // todo: contraction of the observations
      void contract(std::vector<double>& v_t, std::vector<ibex::IntervalVector>& v_y, TubeVector& x, const Tube& u, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);
      void contract(std::vector<double>& v_t, std::vector<ibex::IntervalVector>& v_y, TubeVector& x, const Tube& u, std::vector<ConvexPolygon>& v_p_k, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);


    protected:

      void ctc_fwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_km1, double dt_km1_k, const ibex::Matrix& A, const ibex::Vector& b, const ibex::Interval& u_km1);
      void ctc_bwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_kp1, double dt_k_kp1, const ibex::Matrix& A, const ibex::Vector& b, const ibex::Interval& u_k);

      ConvexPolygon polygon_envelope(const ConvexPolygon& p_k, double dt_k_kp1, const ibex::Matrix& A, const ibex::Vector& b, const ibex::Interval& u_k);

    protected:

      const ibex::Matrix& m_A;
      const ibex::Vector& m_b;
      ibex::IntervalMatrix (*m_exp_At)(const ibex::Matrix& A, const ibex::Interval& t);

      const int m_polygon_max_edges = 15;
  };
}

#endif