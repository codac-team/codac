/** 
 *  CtcLinobs class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcLinobs.h"
#include "codac_Domain.h"
#include "codac_polygon_arithmetic.h"
#include "codac_DomainsTypeException.h"
#include <unsupported/Eigen/MatrixFunctions> // for computing e^At

using namespace std;
using namespace ibex;
using namespace codac;

namespace codac2
{
  IntervalMatrix exp_At(const Matrix& A, const Interval& t) // computes e^At
  {
    assert(A.nb_rows() == 2 && A.nb_cols() == 2);
    IntervalMatrix A_exp(2, 2, Interval::EMPTY_SET);
    vector<double> v_t({t.lb(), t.ub()});

    for(const auto& t : v_t)
    {
      Eigen::MatrixXd eigen_A(2,2);
      eigen_A << A[0][0], A[0][1],
                 A[1][0], A[1][1];
      eigen_A = eigen_A * t;
      Eigen::MatrixXd eigen_A_exp = eigen_A.exp();

      A_exp[0][0] |= eigen_A_exp(0,0); A_exp[0][1] |= eigen_A_exp(0,1);
      A_exp[1][0] |= eigen_A_exp(1,0); A_exp[1][1] |= eigen_A_exp(1,1);
    }
    
    return A_exp;
  }

  CtcLinobs::CtcLinobs(const Matrix& A, const Vector& b)
    : DynCtc(), _A(A), _b(b)
  {
    assert(A.nb_cols() == 2 && A.nb_rows() == 2);
    assert(b.size() == 2);
  }

  CtcLinobs::~CtcLinobs()
  {

  }

  // Static members for contractor signature (mainly used for CN Exceptions)
  const string CtcLinobs::m_ctc_name = "CtcLinobs";
  vector<string> CtcLinobs::m_str_expected_doms(
  {
    "TubeVector, Tube"
  });

  void CtcLinobs::contract(vector<codac::Domain*>& v_domains)
  {
    /*if(v_domains.size() != 2
      || v_domains[0]->type() != Domain::Type::T_TUBE_VECTOR
      || v_domains[1]->type() != Domain::Type::T_TUBE)
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);

    contract(v_domains[0]->tube_vector(), v_domains[1]->tube());*/
  }

  void CtcLinobs::contract(Tube<ConvexPolygon>& x, const Tube<Interval>& u, TimePropag t_propa, bool compute_envelopes)
  {
    assert(x.tdomain() == u.tdomain());

    if(t_propa & TimePropag::FORWARD)
      for(auto it = x.begin(); it != x.end(); ++it)
      {
        if((*it).is_gate()) continue;
        const shared_ptr<Slice<Interval>> su = static_pointer_cast<Slice<Interval>>((*it).tslice().slices().at(&u));
        contract(*it, *su, TimePropag::FORWARD, compute_envelopes && !(t_propa & TimePropag::BACKWARD));
        // Envelopes are contracted in the bwd iteration if selected
      }

    if(t_propa & TimePropag::BACKWARD)
      for(auto it = x.rbegin(); it != x.rend(); ++it)
      {
        if((*it).is_gate()) continue;
        const shared_ptr<Slice<Interval>> su = static_pointer_cast<Slice<Interval>>((*it).tslice().slices().at(&u));
        contract(*it, *su, TimePropag::BACKWARD, compute_envelopes);
      }
  }

  void CtcLinobs::contract(Slice<ConvexPolygon>& x, const Slice<Interval>& u, TimePropag t_propa, bool compute_envelope)
  {
    if(x.is_gate())
      return;

    if((t_propa & TimePropag::FORWARD) && x.next_slice_ptr())
    {
      ConvexPolygon output_gate = x.output_gate();
      ctc_fwd_gate(output_gate, x.input_gate(), x.t0_tf().diam(), u.codomain());
      x.next_slice_ptr()->set(output_gate);
    }

    if((t_propa & TimePropag::BACKWARD) && x.prev_slice_ptr())
    {
      ConvexPolygon input_gate = x.input_gate();
      ctc_bwd_gate(input_gate, x.output_gate(), x.t0_tf().diam(), u.codomain());
      x.prev_slice_ptr()->set(input_gate);
    }

    if(compute_envelope)
      x.set(polygon_envelope(x.input_gate(), x.t0_tf().diam(), u.codomain()));
  }
  
  void CtcLinobs::ctc_fwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_km1,
    double dt_km1_k, const Interval& u_km1)
  {
    p_k = p_k & (exp_At(_A,dt_km1_k)*p_km1 + dt_km1_k*exp_At(_A,Interval(0.,dt_km1_k))*(u_km1*_b));
    p_k.simplify(m_polygon_max_edges);
  }

  void CtcLinobs::ctc_bwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_kp1,
    double dt_k_kp1, const Interval& u_k)
  {
    p_k = p_k & (exp_At(-_A,dt_k_kp1)*p_kp1 - dt_k_kp1*exp_At(-_A,Interval(0.,dt_k_kp1))*(u_k*_b));
    p_k.simplify(m_polygon_max_edges);
  }

  ConvexPolygon CtcLinobs::polygon_envelope(const ConvexPolygon& p_k,
    double dt_k_kp1, const Interval& u_k)
  {
    return exp_At(_A,Interval(0.,dt_k_kp1))*p_k + Interval(0.,dt_k_kp1)*exp_At(_A,Interval(0.,dt_k_kp1))*(u_k*_b);
  }
}