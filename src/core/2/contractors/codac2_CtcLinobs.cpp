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

  /*void CtcLinobs::contract(Tube& x1, Tube& x2, const Tube& u, TimePropag t_propa)
  {
    vector<double> v_t;
    vector<IntervalVector> v_y;
    vector<ConvexPolygon> v_p_k;
    contract(v_t, v_y, x1, x2, u, v_p_k, t_propa);
  }

  void CtcLinobs::contract(TubeVector& x, const Tube& u, TimePropag t_propa)
  {
    vector<ConvexPolygon> v_p_k;
    contract(x, u, v_p_k, t_propa);
  }

  void CtcLinobs::contract(TubeVector& x, const Tube& u, vector<ConvexPolygon>& v_p_k, TimePropag t_propa)
  {
    vector<double> v_t;
    vector<IntervalVector> v_y;
    contract(v_t, v_y, x, u, v_p_k, t_propa);
  }

  void CtcLinobs::contract(double& t, IntervalVector& y, TubeVector& x, const Tube& u, TimePropag t_propa)
  {
    vector<ConvexPolygon> v_p_k;
    contract(t, y, x, u, v_p_k, t_propa);
  }

  void CtcLinobs::contract(double& t, IntervalVector& y, TubeVector& x, const Tube& u, vector<ConvexPolygon>& v_p_k, TimePropag t_propa)
  {
    vector<double> v_t(1, t);
    vector<IntervalVector> v_y(1, y);

    contract(v_t, v_y, x, u, v_p_k, t_propa);

    y &= v_y[0];
  }

  void CtcLinobs::contract(vector<double>& v_t, vector<IntervalVector>& v_y, TubeVector& x, const Tube& u, TimePropag t_propa)
  {
    vector<ConvexPolygon> v_p_k;
    contract(v_t, v_y, x, u, v_p_k, t_propa);
  }

  void CtcLinobs::contract(vector<double>& v_t, vector<IntervalVector>& v_y, TubeVector& x, const Tube& u, vector<ConvexPolygon>& v_p_k, TimePropag t_propa)
  {
    assert(x.size() == 2 && "operation not supported for other dimensions");
    contract(v_t, v_y, x[0], x[1], u, v_p_k, t_propa);
  }
*/

  void CtcLinobs::contract(Slice<ConvexPolygon>& x, const Slice<Interval>& u, TimePropag t_propa, bool compute_envelope)
  {
    if(x.is_gate())
      return;

    if((t_propa & TimePropag::FORWARD) && x.next_slice_ptr())
    {
      ConvexPolygon output_gate = x.output_gate();
      ctc_fwd_gate(output_gate, x.input_gate(), x.t0_tf().diam(), _A, _b, u.codomain());
      x.next_slice_ptr()->set(output_gate);
    }

    if((t_propa & TimePropag::BACKWARD) && x.prev_slice_ptr())
    {
      ConvexPolygon input_gate = x.input_gate();
      ctc_bwd_gate(input_gate, x.output_gate(), x.t0_tf().diam(), _A, _b, u.codomain());
      x.prev_slice_ptr()->set(input_gate);
    }

    if(compute_envelope)
      x.set(polygon_envelope(x.input_gate(), x.t0_tf().diam(), _A, _b, u.codomain()));
  }

  /*void CtcLinobs::contract(vector<double>& v_t, vector<IntervalVector>& v_y, Tube& x1, Tube& x2, const Tube& u, vector<ConvexPolygon>& v_p_k, TimePropag t_propa)
  {
    assert(Tube::same_slicing(x1, u));
    assert(Tube::same_slicing(x2, u));
    assert(v_t.size() == v_y.size());
    #ifdef DEBUG
    for(const auto& t : v_t)
      assert(x1.tdomain().contains(t));
    for(const auto& y : v_y)
      assert(y.size() == 2);
    #endif

    int k = x1.nb_slices();

    // Unbounded polygons are not supported yet, so we limit their size
    // (and so the envelope of the tube)
    
      IntervalVector box_domain(2, Interval(-9999.,9999.));
      x1 &= box_domain[0];
      x2 &= box_domain[1];

    // Creating a vector of (k+1) polygons

      v_p_k.clear();
      v_p_k = vector<ConvexPolygon>(k+1);

      int i = 0;
      const Slice *su;
      Slice *s1 = x1.first_slice(), *s2 = x2.first_slice();

      v_p_k[i] = ConvexPolygon({s1->input_gate(), s2->input_gate()});
      while(s1)
      {
        i++;
        v_p_k[i] = ConvexPolygon({s1->output_gate(), s2->output_gate()});
        s1 = s1->next_slice(); s2 = s2->next_slice();
      }
      assert(i == k);

    // Forward contractions

      if(t_propa & TimePropag::FORWARD)
      {
        i = 1;
        s1 = x1.first_slice();
        s2 = x2.first_slice();
        su = u.first_slice();

        while(s1)
        {
          const Interval tkm1_tk = s1->tdomain(); // [t_{k-1},t_k]

          if(tkm1_tk.intersects(m_restricted_tdomain))
          {
            ctc_fwd_gate(v_p_k[i], v_p_k[i-1], tkm1_tk.diam(), *_A, *_b, su->codomain());

            for(size_t j = 0 ; j < v_t.size() ; j++) // observations at uncertain times
              if(tkm1_tk.contains(v_t[j]))
                ctc_fwd_gate(v_p_k[i], ConvexPolygon(v_y[j]), tkm1_tk.ub()-v_t[j], *_A, *_b, su->codomain());
            // todo: contraction of the observations

            IntervalVector ouputgate_box = v_p_k[i].box();
            s1->set_output_gate(ouputgate_box[0]);
            s2->set_output_gate(ouputgate_box[1]);

            if(t_propa & TimePropag::BACKWARD)
            {
              // The slice envelope can be computed only from the gates,
              // and so it will be computed during the backward process.
            }

            else
            {
              IntervalVector envelope_box = polygon_envelope(v_p_k[i-1], tkm1_tk.diam(), *_A, *_b, su->codomain()).box();
              s1->set_envelope(envelope_box[0]);
              s2->set_envelope(envelope_box[1]);
            }
          }

          s1 = s1->next_slice(); s2 = s2->next_slice(); su = su->next_slice();
          i++;
        }
      }

    // Backward contractions

      if(t_propa & TimePropag::BACKWARD)
      {
        i = k-1;
        s1 = x1.last_slice();
        s2 = x2.last_slice();
        su = u.last_slice();

        while(s1)
        {
          const Interval tk_kp1 = s1->tdomain(); // [t_k,t_{k+1}]

          if(tk_kp1.intersects(m_restricted_tdomain))
          {
            ctc_bwd_gate(v_p_k[i], v_p_k[i+1], tk_kp1.diam(), *_A, *_b, su->codomain());

            for(size_t j = 0 ; j < v_t.size() ; j++) // observations at uncertain times
              if(tk_kp1.contains(v_t[j]))
                ctc_bwd_gate(v_p_k[i], ConvexPolygon(v_y[j]), v_t[j]-tk_kp1.lb(), *_A, *_b, su->codomain());
            // todo: contraction of the observations

            IntervalVector polybox = v_p_k[i].box();
            s1->set_input_gate(polybox[0]);
            s2->set_input_gate(polybox[1]);

            IntervalVector envelope_box = polygon_envelope(v_p_k[i], tk_kp1.diam(), *_A, *_b, su->codomain()).box();
            s1->set_envelope(envelope_box[0]);
            s2->set_envelope(envelope_box[1]);
          }

          s1 = s1->prev_slice(); s2 = s2->prev_slice(); su = su->prev_slice();
          i--;
        }
      }
  }*/

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