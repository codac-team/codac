/* ============================================================================
 *  tubex-lib - Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_TrajectorySerialization.h"
#include "tubex_DimensionException.h"
#include "tubex_SlicingException.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_Arithmetic.h"
#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Tube::Tube()
    {
      
    }

    Tube::Tube(const Interval& domain, const Interval& codomain)
    {
      DomainException::check(domain);

      // By default, the tube is defined as one single slice
      m_first_slice = new Slice(domain, codomain);
      m_first_slice->set_tube_ref(this);
    }
    
    Tube::Tube(const Interval& domain, double timestep, const Interval& codomain)
    {
      DomainException::check(domain);
      DomainException::check(timestep);

      Slice *prev_slice = NULL, *slice;
      double lb, ub = domain.lb();

      if(timestep == 0.)
        timestep = domain.diam();

      do
      {
        lb = ub; // we guarantee all slices are adjacent
        ub = min(lb + timestep, domain.ub());

        slice = new Slice(Interval(lb,ub));
        slice->set_tube_ref(this);

        if(prev_slice != NULL)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = NULL;
          Slice::chain_slices(prev_slice, slice);
        }

        prev_slice = slice;
        if(m_first_slice == NULL) m_first_slice = slice;
        slice = slice->next_slice();

      } while(ub < domain.ub());

      if(codomain != Interval::ALL_REALS)
        set(codomain);
    }
    
    Tube::Tube(const Interval& domain, double timestep, const tubex::Fnc& f, int f_image_id)
      : Tube(domain, timestep)
    {
      // todo: check scalar f
      // todo: check f_image_id
      DomainException::check(domain);
      DomainException::check(timestep);

      if(f.nb_vars() != 0)
        throw Exception("Tube constructor",
                        "function's inputs not limited to system variable");
      // A copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(*this); // 1: one variable (system variable t), todo: update this comment
      *this = f.eval_vector(input)[f_image_id];
      // todo: delete something here?
    }

    Tube::Tube(const Tube& x)
    {
      *this = x;
    }

    Tube::Tube(const Tube& x, const Interval& codomain)
      : Tube(x)
    {
      set(codomain);
    }

    Tube::Tube(const Trajectory& traj, double timestep)
      : Tube(traj.domain(), timestep)
    {
      DomainException::check(timestep);
      set_empty();
      *this |= traj;
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep)
      : Tube(lb.domain(), timestep)
    {
      DomainException::check(timestep);
      set_empty();
      *this |= lb;
      *this |= ub;
    }

    Tube::Tube(const string& binary_file_name)
    {
      vector<Trajectory> v_trajs;
      deserialize(binary_file_name, v_trajs);
    }
    
    Tube::Tube(const string& binary_file_name, Trajectory& traj)
    {
      vector<Trajectory> v_trajs;
      deserialize(binary_file_name, v_trajs);

      if(v_trajs.size() == 0)
        throw Exception("Tube constructor", "unable to deserialize a Trajectory");

      traj = v_trajs[0];
    }

    Tube::Tube(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {
      deserialize(binary_file_name, v_trajs);
      if(v_trajs.size() == 0)
        throw Exception("Tube constructor", "unable to deserialize some Trajectory");
    }
    
    Tube::~Tube()
    {
      Slice *slice = get_first_slice();
      while(slice != NULL)
      {
        Slice *next_slice = slice->next_slice();
        delete slice;
        slice = next_slice;
      }
    }

    int Tube::size() const
    {
      return 1;
    }

    const Tube Tube::primitive() const
    {
      Tube primitive(*this, Interval::ALL_REALS); // a copy of this initialized to [-oo,oo]
      primitive.set(0., primitive.domain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, static_cast<const Tube&>(*this), FORWARD);
      return primitive;
    }

    const Tube& Tube::operator=(const Tube& x)
    {
      { // Destroying already existing slices
        Slice *slice = get_first_slice();
        while(slice != NULL)
        {
          Slice *next_slice = slice->next_slice();
          delete slice;
          slice = next_slice;
        }
      }

      Slice *prev_slice = NULL, *slice = NULL;

      for(const Slice *s = x.get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(slice == NULL)
        {
          slice = new Slice(*s);
          m_first_slice = slice;
        }

        else
        {
          slice->m_next_slice = new Slice(*s);
          slice = slice->next_slice();
        }

        slice->set_tube_ref(this);

        if(prev_slice != NULL)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = NULL;
          Slice::chain_slices(prev_slice, slice);
        }

        prev_slice = slice;
      }

      return *this;
    }

    const Interval Tube::domain() const
    {
      return Interval(get_first_slice()->domain().lb(),
                      get_last_slice()->domain().ub());
    }
  
    // Slices structure

    int Tube::nb_slices() const
    {
      int size = 0;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        size ++;
      return size;
    }

    Slice* Tube::get_slice(int slice_id)
    {
      SlicingException::check(*this, slice_id);
      return const_cast<Slice*>(static_cast<const Tube&>(*this).get_slice(slice_id));
    }

    const Slice* Tube::get_slice(int slice_id) const
    {
      SlicingException::check(*this, slice_id);
      int i = 0;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(i == slice_id)
          return s;
        i++;
      }
      return NULL;
    }

    Slice* Tube::get_slice(double t)
    {
      DomainException::check(*this, t);
      return const_cast<Slice*>(static_cast<const Tube&>(*this).get_slice(t));
    }

    const Slice* Tube::get_slice(double t) const
    {
      DomainException::check(*this, t);
      return get_slice(input2index(t));
    }

    Slice* Tube::get_first_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).get_first_slice());
    }

    const Slice* Tube::get_first_slice() const
    {
      return m_first_slice;
    }

    Slice* Tube::get_last_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).get_last_slice());
    }

    const Slice* Tube::get_last_slice() const
    {
      for(const Slice *s = get_first_slice() ; true ; s = s->next_slice())
        if(s->next_slice() == NULL)
          return s;
      return NULL;
    }

    Slice* Tube::get_wider_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).get_wider_slice());
    }

    const Slice* Tube::get_wider_slice() const
    {
      double max_domain_width = 0.;
      const Slice *wider_slice;

      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        if(s->domain().diam() > max_domain_width)
        {
          wider_slice = s;
          max_domain_width = s->domain().diam();
        }

      return wider_slice;
    }

    Slice* Tube::get_largest_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).get_largest_slice());
    }

    const Slice* Tube::get_largest_slice() const
    {
      int i = 0;
      double max_diam = 0.;
      const Slice *largest = get_first_slice();
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(s->codomain().is_unbounded())
          return s;

        if(s->codomain().diam() > max_diam)
        {
          max_diam = s->codomain().diam();
          largest = s;
        }
      }

      return largest;
    }
    
    const Interval Tube::slice_domain(int slice_id) const
    {
      // todo: check slice_id
      return get_slice(slice_id)->domain();
    }

    int Tube::input2index(double t) const
    {
      DomainException::check(*this, t);

      int i = -1;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        i++;
        if(t < s->domain().ub())
          return i;
      }

      return i;
    }

    int Tube::index(const Slice* slice) const
    {
      int i = 0;
      const Slice *it = get_first_slice();
      while(it != NULL && it != slice)
      {
        it = it->next_slice();
        if(it == NULL) return -1;
        i++;
      }
      return i;
    }

    void Tube::sample(double t)
    {
      DomainException::check(*this, t);

      Slice *slice_to_be_sampled = get_slice(t);

      if(slice_to_be_sampled->domain().lb() == t || slice_to_be_sampled->domain().ub() == t)
      {
        // No degenerate slice,
        // the method has no effect.
        return;
      }

      Slice *next_slice = slice_to_be_sampled->next_slice();

      // Creating new slice
      Slice *new_slice = new Slice(*slice_to_be_sampled);
      new_slice->set_tube_ref(this);
      new_slice->set_domain(Interval(t, slice_to_be_sampled->domain().ub()));
      slice_to_be_sampled->set_domain(Interval(slice_to_be_sampled->domain().lb(), t));

      // todo: remove this? vector<Slice*>::iterator it = find(m_v_slices.begin(), m_v_slices.end(), slice_to_be_sampled);
      // todo: remove this? m_v_slices.insert(++it, new_slice);

      // Updated slices structure
      delete new_slice->m_input_gate;
      new_slice->m_input_gate = NULL;
      Slice::chain_slices(new_slice, next_slice);
      Slice::chain_slices(slice_to_be_sampled, new_slice);
      new_slice->set_input_gate(new_slice->codomain());
    }

    void Tube::sample(double t, const Interval& gate)
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, gate);
      sample(t);
      Slice *slice = get_slice(t);
      if(t == slice->domain().lb())
        slice->set_input_gate(gate);
      else
        slice->set_output_gate(gate);
    }
    
    bool Tube::share_same_slicing(const Tube& x1, const Tube& x2)
    {
      if(x1.nb_slices() != x2.nb_slices())
        return false;

      const Slice *s2 = x2.get_first_slice();
      for(const Slice *s1 = x1.get_first_slice() ; s1 != NULL ; s1 = s1->next_slice())
      {
        if(s1->domain() != s2->domain())
          return false;
        s2 = s2->next_slice();
      }

      return true;
    }

    // Access values

    const Interval Tube::codomain() const
    {
      return codomain_box()[0];
    }

    double Tube::volume() const
    {
      double volume = 0.;
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        volume += s->volume();
      return volume;
    }

    const Interval Tube::operator()(int slice_id) const
    {
      SlicingException::check(*this, slice_id);
      return get_slice(slice_id)->codomain();
    }

    const Interval Tube::operator()(double t) const
    {
      DomainException::check(*this, t);
      return get_slice(t)->operator()(t);
    }

    const Interval Tube::operator()(const Interval& t) const
    {
      DomainException::check(*this, t);

      if(t.is_degenerated())
        return operator()(t.lb());

      const Slice *first_slice = get_slice(t.lb());
      const Slice *last_slice = get_slice(t.ub());
      if(last_slice->domain().lb() != t.ub())
        last_slice = last_slice->next_slice();

      Interval codomain = Interval::EMPTY_SET;
      for(const Slice *s = first_slice ; s != last_slice ; s = s->next_slice())
        codomain |= s->codomain();

      return codomain;
    }

    const Interval Tube::invert(const Interval& y, const Interval& search_domain) const
    {
      Tube v(*this, Interval::ALL_REALS); // todo: optimize this
      return invert(y, v, search_domain);
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      Tube v(*this, Interval::ALL_REALS); // todo: optimize this
      v_t.clear();
      invert(y, v_t, v, search_domain);
    }

    const Interval Tube::invert(const Interval& y, const Tube& v, const Interval& search_domain) const
    {
      SlicingException::check(*this, v);
      DomainException::check(*this, v);

      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return Interval::EMPTY_SET;

      const Slice *slice_x = get_slice(intersection.lb());
      const Slice *slice_xdot = v.get_slice(intersection.lb());
      while(slice_x != NULL && slice_x->domain().lb() < intersection.ub())
      {
        if(slice_x->codomain().intersects(y))
          invert |= slice_x->invert(y, *slice_xdot, intersection);

        slice_x = slice_x->next_slice();
        slice_xdot = slice_xdot->next_slice();
      }

      return invert;
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Tube& v, const Interval& search_domain) const
    {
      v_t.clear();

      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return;

      const Slice *slice_x = get_slice(intersection.lb());
      const Slice *slice_xdot = v.get_slice(intersection.lb());
      while(slice_x != NULL && slice_x->domain().lb() <= intersection.ub())
      {
        Interval local_invert = slice_x->invert(y, *slice_xdot, intersection);
        if(local_invert.is_empty() && !invert.is_empty())
        {
          v_t.push_back(invert);
          invert.set_empty();
        }

        else
          invert |= local_invert;

        slice_x = slice_x->next_slice();
        slice_xdot = slice_xdot->next_slice();
      }

      if(!invert.is_empty())
        v_t.push_back(invert);
    }

    const pair<Interval,Interval> Tube::eval(const Interval& t) const
    {
      pair<Interval,Interval> enclosed_bounds
        = make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

      Interval intersection = t & domain();
      if(t.is_empty())
        return enclosed_bounds;

      const Slice *slice = get_slice(intersection.lb());
      while(slice != NULL && slice->domain().lb() <= intersection.ub())
      {
        pair<Interval,Interval> local_eval = slice->eval(intersection);
        enclosed_bounds.first |= local_eval.first;
        enclosed_bounds.second |= local_eval.second;
        slice = slice->next_slice();
      }

      return enclosed_bounds;
    }

    const Interval Tube::interpol(double t, const Tube& v) const
    {
      DomainException::check(*this, t);
      SlicingException::check(*this, v);

      const Slice *slice_x = get_slice(t);
      if(slice_x->domain().lb() == t || slice_x->domain().ub() == t)
        return (*slice_x)(t);

      return interpol(Interval(t), v);
      // todo: check a faster implementation for this degenerate case?
    }

    const Interval Tube::interpol(const Interval& t, const Tube& v) const
    {
      DomainException::check(*this, t);
      SlicingException::check(*this, v);

      Interval interpol = Interval::EMPTY_SET;

      const Slice *slice_x = get_slice(t.lb());
      const Slice *slice_xdot = v.get_slice(t.lb());
      while(slice_x != NULL && slice_x->domain().lb() < t.ub())
      {
        interpol |= slice_x->interpol(t & slice_x->domain(), *slice_xdot);
        slice_x = slice_x->next_slice();
        slice_xdot = slice_xdot->next_slice();
      }

      return interpol;
    }

    double Tube::max_thickness() const
    {
      const Slice *largest = get_largest_slice();

      if(largest->codomain().is_unbounded())
        return numeric_limits<double>::infinity();

      else
        return largest->codomain().diam();
    }

    double Tube::max_gate_thickness(double& t) const
    {
      const Slice *slice = get_first_slice();

      if(slice->input_gate().is_unbounded())
      {
        t = slice->domain().lb();
        return numeric_limits<double>::infinity();
      }

      double max_diam = 0.;
      double max_thickness = slice->input_gate().diam();

      while(slice != NULL)
      {
        if(slice->output_gate().is_unbounded())
        {
          t = slice->domain().ub();
          return numeric_limits<double>::infinity();
        }

        if(slice->output_gate().diam() > max_diam)
        {
          max_diam = slice->codomain().diam();
          max_thickness = slice->output_gate().diam();
          t = slice->domain().ub();
        }

        slice = slice->next_slice();
      }

      return max_thickness;
    }

    // Tests

    #define test_content(f, bool_output) \
      \
      SlicingException::check(*this, x); \
      const Slice *slice = get_first_slice(), *slice_x = x.get_first_slice(); \
      while(slice != NULL) \
      { \
        if(slice->f(*slice_x)) \
          return bool_output; \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return !bool_output;

    bool Tube::operator==(const Tube& x) const
    {
      if(x.nb_slices() != nb_slices())
        return false;
      test_content(operator!=, false);
    }

    bool Tube::operator!=(const Tube& x) const
    {
      if(x.nb_slices() != nb_slices())
        return true;
      test_content(operator!=, true);
    }

    bool Tube::is_subset(const Tube& x) const
    {
      test_content(is_subset, true);
    }

    bool Tube::is_strict_subset(const Tube& x) const
    {
      test_content(is_strict_subset, true);
    }

    bool Tube::is_empty() const
    {
      const Slice *slice = get_first_slice();
      while(slice != NULL)
      {
        if(slice->is_empty()) return true;
        slice = slice->next_slice();
      }
      return false;
    }

    bool Tube::encloses(const Trajectory& x) const
    {
      DomainException::check(*this, x);

      const Slice *slice = get_first_slice();
      while(slice != NULL)
      {
        if(!slice->encloses(x)) return false;
        slice = slice->next_slice();
      }
      
      return true;
    }

    // Setting values

    void Tube::set(const Interval& y)
    {
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set(y);
    }

    void Tube::set(const Interval& y, int slice_id)
    {
      SlicingException::check(*this, slice_id);
      get_slice(slice_id)->set(y);
    }

    void Tube::set(const Interval& y, double t)
    {
      DomainException::check(*this, t);
      sample(t, y);
    }

    void Tube::set(const Interval& y, const Interval& t)
    {
      DomainException::check(*this, t);

      if(t.is_degenerated())
        set(y, t.lb());

      else
      {
        sample(t.lb());
        sample(t.ub());

        Slice *slice = get_slice(input2index(t.lb()));
        while(slice != NULL && !(t & slice->domain()).is_degenerated())
        {
          slice->set(y);
          slice = slice->next_slice();
        }
      }
    }

    void Tube::set_empty()
    {
      set(Interval::EMPTY_SET);
    }

    const Tube& Tube::inflate(double rad)
    {
      Interval e(-rad,rad);

      // Setting envelopes before gates' inflation
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set_envelope(s->codomain() + e);

      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(s == get_first_slice())
          s->set_input_gate(s->input_gate() + e);
        s->set_output_gate(s->output_gate() + e);
      }

      return *this;
    }

    const Tube& Tube::inflate(const Trajectory& rad)
    {
      DomainException::check(*this, rad);

      // Setting envelopes before gates' inflation
      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        s->set_envelope(s->codomain() + Interval(-1.,1.) * rad(s->domain()));

      for(Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
      {
        if(s == get_first_slice())
          s->set_input_gate(s->input_gate() + Interval(-1.,1.) * rad(s->domain().lb()));
        s->set_output_gate(s->output_gate() + Interval(-1.,1.) * rad(s->domain().ub()));
      }

      return *this;
    }

    // Bisection
    
    const pair<Tube,Tube> Tube::bisect(double t, float ratio) const
    {
      DomainException::check(*this, t);
      pair<Tube,Tube> p = make_pair(*this,*this);

      LargestFirst bisector(0., ratio); // todo: bisect according to another rule than largest first?

      try
      {
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect(IntervalVector((*this)(t)));
        p.first.set(p_codomain.first[0], t);
        p.second.set(p_codomain.second[0], t);
      }

      catch(ibex::NoBisectableVariableException&)
      {
        throw Exception("Tube::bisect", "unable to bisect, degenerated slice (ibex::NoBisectableVariableException)");
      };

      return p;
    }

    // String
    
    ostream& operator<<(ostream& str, const Tube& x)
    {
      str << x.class_name() << " " << x.domain() << "↦" << x.codomain_box()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    const Interval Tube::integral(double t) const
    {
      DomainException::check(*this, t);
      return integral(Interval(t));
    }

    const Interval Tube::integral(const Interval& t) const
    {
      // todo: more accurate output in case of unbounded tubes (evaluate each dimension separately)
      DomainException::check(*this, t);
      pair<Interval,Interval> partial_ti = partial_integral(t);

      if(partial_ti.first.is_empty() || partial_ti.second.is_empty())
        return Interval::EMPTY_SET;

      else if(partial_ti.first.is_unbounded() || partial_ti.second.is_unbounded())
        return Interval::ALL_REALS;

      else
        return Interval(partial_ti.first.lb()) | partial_ti.second.ub();
    }

    const Interval Tube::integral(const Interval& t1, const Interval& t2) const
    {
      // todo: more accurate output in case of unbounded tubes (evaluate each dimension separately)
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);

      pair<Interval,Interval> integral_t1 = partial_integral(t1);
      pair<Interval,Interval> integral_t2 = partial_integral(t2);

      if(integral_t1.first.is_empty() || integral_t1.second.is_empty() ||
         integral_t2.first.is_empty() || integral_t2.second.is_empty())
      {
        return Interval::EMPTY_SET;
      }

      else if(integral_t1.first.is_unbounded() || integral_t1.second.is_unbounded() ||
              integral_t2.first.is_unbounded() || integral_t2.second.is_unbounded())
      {
        return Interval::ALL_REALS;
      }

      else
      {
        double lb = (integral_t2.first - integral_t1.first).lb();
        double ub = (integral_t2.second - integral_t1.second).ub();
        return Interval(lb) | ub;
      }
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t) const
    {
      // todo: more accurate output in case of unbounded tubes (evaluate each dimension separately)
      DomainException::check(*this, t);

      Interval intv_t;
      const Slice *slice = get_first_slice();
      pair<Interval,Interval> p_integ
        = make_pair(0., 0.), p_integ_uncertain(p_integ);

      while(slice != NULL && slice->domain().lb() < t.ub())
      {
        if(slice->codomain().is_empty())
        {
          p_integ.first.set_empty();
          p_integ.second.set_empty();
          return p_integ;
        }

        if(slice->codomain().is_unbounded())
        {
          p_integ.first = Interval::ALL_REALS;
          p_integ.second = Interval::ALL_REALS;
          return p_integ;
        }

        // From t0 to tlb

          intv_t = slice->domain() & Interval(domain().lb(), t.lb());
          if(!intv_t.is_empty())
          {
            p_integ.first += intv_t.diam() * slice->codomain().lb();
            p_integ.second += intv_t.diam() * slice->codomain().ub();
            p_integ_uncertain = p_integ;
          }

        // From tlb to tub

          intv_t = slice->domain() & t;
          if(!intv_t.is_empty())
          {
            pair<Interval,Interval> p_integ_temp(p_integ_uncertain);
            p_integ_uncertain.first += Interval(0., intv_t.diam()) * slice->codomain().lb();
            p_integ_uncertain.second += Interval(0., intv_t.diam()) * slice->codomain().ub();
          
            p_integ.first |= p_integ_uncertain.first;
            p_integ.second |= p_integ_uncertain.second;

            p_integ_uncertain.first = p_integ_temp.first + intv_t.diam() * slice->codomain().lb();
            p_integ_uncertain.second = p_integ_temp.second + intv_t.diam() * slice->codomain().ub();
          }

        slice = slice->next_slice();
      }

      return p_integ;
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);
      pair<Interval,Interval> integral_t1 = partial_integral(t1);
      pair<Interval,Interval> integral_t2 = partial_integral(t2);
      return make_pair((integral_t2.first - integral_t1.first),
                       (integral_t2.second - integral_t1.second));
    }

    // Contractors

    bool Tube::ctc_deriv(const Tube& v)
    {
      CtcDeriv ctc;
      return ctc.contract(*this, v);
    }

    bool Tube::ctc_eval(Interval& t, Interval& z, Tube& w)
    {
      CtcEval ctc;
      return ctc.contract(t, z, *this, w);
    }
      
    // Serialization

    void Tube::serialize(const string& binary_file_name, int version_number) const
    {
      vector<Trajectory> v_trajs;
      serialize(binary_file_name, v_trajs, version_number);
    }

    void Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {
      vector<Trajectory> v_trajs;
      v_trajs.push_back(traj);
      serialize(binary_file_name, v_trajs, version_number);
    }
    
    void Tube::serialize(const string& binary_file_name, const vector<Trajectory>& v_trajs, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serialize_tube(bin_file, *this, version_number);

      int nb_trajs = v_trajs.size();
      bin_file.write((const char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < v_trajs.size() ; i++)
        serialize_traj(bin_file, v_trajs[i], version_number);

      bin_file.close();
    }

  // Protected methods

    // Access values

    const IntervalVector Tube::codomain_box() const
    {
      IntervalVector codomain(1, Interval::EMPTY_SET);
      for(const Slice *s = get_first_slice() ; s != NULL ; s = s->next_slice())
        codomain |= s->codomain_box();
      return codomain;
    }

    // Integration

    // Serialization

    void Tube::deserialize(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::deserialize()", "error while opening file \"" + binary_file_name + "\"");

      deserialize_tube(bin_file, *this);

      if(!bin_file.eof())
      {
        int nb_trajs;
        bin_file.read((char*)&nb_trajs, sizeof(int));
        for(int i = 0 ; i < nb_trajs ; i++)
        {
          Trajectory traj;
          deserialize_traj(bin_file, traj);
          v_trajs.push_back(traj);
        }
      }

      bin_file.close();
    }
}