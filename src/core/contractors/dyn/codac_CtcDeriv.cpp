/** 
 *  CtcDeriv class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcDeriv.h"
#include "codac_ConvexPolygon.h"
#include "codac_Domain.h"
#include "codac_DomainsTypeException.h"

using namespace std;
using namespace ibex;

namespace codac
{
  CtcDeriv::CtcDeriv()
    : DynCtc(false)
  {

  }

  // Static members for contractor signature (mainly used for CN Exceptions)
  const string CtcDeriv::m_ctc_name = "CtcDeriv";
  vector<string> CtcDeriv::m_str_expected_doms(
  {
    "Tube, Tube",
    "TubeVector, TubeVector",
    "Slice, Slice[, Slice, Slice..]"
  });
  
  void CtcDeriv::contract(vector<Domain*>& v_domains)
  {
    // Tube scalar case:
    if(v_domains[0]->type() == Domain::Type::T_TUBE && v_domains[1]->type() == Domain::Type::T_TUBE)
    {
      if(v_domains.size() != 2)
        throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);

      contract(v_domains[0]->tube(), v_domains[1]->tube());
    }

    // Tube vector case:
    else if(v_domains[0]->type() == Domain::Type::T_TUBE_VECTOR && v_domains[1]->type() == Domain::Type::T_TUBE_VECTOR)
    {
      if(v_domains.size() != 2)
        throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
      
      contract(v_domains[0]->tube_vector(), v_domains[1]->tube_vector());
    }

    // Slice case:
    else if(v_domains[0]->type() == Domain::Type::T_SLICE)
    {
      if(v_domains.size() % 2 != 0)
        throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
      
      for(int i = 0 ; i < floor(v_domains.size()/2) ; i++)
      {
        if(v_domains[i]->type() != Domain::Type::T_SLICE 
          || v_domains[i+v_domains.size()/2]->type() != Domain::Type::T_SLICE)
          throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
      
        contract(v_domains[i]->slice(), v_domains[i+v_domains.size()/2]->slice());
      }
    }

    else
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);
  }

  void CtcDeriv::contract(Tube& x, const Tube& v, TimePropag t_propa)
  {
    assert(x.tdomain() == v.tdomain());
    assert(Tube::same_slicing(x, v));
    
    if(t_propa & TimePropag::FORWARD)
    {
      Slice *s_x = x.first_slice();
      const Slice *s_v = v.first_slice();

      while(s_x)
      {
        assert(s_v);
        contract(*s_x, *s_v, t_propa);
        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }
    }
    
    if(t_propa & TimePropag::BACKWARD)
    {
      Slice *s_x = x.last_slice();
      const Slice *s_v = v.last_slice();

      while(s_x)
      {
        assert(s_v);
        contract(*s_x, *s_v, t_propa);
        s_x = s_x->prev_slice();
        s_v = s_v->prev_slice();
      }
    }
  }

  void CtcDeriv::contract(TubeVector& x, const TubeVector& v, TimePropag t_propa)
  {
    assert(x.size() == v.size());
    assert(x.tdomain() == v.tdomain());
    assert(TubeVector::same_slicing(x, v));

    for(int i = 0 ; i < x.size() ; i++)
      contract(x[i], v[i], t_propa);
  }
  
  void CtcDeriv::contract(Slice& x, const Slice& v, TimePropag t_propa)
  {
    assert(x.tdomain() == v.tdomain());
    #ifndef NDEBUG
      double volume = x.volume() + v.volume(); // for last assert
      // todo: remove this: (or use Polygons with truncation)
      if(x.codomain().ub() == BOUNDED_INFINITY || x.codomain().lb() == -BOUNDED_INFINITY)
        volume = std::numeric_limits<double>::infinity();
    #endif

    if(!x.tdomain().intersects(m_restricted_tdomain))
    {
      // todo: Thin contraction with respect to tube's slicing:
      // the contraction should not be optimal on purpose if the
      // restricted tdomain does not cover the slice's domain
      return;
    }

    // todo: the fwd/bwd way of propag. is lost in this procedure
    // Restore this feature? Use it in CN only?

    Interval ingate = x.input_gate();
    Interval outgate = x.output_gate();
    Interval envelope = x.codomain();

    // Gates contraction
    ingate &= x.output_gate() - x.tdomain().diam() * v.codomain();
    outgate &= x.input_gate() + x.tdomain().diam() * v.codomain();

    if(outgate.is_superset(x.output_gate()) || ingate.is_superset(x.input_gate()))
    {
      // Optimal computation

      // todo: remove this: (or use Polygons with truncation)
      envelope &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY);

      x.set_envelope(envelope);

      // Gates needed for polygon computation
      x.set_input_gate(ingate);
      x.set_output_gate(outgate);

      // Optimal envelope
      envelope &= x.polygon(v).box()[1];

      // todo: remove this: (or use Polygons with truncation)
      if(envelope.ub() == BOUNDED_INFINITY) envelope = Interval(envelope.lb(),POS_INFINITY);
      if(envelope.lb() == -BOUNDED_INFINITY) envelope = Interval(NEG_INFINITY,envelope.ub());
      if(ingate.ub() == BOUNDED_INFINITY) ingate = Interval(ingate.lb(),POS_INFINITY);
      if(ingate.lb() == -BOUNDED_INFINITY) ingate = Interval(NEG_INFINITY,ingate.ub());
      if(outgate.ub() == BOUNDED_INFINITY) outgate = Interval(outgate.lb(),POS_INFINITY);
      if(outgate.lb() == -BOUNDED_INFINITY) outgate = Interval(NEG_INFINITY,outgate.ub());
    }

    else
    {
      envelope &= ingate + Interval(0.,x.tdomain().diam()) * v.codomain();
      envelope &= outgate - Interval(0.,x.tdomain().diam()) * v.codomain();
    }

    x.set_envelope(envelope);
    x.set_input_gate(ingate);
    x.set_output_gate(outgate);

    assert(volume >= x.volume() + v.volume() && "contraction rule not respected");
  }

  void CtcDeriv::contract_gates(Slice& x, const Slice& v)
  {
    assert(x.tdomain() == v.tdomain());
    
    Interval in_gate = x.input_gate(), out_gate = x.output_gate();

    Interval in_gate_proj = in_gate + x.tdomain().diam() * v.codomain();
    out_gate &= in_gate_proj;
    x.set_output_gate(out_gate);

    Interval out_gate_proj = out_gate - x.tdomain().diam() * v.codomain();
    in_gate &= out_gate_proj;
    x.set_input_gate(in_gate);
  }
}