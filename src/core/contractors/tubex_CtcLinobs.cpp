/** 
 *  CtcLinobs class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcLinobs.h"
#include "tubex_Domain.h"
#include "tubex_polygon_arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcLinobs::CtcLinobs(const Matrix& A, const Vector& b, IntervalMatrix (*exp_At)(const Matrix& A, const Interval& t))
    : Ctc(), m_A(A), m_b(b), m_exp_At(exp_At)
  {
    assert(A.nb_cols() == 2 && A.nb_rows() == 2);
    assert(b.size() == 2);

  }

  void CtcLinobs::contract(vector<Domain*>& v_domains)
  {
    assert(v_domains.size() == 2);
    assert(v_domains[0]->type() == Domain::Type::TUBE_VECTOR && v_domains[0]->type() == Domain::Type::TUBE);

    contract(v_domains[0]->tube_vector(), v_domains[1]->tube());
  }

  void CtcLinobs::contract(TubeVector& x, const Tube& u, TimePropag t_propa)
  {
    assert(Tube::same_slicing(x[0], u));
    assert(Tube::same_slicing(x[1], u));

    int k = x[0].nb_slices();

    // Creating a vector of (k+1) polygons

      vector<ConvexPolygon> v_p_k(k+1);

      int l = 0;
      for(const Slice* s = x[0].first_slice() ; s != NULL ; s = s->next_slice())
      {
        v_p_k[l] = ConvexPolygon(x(s->domain().lb()));
        l++;
      }
      v_p_k[l] = ConvexPolygon(x(x.domain().ub()));
      assert(l == k);

    // Forward contractions

      int i;
      Slice *s0, *s1;
      const Slice *su;

      if(t_propa & TimePropag::FORWARD)
      {
        i = 1;
        s0 = x[0].first_slice();
        s1 = x[1].first_slice();
        su = u.first_slice();

        while(s0 != NULL)
        {
          const Interval tkm1_tk = s0->domain(); // [t_{k-1},t_k]
          ctc_fwd_gate(v_p_k[i], v_p_k[i-1], tkm1_tk.diam(), m_A, m_b, su->codomain());

          IntervalVector ouputgate_box = v_p_k[i].box();
          s0->set_output_gate(ouputgate_box[0]);
          s1->set_output_gate(ouputgate_box[1]);

          if(t_propa & TimePropag::BACKWARD)
          {
            // The slice envelope can be computed only from the gates,
            // and so it will be computed during the backward process.
          }

          else
          {
            IntervalVector envelope_box = polygon_envelope(v_p_k[i-1], tkm1_tk.diam(), m_A, m_b, su->codomain()).box();
            s0->set_envelope(envelope_box[0]);
            s1->set_envelope(envelope_box[1]);
          }

          //for(const auto &[t, box] : m_obs) // observations at uncertain times
          //  if(tkm1_tk.contains(t))
          //    ctc_fwd(v_p_k[i], ConvexPolygon(box), tkm1_tk.ub()-t, A, b, u(i-1));

          s0 = s0->next_slice(); s1 = s1->next_slice(); su = su->next_slice();
          i++;
        }
      }

    // Backward contractions

      if(t_propa & TimePropag::BACKWARD)
      {
        i = k-1;
        s0 = x[0].last_slice();
        s1 = x[1].last_slice();
        su = u.last_slice();

        while(s0 != NULL)
        {
          const Interval tk_kp1 = s0->domain(); // [t_k,t_{k+1}]
          ctc_bwd_gate(v_p_k[i], v_p_k[i+1], tk_kp1.diam(), m_A, m_b, su->codomain());

          IntervalVector polybox = v_p_k[i].box();
          s0->set_input_gate(polybox[0]);
          s1->set_input_gate(polybox[1]);

          IntervalVector envelope_box = polygon_envelope(v_p_k[i], tk_kp1.diam(), m_A, m_b, su->codomain()).box();
          s0->set_envelope(envelope_box[0]);
          s1->set_envelope(envelope_box[1]);

          //for(const auto &[t, box] : m_obs) // observations at uncertain times
          //  if(tk_kp1.contains(t))
          //    ctc_bwd(v_p_k[i], ConvexPolygon(box), t-tk_kp1.lb(), A, b, u(i));
          
          s0 = s0->prev_slice(); s1 = s1->prev_slice(); su = su->prev_slice();
          i--;
        }
      }
  }

  void CtcLinobs::ctc_fwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_km1,
    double dt_km1_k, const Matrix& A, const Vector& b, const Interval& u_km1)
  {
    p_k = p_k & (m_exp_At(A,dt_km1_k)*p_km1 + dt_km1_k*m_exp_At(A,Interval(0.,dt_km1_k))*(u_km1*b));
    p_k.simplify(m_polygon_max_edges);
  }

  void CtcLinobs::ctc_bwd_gate(ConvexPolygon& p_k, const ConvexPolygon& p_kp1,
    double dt_k_kp1, const Matrix& A, const Vector& b, const Interval& u_k)
  {
    p_k = p_k & (m_exp_At(-A,dt_k_kp1)*p_kp1 - dt_k_kp1*m_exp_At(-A,Interval(0.,dt_k_kp1))*(u_k*b));
    p_k.simplify(m_polygon_max_edges);
  }

  ConvexPolygon CtcLinobs::polygon_envelope(const ConvexPolygon& p_k,
    double dt_k_kp1, const Matrix& A, const Vector& b, const Interval& u_k)
  {
    return m_exp_At(A,Interval(0.,dt_k_kp1))*p_k + Interval(0.,dt_k_kp1)*m_exp_At(A,Interval(0.,dt_k_kp1))*(u_k*b);
  }
}