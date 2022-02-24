/** 
 *  Tube class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Tube.h"
#include "codac_Exception.h"
#include "codac_CtcDeriv.h"
#include "codac_CtcEval.h"
#include "codac_serialize_trajectories.h"
#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Public methods

    // Definition

    Tube::Tube()
    {
      
    }

    Tube::Tube(const Interval& tdomain, const Interval& codomain)
    {
      assert(valid_tdomain(tdomain));

      // By default, the tube is defined as one single slice
      m_first_slice = new Slice(tdomain, codomain);
      
      // Redundant information for fast access
      m_tdomain = tdomain;
    }
    
    Tube::Tube(const Interval& tdomain, double timestep, const Interval& codomain)
    {
      assert(valid_tdomain(tdomain));
      assert(timestep >= 0.); // if 0., equivalent to no sampling

      // Redundant information for fast access
      m_tdomain = tdomain;

      Slice *prev_slice = nullptr, *slice;
      double lb, ub = tdomain.lb();

      if(timestep == 0.)
        timestep = tdomain.diam();

      do
      {
        lb = ub; // we guarantee all slices are adjacent
        ub = std::min(lb + timestep, tdomain.ub()); // the tdomain of the last slice may be smaller

        slice = new Slice(Interval(lb,ub));

        if(prev_slice)
        {
          delete slice->m_input_gate;
          slice->m_input_gate = nullptr;
          Slice::chain_slices(prev_slice, slice);
        }

        prev_slice = slice;
        if(m_first_slice == nullptr) m_first_slice = slice;
        slice = slice->next_slice();

      } while(ub < tdomain.ub());

      if(codomain != Interval::ALL_REALS)
        set(codomain);
    }
    
    Tube::Tube(const Interval& tdomain, double timestep, const TFnc& f, int f_image_id)
      : Tube(tdomain, timestep)
    {
      assert(valid_tdomain(tdomain));
      assert(timestep >= 0.); // if 0., equivalent to no sampling
      assert(f_image_id >= 0 && f_image_id < f.image_dim());
      assert(f.nb_var() == 0 && "function's inputs must be limited to system variable");

      // A scalar copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(1, *this);
      *this = f.eval_vector(input)[f_image_id];
    }
    
    Tube::Tube(const vector<Interval>& v_tdomains, const vector<Interval>& v_codomains)
    {
      assert(v_tdomains.size() == v_codomains.size());
      assert(!v_tdomains.empty());

      Interval tube_tdomain = Interval::EMPTY_SET;
      for(size_t i = 0 ; i < v_tdomains.size() ; i++)
      {
        assert(valid_tdomain(v_tdomains[i]));
        if(i > 0) assert(v_tdomains[i].lb() == v_tdomains[i-1].ub()); // domains continuity
        tube_tdomain |= v_tdomains[i];
      }

      m_first_slice = new Slice(tube_tdomain, Interval::ALL_REALS);
      Slice *s = m_first_slice;

      for(size_t i = 0 ; i < v_tdomains.size() ; i++)
      {
        sample(v_tdomains[i].ub(), s);
        s->set_envelope(v_codomains[i]);
        s = s->next_slice();
      }
      
      // Redundant information for fast access
      m_tdomain = tube_tdomain;
    }

    Tube::Tube(const Tube& x)
    {
      *this = x;
    }

    Tube::Tube(const Tube& x, const TFnc& f, int f_image_id)
      : Tube(x)
    {
      assert(f_image_id >= 0 && f_image_id < f.image_dim());
      assert(f.nb_var() == 0 && "function's inputs must be limited to system variable");

      // A scalar copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(1, *this);
      *this = f.eval_vector(input)[f_image_id];
    }

    Tube::Tube(const Trajectory& traj, double timestep)
      : Tube(traj.tdomain(), timestep)
    {
      assert(timestep >= 0.); // if 0., equivalent to no sampling
      set_empty();
      *this |= traj;
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep)
      : Tube(lb.tdomain(), timestep)
    {
      assert(timestep >= 0.); // if 0., equivalent to no sampling
      assert(lb.tdomain() == ub.tdomain());
      set_empty();
      *this |= lb; *this |= ub;
    }

    Tube::Tube(const string& binary_file_name)
    {
      Trajectory *traj;
      deserialize(binary_file_name, traj);
    }
    
    Tube::Tube(const string& binary_file_name, Trajectory *&traj)
    {
      deserialize(binary_file_name, traj);

      if(traj == nullptr)
        throw Exception(__func__, "unable to deserialize Trajectory object");
    }
    
    Tube::~Tube()
    {
      delete_synthesis_tree();
      delete_polynomial_synthesis();

      Slice *slice = first_slice();
      while(slice)
      {
        Slice *next_slice = slice->next_slice();
        delete slice;
        slice = next_slice;
      }
    }

    int Tube::size() const
    {
      return 1; // scalar object
    }

    const Tube Tube::primitive(const Interval& c) const
    {
      Tube primitive(*this); // same slicing
      primitive.set(Interval::ALL_REALS); // initialized to [-oo,oo]
      primitive.set(c, primitive.tdomain().lb()); // initial condition
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, static_cast<const Tube&>(*this), TimePropag::FORWARD);
      return primitive;
    }

    const Tube& Tube::operator=(const Tube& x)
    {
      // Destroying already existing structure

        Slice *prev_slice, *slice = first_slice();
        while(slice)
        {
          Slice *next_slice = slice->next_slice();
          delete slice;
          slice = next_slice;
        }

        delete_synthesis_tree();
        delete_polynomial_synthesis();
      
      // Creating new structure

        prev_slice = nullptr; slice = nullptr;

        for(const Slice *s = x.first_slice() ; s ; s = s->next_slice())
        {
          if(slice == nullptr)
          {
            slice = new Slice(*s);
            m_first_slice = slice;
          }

          else
          {
            slice->m_next_slice = new Slice(*s);
            slice = slice->next_slice();
          }

          if(prev_slice)
          {
            delete slice->m_input_gate;
            slice->m_input_gate = nullptr;
            Slice::chain_slices(prev_slice, slice);
          }

          prev_slice = slice;
        }

        // Redundant information for fast access
        m_tdomain = x.tdomain();

      return *this;
    }

    const Interval Tube::tdomain() const
    {
      return m_tdomain; // redundant information for fast access
      return Interval(first_slice()->tdomain().lb(),
                      last_slice()->tdomain().ub());
    }
    
    const Polygon Tube::polygon_envelope() const
    {
      if(is_empty())
        return Polygon();

      vector<Vector> v_pts;
      const Slice *s = nullptr;

      do
      {
        if(s == nullptr) // first iteration
          s = first_slice();
        else
          s = s->next_slice();

        v_pts.push_back(Vector({s->tdomain().lb(), s->codomain().ub()}));
        v_pts.push_back(Vector({s->tdomain().ub(), s->codomain().ub()}));

      } while(s->next_slice());

      while(s)
      {
        v_pts.push_back(Vector({s->tdomain().ub(), s->codomain().lb()}));
        v_pts.push_back(Vector({s->tdomain().lb(), s->codomain().lb()}));

        s = s->prev_slice();
      }
      
      return Polygon(v_pts);
    }
    
    const Trajectory Tube::lb() const
    {
      Trajectory lb;

      const Slice *s_x = first_slice();
      lb.set(s_x->input_gate().lb(), s_x->tdomain().lb());

      while(s_x)
      {
        lb.set(s_x->output_gate().lb(), s_x->tdomain().ub());
        s_x = s_x->next_slice();
      }

      return lb;
    }
    
    const Trajectory Tube::ub() const
    {
      Trajectory ub;

      const Slice *s_x = first_slice();
      ub.set(s_x->input_gate().ub(), s_x->tdomain().lb());

      while(s_x)
      {
        ub.set(s_x->output_gate().ub(), s_x->tdomain().ub());
        s_x = s_x->next_slice();
      }

      return ub;
    }
  
    // Slices structure

    int Tube::nb_slices() const
    {
      if(m_synthesis_mode == SynthesisMode::BINARY_TREE) // fast evaluation
        return m_synthesis_tree->nb_slices();
      
      else
      {
        int size = 0;
        for(const Slice *s = first_slice() ; s ; s = s->next_slice())
          size ++;
        return size;
      }
    }

    Slice* Tube::slice(int slice_id)
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).slice(slice_id));
    }

    const Slice* Tube::slice(int slice_id) const
    {
      if(slice_id < 0 && slice_id >= nb_slices())
        return nullptr;

      if(m_synthesis_mode == SynthesisMode::BINARY_TREE) // fast access
        return m_synthesis_tree->slice(slice_id);
      
      else
      {
        int i = 0;

        for(const Slice *s = first_slice() ; s ; s = s->next_slice())
        {
          if(i == slice_id)
            return s;
          i++;
        }

        return nullptr;
      }
    }

    Slice* Tube::slice(double t)
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).slice(t));
    }

    const Slice* Tube::slice(double t) const
    {
      if(!tdomain().contains(t))
        return nullptr;

      if(m_synthesis_mode == SynthesisMode::BINARY_TREE) // fast evaluation
        return m_synthesis_tree->slice(t);
      
      else
      {
        const Slice *last_s = nullptr;
        for(const Slice *s = first_slice() ; s ; s = s->next_slice())
        {
          if(t < s->tdomain().ub())
            return s;
          last_s = s;
        }
        
        assert(last_s);
        return last_s;
      }
    }

    Slice* Tube::first_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).first_slice());
    }

    const Slice* Tube::first_slice() const
    {
      return m_first_slice;
    }

    Slice* Tube::last_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).last_slice());
    }

    const Slice* Tube::last_slice() const
    {
      if(m_synthesis_mode == SynthesisMode::BINARY_TREE) // fast evaluation
        return m_synthesis_tree->slice(nb_slices() - 1);
      
      else
      {
        for(const Slice *s = first_slice() ; true ; s = s->next_slice())
          if(s->next_slice() == nullptr)
            return s;
        return nullptr;
      }
    }

    Slice* Tube::wider_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).wider_slice());
    }

    const Slice* Tube::wider_slice() const
    {
      double max_tdomain_width = 0.;
      const Slice *wider_slice = first_slice();

      for(const Slice *s = first_slice() ; s ; s = s->next_slice())
        if(s->tdomain().diam() > max_tdomain_width)
        {
          wider_slice = s;
          max_tdomain_width = s->tdomain().diam();
        }

      return wider_slice;
    }

    Slice* Tube::largest_slice()
    {
      return const_cast<Slice*>(static_cast<const Tube&>(*this).largest_slice());
    }

    const Slice* Tube::largest_slice() const
    {
      double max_diam = 0.;
      const Slice *largest = first_slice();
      for(const Slice *s = first_slice() ; s ; s = s->next_slice())
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
    
    const Interval Tube::slice_tdomain(int slice_id) const
    {
      assert(slice_id >= 0 && slice_id < nb_slices());
      return slice(slice_id)->tdomain();
    }

    int Tube::time_to_index(double t) const
    {
      assert(tdomain().contains(t));

      if(m_synthesis_mode == SynthesisMode::BINARY_TREE) // fast evaluation
        return m_synthesis_tree->time_to_index(t);
      
      else
      {
        int i = -1;
        for(const Slice *s = first_slice() ; s ; s = s->next_slice())
        {
          i++;
          if(t < s->tdomain().ub())
            return i;
        }

        return i;
      }
    }

    int Tube::index(const Slice* slice) const
    {
      int i = 0;
      const Slice *it = first_slice();
      while(it && it != slice)
      {
        it = it->next_slice();
        if(it == nullptr) return -1;
        i++;
      }
      return i;
    }

    void Tube::sample(double t)
    {
      assert(tdomain().contains(t));

      Slice *slice_to_be_sampled = slice(t);
      sample(t, slice_to_be_sampled);
    }

    void Tube::sample(double t, Slice *slice_to_be_sampled)
    {
      assert(slice_to_be_sampled);
      assert(slice_to_be_sampled->tdomain().contains(t));

      if(slice_to_be_sampled->tdomain().lb() == t || slice_to_be_sampled->tdomain().ub() == t)
      {
        // No degenerate slice,
        // the method has no effect.
        return;
      }

      else
      {
        delete_synthesis_tree(); // todo: update tree if created, instead of delete
        delete_polynomial_synthesis(); // todo: update tree if created, instead of delete

        Slice *next_slice = slice_to_be_sampled->next_slice();

        // Creating new slice
        Slice *new_slice = new Slice(*slice_to_be_sampled);
        new_slice->set_tdomain(Interval(t, slice_to_be_sampled->tdomain().ub()));
        slice_to_be_sampled->set_tdomain(Interval(slice_to_be_sampled->tdomain().lb(), t));

        // Updated slices structure
        delete new_slice->m_input_gate;
        new_slice->m_input_gate = nullptr;
        Slice::chain_slices(new_slice, next_slice);
        Slice::chain_slices(slice_to_be_sampled, new_slice);
        new_slice->set_input_gate(new_slice->codomain());
      }
    }

    void Tube::sample(double t, const Interval& gate)
    {
      assert(tdomain().contains(t));

      delete_synthesis_tree(); // todo: update tree if created, instead of delete
      delete_polynomial_synthesis(); // todo: update tree if created, instead of delete

      sample(t);
      Slice *s = slice(t);
      if(t == s->tdomain().lb())
        s->set_input_gate(gate);
      else
        s->set_output_gate(gate);
    }

    void Tube::sample(const Tube& x)
    {
      assert(tdomain() == x.tdomain());

      for(const Slice *s = x.first_slice() ; s ; s = s->next_slice())
        sample(s->tdomain().ub());
    }

    bool Tube::gate_exists(double t) const
    {
      return slice(t)->tdomain().lb() == t || t == tdomain().ub();
    }

    void Tube::remove_gate(double t)
    {
      assert(tdomain().contains(t));
      assert(t != tdomain().lb() && t != tdomain().ub() && "cannot remove initial/final gates");

      Slice *s2 = slice(t);
      assert(s2->tdomain().lb() == t && "the gate must already exist");
      Slice *s1 = s2->prev_slice();

      Slice::merge_slices(s1, s2);
    }

    void Tube::merge_similar_slices(double distance_threshold)
    {
      Slice *s2 = first_slice();
      while(s2)
      {
        Slice *s1 = s2->prev_slice();
        Slice *next_slice = s2->next_slice();

        if(s1 && distance(s1->codomain(),s2->codomain()) < distance_threshold)
            Slice::merge_slices(s1, s2);
      
        s2 = next_slice;
      }
    }

    // Accessing values

    const Interval Tube::codomain() const
    {
      return codomain_box()[0];
    }

    double Tube::volume() const
    {
      double volume = 0.;
      for(const Slice *s = first_slice() ; s ; s = s->next_slice())
      {
        if(s->volume() == POS_INFINITY)
          return POS_INFINITY;
        volume += s->volume();
      }
      return volume;
    }

    const Interval Tube::operator()(int slice_id) const
    {
      assert(slice_id >= 0 && slice_id < nb_slices());
      return slice(slice_id)->codomain();
    }

    const Interval Tube::operator()(double t) const
    {
      assert(!isnan(t));
      if(!tdomain().contains(t))
        return Interval::all_reals();
      return slice(t)->operator()(t);
    }

    const Interval Tube::operator()(const Interval& t) const
    {
      if(t.is_empty())
        return Interval::empty_set();

      else if(t.lb() < tdomain().lb() || t.ub() > tdomain().ub())
        return Interval::all_reals();

      else if(t.is_degenerated())
        return operator()(t.lb());

      else
      {
        switch(m_synthesis_mode)
        {
          case SynthesisMode::BINARY_TREE:
          {
            return m_synthesis_tree->operator()(t);
          }

          case SynthesisMode::POLYNOMIAL:
          {
            return m_polynomial_synthesis->operator()(t);
          }

          case SynthesisMode::NONE:
          default:
          {
            const Slice *first_slice = slice(t.lb());
            const Slice *last_slice = slice(t.ub());
            if(last_slice->tdomain().lb() != t.ub())
              last_slice = last_slice->next_slice();

            Interval codomain = Interval::EMPTY_SET;
            for(const Slice *s = first_slice ; s != last_slice ; s = s->next_slice())
              codomain |= s->codomain();

            return codomain;
          }
        }
      }
    }

    const pair<Interval,Interval> Tube::eval(const Interval& t) const
    {
      switch(m_synthesis_mode)
      {
        case SynthesisMode::BINARY_TREE:
        {
          return m_synthesis_tree->eval(t);
        }

        case SynthesisMode::POLYNOMIAL:
          cout << "todo: Polynomial eval" << endl;
          // no break

        case SynthesisMode::NONE:
        default:
        {
          pair<Interval,Interval> enclosed_bounds
            = make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

          if(t.lb() < tdomain().lb() || t.ub() > tdomain().ub())
          {
            enclosed_bounds.first |= NEG_INFINITY;
            enclosed_bounds.second |= POS_INFINITY;
          }

          Interval t_tdomain = t & tdomain();
          const Slice *s = slice(t_tdomain.lb());
          while(s && s->tdomain().lb() <= t_tdomain.ub())
          {
            pair<Interval,Interval> local_eval = s->eval(t_tdomain & s->tdomain());
            enclosed_bounds.first |= local_eval.first;
            enclosed_bounds.second |= local_eval.second;
            s = s->next_slice();
          }

          return enclosed_bounds;
        }
      }
    }

    const Interval Tube::interpol(double t, const Tube& v) const
    {
      assert(tdomain().contains(t));
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      const Slice *s_x = slice(t);
      if(s_x->tdomain().lb() == t || s_x->tdomain().ub() == t)
        return (*s_x)(t);

      return interpol(Interval(t), v);
      // todo: check a faster implementation for this degenerate case?
    }

    const Interval Tube::interpol(const Interval& t, const Tube& v) const
    {
      assert(tdomain().is_superset(t));
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      Interval interpol = Interval::EMPTY_SET;

      const Slice *s_x = slice(t.lb());
      const Slice *s_v = v.slice(t.lb());
      while(s_x && s_x->tdomain().lb() < t.ub())
      {
        interpol |= s_x->interpol(t & s_x->tdomain(), *s_v);
        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }

      return interpol;
    }

    const Interval Tube::invert(const Interval& y, const Interval& search_tdomain) const
    {
      switch(m_synthesis_mode)
      {
        case SynthesisMode::BINARY_TREE:
        {
          return m_synthesis_tree->invert(y, search_tdomain);
        }

        case SynthesisMode::POLYNOMIAL:
        case SynthesisMode::NONE:
        default:
        {
          if(search_tdomain.is_empty())
            return Interval::empty_set();

          else if(search_tdomain.lb() < tdomain().lb() || search_tdomain.ub() > tdomain().ub())
            return Interval::all_reals();

          Interval invert = Interval::EMPTY_SET;

          const Slice *s_x = slice(search_tdomain.lb());
          while(s_x && s_x->tdomain().lb() < search_tdomain.ub())
          {
            invert |= s_x->invert(y, search_tdomain & s_x->tdomain());
            s_x = s_x->next_slice();
          }

          return invert;
        }
      }
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Interval& search_tdomain) const
    {
      // todo: optimize this:
      // todo: fast inversion with binary tree considering derivative information
      v_t.clear();

      // todo: tree computations

      if(search_tdomain.is_empty())
        return;

      else if(search_tdomain.lb() < tdomain().lb() || search_tdomain.ub() > tdomain().ub())
      {
        v_t.push_back(Interval::all_reals());
        return;
      }

      Interval invert = Interval::EMPTY_SET;

      const Slice *s_x = slice(search_tdomain.lb());
      while(s_x && s_x->tdomain().lb() <= search_tdomain.ub())
      {
        Interval local_invert = s_x->invert(y, search_tdomain & s_x->tdomain());
        if(local_invert.is_empty() && !invert.is_empty())
        {
          v_t.push_back(invert);
          invert.set_empty();
        }

        else
          invert |= local_invert;

        s_x = s_x->next_slice();
      }

      if(!invert.is_empty())
        v_t.push_back(invert);
    }

    const Interval Tube::invert(const Interval& y, const Tube& v, const Interval& search_tdomain) const
    {
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      if(search_tdomain.is_empty())
        return Interval::empty_set();

      else if(search_tdomain.lb() < tdomain().lb() || search_tdomain.ub() > tdomain().ub())
        return Interval::all_reals();

      // todo: tree computations

      Interval invert = Interval::EMPTY_SET;

      const Slice *s_x = slice(search_tdomain.lb());
      const Slice *s_v = v.slice(search_tdomain.lb());
      while(s_x && s_x->tdomain().lb() < search_tdomain.ub())
      {
        invert |= s_x->invert(y, *s_v, search_tdomain & s_x->tdomain());
        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }

      return invert;
    }

    void Tube::invert(const Interval& y, vector<Interval> &v_t, const Tube& v, const Interval& search_tdomain) const
    {
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));
      v_t.clear();

      // todo: tree computations

      if(search_tdomain.is_empty())
        return;

      else if(search_tdomain.lb() < tdomain().lb() || search_tdomain.ub() > tdomain().ub())
      {
        v_t.push_back(Interval::all_reals());
        return;
      }

      Interval invert = Interval::EMPTY_SET;

      const Slice *s_x = slice(search_tdomain.lb());
      const Slice *s_v = v.slice(search_tdomain.lb());
      while(s_x && s_x->tdomain().lb() <= search_tdomain.ub())
      {
        Interval local_invert = s_x->invert(y, *s_v, search_tdomain & s_x->tdomain());
        if(local_invert.is_empty() && !invert.is_empty())
        {
          v_t.push_back(invert);
          invert.set_empty();
        }

        else
          invert |= local_invert;

        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }

      if(!invert.is_empty())
        v_t.push_back(invert);
    }

    double Tube::max_diam() const
    {
      const Slice *largest = largest_slice();

      if(largest->codomain().is_unbounded())
        return numeric_limits<double>::infinity();

      else
        return largest->codomain().diam();
    }

    double Tube::max_gate_diam(double& t) const
    {
      const Slice *slice = first_slice();

      if(slice->input_gate().is_unbounded())
      {
        t = slice->tdomain().lb();
        return numeric_limits<double>::infinity();
      }

      double max_diam = 0.;
      double max_thickness = slice->input_gate().diam();

      while(slice)
      {
        if(slice->output_gate().is_unbounded())
        {
          t = slice->tdomain().ub();
          return numeric_limits<double>::infinity();
        }

        if(slice->output_gate().diam() > max_diam)
        {
          max_diam = slice->codomain().diam();
          max_thickness = slice->output_gate().diam();
          t = slice->tdomain().ub();
        }

        slice = slice->next_slice();
      }

      return max_thickness;
    }
    
    const Trajectory Tube::diam(bool gates_thicknesses) const
    {
      Trajectory thicknesses;

      const Slice *s_x = first_slice();
      if(gates_thicknesses)
        thicknesses.set(Slice::diam(s_x->input_gate()), s_x->tdomain().lb());
      else
        thicknesses.set(Slice::diam(s_x->codomain()), s_x->tdomain().lb());

      while(s_x)
      {
        thicknesses.set(Slice::diam(s_x->codomain()), ibex::next_float(s_x->tdomain().lb()));
        thicknesses.set(Slice::diam(s_x->codomain()), ibex::previous_float(s_x->tdomain().ub()));

        if(gates_thicknesses)
          thicknesses.set(Slice::diam(s_x->output_gate()), s_x->tdomain().ub());

        else if(s_x->next_slice() == nullptr)
          thicknesses.set(Slice::diam(s_x->codomain()), s_x->tdomain().ub());

        s_x = s_x->next_slice();
      }

      return thicknesses;
    }
    
    const Trajectory Tube::diam(const Tube& v) const
    {
      Trajectory thicknesses;

      const Slice *s_x = first_slice(), *s_v = v.first_slice();

      while(s_x)
      {
        ConvexPolygon p = s_x->polygon(*s_v);

        for(size_t i = 0 ; i < p.vertices().size() ; i++)
          thicknesses.set(Slice::diam(s_x->interpol(p[i][0], *s_v)), p[i][1]);

        s_x = s_x->next_slice();
        s_v = s_v->next_slice();
      }

      return thicknesses;
    }

    // Tests

    bool Tube::operator==(const Tube& x) const
    {
      if(x.nb_slices() != nb_slices())
        return false;

      const Slice *s = first_slice(), *s_x = x.first_slice();
      while(s)
      {
        if(*s != *s_x)
          return false;
        s = s->next_slice();
        s_x = s_x->next_slice();
      }
      return true;
    }

    bool Tube::operator!=(const Tube& x) const
    {
      if(x.nb_slices() != nb_slices())
        return true;

      const Slice *s = first_slice(), *s_x = x.first_slice();
      while(s)
      {
        if(*s != *s_x)
          return true;
        s = s->next_slice();
        s_x = s_x->next_slice();
      }
      return false;
    }

    #define macro_sets_comparison(f) \
      \
      if(same_slicing(*this, x)) /* faster */ \
      { \
        const Slice *s = first_slice(), *s_x = x.first_slice(); \
        while(s) \
        { \
          if(!s->f(*s_x)) \
            return false; \
          s = s->next_slice(); \
          s_x = s_x->next_slice(); \
        } \
        return true; \
      } \
      \
      else \
      { \
        const Slice *s = nullptr; \
        do \
        { \
          if(s == nullptr) /* first iteration */ \
            s = first_slice(); \
          else \
            s = s->next_slice(); \
           \
          if(!s->input_gate().f(x(s->tdomain().lb())) || \
             !s->codomain().f(x(s->tdomain()))) \
            return false; \
           \
        } while(s->next_slice()); \
         \
        if(!s->output_gate().f(x(s->tdomain().ub()))) \
          return false; \
        return true; \
      } \

    bool Tube::is_subset(const Tube& x) const
    {
      macro_sets_comparison(is_subset);
    }

    bool Tube::is_strict_subset(const Tube& x) const
    {
      return is_subset(x) && *this != x;
    }

    bool Tube::is_interior_subset(const Tube& x) const
    {
      macro_sets_comparison(is_interior_subset);
    }

    bool Tube::is_strict_interior_subset(const Tube& x) const
    {
      return is_interior_subset(x) && *this != x;
    }

    bool Tube::is_superset(const Tube& x) const
    {
      macro_sets_comparison(is_superset);
    }

    bool Tube::is_strict_superset(const Tube& x) const
    {
      return is_superset(x) && *this != x;
    }

    bool Tube::is_empty() const
    {
      //if(m_data_tree) // todo: faster computation from binary tree
      //  return m_data_tree->emptiness_synthesis().emptiness();

      // Fast implementation without redundant gate evaluations:
      const Slice *slice = first_slice();

      if(slice->input_gate().is_empty())
        return true;

      for( ; slice ; slice = slice->next_slice())
      {
        if(slice->codomain().is_empty() || slice->output_gate().is_empty())
          return true;
      }
      
      return false;
    }

    const BoolInterval Tube::contains(const Trajectory& x) const
    {
      assert(tdomain() == x.tdomain());

      BoolInterval result = YES;
      for(const Slice *s = first_slice() ; s ; s = s->next_slice())
      {
        BoolInterval b = s->contains(x);
        if(b == NO) return NO;
        else if(b == MAYBE) result = MAYBE; 
      }

      return result;
    }

    bool Tube::overlaps(const Tube& x, float ratio) const
    {
      assert(tdomain() == x.tdomain());

      Tube inter = *this & x;

      double overlaping_range = 0.;
      for(const Slice *s = inter.first_slice() ; s ; s = s->next_slice())
      {
        if(!s->codomain().is_empty())
          overlaping_range += s->tdomain().diam();
      }

      return overlaping_range / tdomain().diam() >= ratio;
    }

    // Setting values

    const Tube& Tube::set(const Interval& y)
    {
      for(Slice *s = first_slice() ; s ; s = s->next_slice())
        s->set(y);
      return *this;
    }

    const Tube& Tube::set(const Interval& y, int slice_id)
    {
      assert(slice_id >= 0 && slice_id < nb_slices());
      slice(slice_id)->set(y);
      return *this;
    }

    const Tube& Tube::set(const Interval& y, double t)
    {
      assert(tdomain().contains(t));
      sample(t, y);
      return *this;
    }

    const Tube& Tube::set(const Interval& y, const Interval& t)
    {
      assert(tdomain().is_superset(t));

      if(t.is_degenerated())
        set(y, t.lb());

      else
      {
        sample(t.lb());
        sample(t.ub());

        for(Slice *s = slice(time_to_index(t.lb())) ;
            s && !(t & s->tdomain()).is_degenerated() ;
            s = s->next_slice())
          s->set(y);
      }

      return *this;
    }

    const Tube& Tube::set_empty()
    {
      return set(Interval::EMPTY_SET);
    }

    const Tube& Tube::inflate(double rad)
    {
      assert(rad >= 0.);
      Interval e(-rad,rad);

      Slice *s = nullptr;
      do
      {
        if(s == nullptr) // first iteration
          s = first_slice();
        else
          s = s->next_slice();

        s->set_envelope(s->codomain() + e, false);
        s->set_input_gate(s->input_gate() + e, false);

      } while(s->next_slice());

      s->set_output_gate(s->output_gate() + e, false);
      return *this;
    }

    const Tube& Tube::inflate(const Trajectory& rad)
    {
      assert(rad.codomain().lb() >= 0.);
      assert(tdomain() == rad.tdomain());

      Slice *s = nullptr;
      do
      {
        if(s == nullptr) // first iteration
          s = first_slice();
        else
          s = s->next_slice();

        s->set_envelope(s->codomain()
          + Interval(-1.,1.) * rad(s->tdomain()), false);
        s->set_input_gate(s->input_gate()
          + Interval(-1.,1.) * rad(s->tdomain().lb()), false);

      } while(s->next_slice());

      s->set_output_gate(s->output_gate()
        + Interval(-1.,1.) * rad(s->tdomain().ub()), false);
      return *this;
    }

    Tube& Tube::truncate_tdomain(const Interval& t)
    {
      assert(valid_tdomain(t));
      assert(tdomain().is_superset(t));

      // The first slice is the slice containing t.lb()
      while(!m_first_slice->tdomain().contains(t.lb()))
      {
        Slice *s_next = m_first_slice->next_slice();
        delete m_first_slice;
        m_first_slice = s_next;
      }

      m_first_slice->set_tdomain(t & m_first_slice->tdomain());

      // After this iteration, the last slice will be the one containing t.ub()
      Slice *s_last = last_slice();
      while(!s_last->tdomain().contains(t.ub()))
      {
        Slice *s_prev = s_last->prev_slice();
        delete s_last;
        s_last = s_prev;
      }

      s_last->set_tdomain(t & s_last->tdomain());

      m_tdomain = t;
      delete_synthesis_tree(); // todo: update tree if created, instead of delete
      delete_polynomial_synthesis(); // todo: update tree if created, instead of delete
      return *this;
    }

    void Tube::shift_tdomain(double shift_ref)
    {
      for(Slice *s = first_slice() ; s ; s = s->next_slice())
        s->shift_tdomain(shift_ref);
      m_tdomain += shift_ref;
      delete_synthesis_tree();
      delete_polynomial_synthesis();
    }

    // Bisection
    
    const pair<Tube,Tube> Tube::bisect(double t, float ratio) const
    {
      assert(tdomain().contains(t));
      assert(Interval(0.,1.).interior_contains(ratio));

      pair<Tube,Tube> p = make_pair(*this,*this);
      LargestFirst bisector(0., ratio);

      try
      {
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect(IntervalVector((*this)(t)));
        p.first.set(p_codomain.first[0], t);
        p.second.set(p_codomain.second[0], t);
      }

      catch(ibex::NoBisectableVariableException&)
      {
        throw Exception(__func__, "unable to bisect, degenerated slice (ibex::NoBisectableVariableException)");
      };

      return p;
    }

    // String
    
    ostream& operator<<(ostream& str, const Tube& x)
    {
      str << x.class_name() << " " << x.tdomain() << "↦" << x.codomain_box()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Synthesis tree
    
    #ifdef USE_TUBE_TREE
    bool Tube::s_enable_syntheses = true;
    #else
    bool Tube::s_enable_syntheses = false;
    #endif
    
    void Tube::enable_syntheses(bool enable)
    {
      Tube::s_enable_syntheses = enable;
    }

    // Integration

    const Interval Tube::integral(double t) const
    {
      assert(tdomain().contains(t));
      return integral(Interval(t));
    }

    const Interval Tube::integral(const Interval& t) const
    {
      assert(tdomain().is_superset(t));
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
      assert(tdomain().is_superset(t1));
      assert(tdomain().is_superset(t2));

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
      assert(tdomain().is_superset(t));

      switch(m_synthesis_mode)
      {
        case SynthesisMode::BINARY_TREE:
        {
          return m_synthesis_tree->partial_integral(t);
        }

        case SynthesisMode::POLYNOMIAL:
        case SynthesisMode::NONE:
        default:
        {
          Interval intv_t;
          const Slice *slice = first_slice();
          pair<Interval,Interval> p_integ
            = make_pair(0., 0.), p_integ_uncertain(p_integ);

          while(slice && slice->tdomain().lb() < t.ub())
          {
            if(slice->codomain().is_empty())
              return make_pair(Interval::EMPTY_SET, Interval::EMPTY_SET);

            if(slice->codomain().is_unbounded())
              return make_pair(Interval::ALL_REALS, Interval::ALL_REALS);

            // From t0 to tlb

              intv_t = slice->tdomain() & Interval(tdomain().lb(), t.lb());
              if(!intv_t.is_empty())
              {
                p_integ.first += intv_t.diam() * slice->codomain().lb();
                p_integ.second += intv_t.diam() * slice->codomain().ub();
                p_integ_uncertain = p_integ;

                if(intv_t.ub() == t.ub())
                  return p_integ; // end of the integral evaluation
              }

            // From tlb to tub

              intv_t = slice->tdomain() & t;
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
      }
    }

    const pair<Interval,Interval> Tube::partial_integral(const Interval& t1, const Interval& t2) const
    {
      assert(tdomain().is_superset(t1));
      assert(tdomain().is_superset(t2));
      pair<Interval,Interval> integral_t1 = partial_integral(t1);
      pair<Interval,Interval> integral_t2 = partial_integral(t2);
      return make_pair((integral_t2.first - integral_t1.first),
                       (integral_t2.second - integral_t1.second));
    }
      
    // Serialization

    void Tube::serialize(const string& binary_file_name, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception(__func__, "error while writing file \"" + binary_file_name + "\"");

      serialize_Tube(bin_file, *this, version_number);
      bin_file.close();
    }

    void Tube::serialize(const string& binary_file_name, const Trajectory& traj, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception(__func__, "error while writing file \"" + binary_file_name + "\"");

      serialize_Tube(bin_file, *this, version_number);
      char c = 0; bin_file.write(&c, 1); // writing a bit to separate the two objects
      serialize_Trajectory(bin_file, traj, version_number);
      bin_file.close();
    }
    
    bool Tube::same_slicing(const Tube& x1, const Tube& x2)
    {
      if(x1.nb_slices() != x2.nb_slices())
        return false;

      const Slice *s2 = x2.first_slice();
      for(const Slice *s1 = x1.first_slice() ; s1 ; s1 = s1->next_slice())
      {
        if(s1->tdomain() != s2->tdomain())
          return false;
        s2 = s2->next_slice();
      }

      return true;
    }

    void Tube::enable_synthesis(SynthesisMode mode, double eps) const
    {
      switch(mode)
      {
        case SynthesisMode::BINARY_TREE:
        {
          create_synthesis_tree();
          break;
        }

        case SynthesisMode::POLYNOMIAL:
        {
          create_polynomial_synthesis(eps);
          break;
        }

        case SynthesisMode::NONE:
        default:
        {
          delete_synthesis_tree();
          delete_polynomial_synthesis();
          break;
        }
      }
    }

    const Tube Tube::hull(const list<Tube>& l_tubes)
    {
      assert(!l_tubes.empty());
      list<Tube>::const_iterator it = l_tubes.begin();
      Tube hull = *it;
      for(++it ; it != l_tubes.end() ; ++it)
        hull |= *it;
      return hull;
    }

  // Protected methods

    // Accessing values

    const IntervalVector Tube::codomain_box() const
    {
      switch(m_synthesis_mode)
      {
        case SynthesisMode::BINARY_TREE:
        {
          return m_synthesis_tree->codomain();
        }

        case SynthesisMode::POLYNOMIAL:
        {
          return m_polynomial_synthesis->operator()(tdomain());
        }

        case SynthesisMode::NONE:
        default:
        {
          IntervalVector codomain(1, Interval::EMPTY_SET);
          for(const Slice *s = first_slice() ; s ; s = s->next_slice())
            codomain |= s->codomain_box();
          return codomain;
        }
      }
    }

    // Integration

    // Serialization

    void Tube::deserialize(const string& binary_file_name, Trajectory *&traj)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception(__func__, "error while opening file \"" + binary_file_name + "\"");

      Tube *ptr;
      deserialize_Tube(bin_file, ptr);
      *this = *ptr;

      char c; bin_file.get(c); // reading a bit of separation

      if(!bin_file.eof())
        deserialize_Trajectory(bin_file, traj);

      else
        traj = nullptr;
      
      delete ptr;
      bin_file.close();
    }

    // Synthesis tree
    
    void Tube::create_synthesis_tree() const
    {
      delete_synthesis_tree();

      vector<const Slice*> v_slices;
      for(const Slice* s = first_slice() ; s ; s = s->next_slice())
        v_slices.push_back(s);

      m_synthesis_tree = new TubeTreeSynthesis(this, 0, nb_slices() - 1, v_slices);
      m_synthesis_mode = SynthesisMode::BINARY_TREE;
    }
    
    void Tube::delete_synthesis_tree() const
    {
      if(m_synthesis_mode == SynthesisMode::BINARY_TREE)
      {
        delete m_synthesis_tree;
        m_synthesis_tree = nullptr;
        m_synthesis_mode = SynthesisMode::NONE;
      }
    }

    // Polynomial synthesis
    
    void Tube::create_polynomial_synthesis(double eps) const
    {
      delete_polynomial_synthesis();
      m_polynomial_synthesis = new TubePolynomialSynthesis(*this, eps);
      m_synthesis_mode = SynthesisMode::POLYNOMIAL;
    }
    
    void Tube::delete_polynomial_synthesis() const
    {
      if(m_synthesis_mode == SynthesisMode::POLYNOMIAL)
      {
        delete m_polynomial_synthesis;
        m_polynomial_synthesis = nullptr;
        m_synthesis_mode = SynthesisMode::NONE;
      }
    }
}
