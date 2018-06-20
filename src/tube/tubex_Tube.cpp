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
#include "tubex_TubeSerialization.h"
#include "tubex_TrajectorySerialization.h"
#include "tubex_DimensionException.h"
//#include "tubex_CtcDeriv.h"
//#include "tubex_CtcEval.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Tube::Tube(const Interval& domain, int dim)
    {
      if(domain.is_empty() || domain.is_unbounded() || domain.is_degenerated())
        throw Exception("Tube constructor", "invalid domain");

      // By default, the tube is defined as one single slice
      TubeSlice *slice = new TubeSlice(domain, dim);
      slice->setTubeReference(this);
      m_v_slices.push_back(slice);
    }

    Tube::Tube(const Interval& domain, const IntervalVector& codomain)
      : Tube(domain, codomain.size())
    {
      set(codomain);
    }
    
    Tube::Tube(const Interval& domain, double timestep, int dim)
    {
      if(domain.is_empty() || domain.is_unbounded() || domain.is_degenerated())
        throw Exception("Tube constructor", "invalid domain");
      
      if(timestep < 0.)
        throw Exception("Tube constructor", "invalid timestep");

      else
      {
        TubeSlice *prev_slice = NULL;
        double lb, ub = domain.lb();

        if(timestep == 0.)
          timestep = domain.diam();

        do
        {
          lb = ub; // we guarantee all slices are adjacent
          ub = min(lb + timestep, domain.ub());

          TubeSlice *slice = new TubeSlice(Interval(lb,ub), dim);
          slice->setTubeReference(this);
          m_v_slices.push_back(slice);

          if(prev_slice != NULL)
            TubeSlice::chainSlices(prev_slice, slice);
          prev_slice = slice;

        } while(ub < domain.ub());
      }
    }
    
    Tube::Tube(const Interval& domain, double timestep, const IntervalVector& codomain)
      : Tube(domain, timestep, codomain.size())
    {
      set(codomain);
    }
    
    Tube::Tube(const Interval& domain, double timestep, const Function& function)
      : Tube(domain, timestep, function.image_dim())
    {
      set(function);
    }

    Tube::Tube(const Tube& x)
    {
      *this = x;
    }

    Tube::Tube(const Tube& x, const IntervalVector& codomain)
      : Tube(x)
    {
      DimensionException::check(x, codomain);
      set(codomain);
    }

    Tube::Tube(const Tube& x, const Function& function)
      : Tube(x)
    {
      DimensionException::check(x, function);
      set(function);
    }

    Tube::Tube(const Trajectory& traj, double timestep)
      : Tube(traj.domain(), timestep, traj.dim())
    {
      setEmpty();
      *this |= traj;
    }

    Tube::Tube(const Trajectory& lb, const Trajectory& ub, double timestep)
      : Tube(lb.domain(), timestep, lb.dim())
    {
      DimensionException::check(lb, ub);
      setEmpty();
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
      m_v_slices.clear();
    }

    Tube Tube::primitive() const
    {
      Tube primitive(domain(), dim());
      primitive.ctcFwd(*this);
      return primitive;
    }

    Tube& Tube::operator=(const Tube& x)
    {
      m_v_slices.clear();
      TubeSlice *prev_slice = NULL;

      for(int i = 0 ; i < x.nbSlices() ; i++)
      {
        TubeSlice *slice = new TubeSlice(*x.getSlice(i));
        slice->setTubeReference(this);
        m_v_slices.push_back(slice);

        if(prev_slice != NULL)
          TubeSlice::chainSlices(prev_slice, slice);
        prev_slice = slice;
      }

      return *this;
    }

    const Interval Tube::domain() const
    {
      // todo: read from tree structure
      return Interval((*m_v_slices.begin())->domain().lb(),
                      (*m_v_slices.rbegin())->domain().ub());
    }

    int Tube::dim() const
    {
      return m_v_slices[0]->dim();
    }
  
    // Slices structure

    int Tube::nbSlices() const
    {
      return m_v_slices.size();
    }

    TubeSlice* Tube::getSlice(int slice_id)
    {
      DomainException::check(*this, slice_id);
      return m_v_slices[slice_id];
    }

    const TubeSlice* Tube::getSlice(int slice_id) const
    {
      // todo: remove this?
      // check cast?
      DomainException::check(*this, slice_id);
      return m_v_slices[slice_id];
    }

    TubeSlice* Tube::getSlice(double t)
    {
      // todo: remove this?
      return getSlice(input2index(t));
    }

    const TubeSlice* Tube::getSlice(double t) const
    {
      // todo: remove this?
      // check cast?
      return getSlice(input2index(t));
    }

    TubeSlice* Tube::getFirstSlice() const
    {
      return m_v_slices[0];
    }

    TubeSlice* Tube::getLastSlice() const
    {
      return m_v_slices[nbSlices() - 1];
    }

    TubeSlice* Tube::getWiderSlice() const
    {
      double max_domain_width = 0.;
      TubeSlice *wider_slice, *slice = getFirstSlice();

      while(slice != NULL)
      {
        if(slice->domain().diam() > max_domain_width)
        {
          wider_slice = slice;
          max_domain_width = slice->domain().diam();
        }

        slice = slice->nextSlice();
      }

      return wider_slice;
    }

    //void Tube::getSlices(vector<const TubeSlice*>& v_slices) const
    //{
    //  // todo: remove this?
    //  //return m_v_slices;
    //}

    int Tube::input2index(double t) const
    {
      // todo: read from tree structure
      DomainException::check(*this, t);

      if(t == domain().ub())
        return nbSlices() - 1;

      for(int i = 0 ; i < nbSlices() ; i++)
        if(t >= m_v_slices[i]->domain().lb() && t < m_v_slices[i]->domain().ub())
          return i;
    }

    void Tube::sample(double t, const IntervalVector& gate)
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, gate);

      TubeSlice *slice_to_be_sampled = getSlice(t);
      TubeSlice *next_slice = slice_to_be_sampled->nextSlice();

      if(slice_to_be_sampled->domain().lb() == t || slice_to_be_sampled->domain().ub() == t)
      {
        // No degenerate slice,
        // the method has no effect.
        return;
      }

      // Creating new slice
      int new_slice_id = input2index(t) + 1;
      vector<TubeSlice*>::iterator it = m_v_slices.begin() + new_slice_id;
      m_v_slices.insert(it, new TubeSlice(*slice_to_be_sampled));
      m_v_slices[new_slice_id]->setTubeReference(this);

      // Updated slices structure
      TubeSlice::chainSlices(m_v_slices[new_slice_id], next_slice);
      TubeSlice::chainSlices(slice_to_be_sampled, m_v_slices[new_slice_id]);

      // Updated domains and gates
      m_v_slices[new_slice_id]->setDomain(Interval(t, slice_to_be_sampled->domain().ub()));
      slice_to_be_sampled->setDomain(Interval(slice_to_be_sampled->domain().lb(), t));
      slice_to_be_sampled->setOutputGate(gate);

      //TubeSlice *slice_to_be_sampled = getSlice(t);
      //
      //if(slice_to_be_sampled->domain().lb() == t || slice_to_be_sampled->domain().ub() == t)
      //{
      //  // No degenerate slice,
      //  // the method has no effect.
      //  return;
      //}
      //
      //else
      //{
      //  TubeComponent *parent = m_component->getParentOf(slice_to_be_sampled);
      //  TubeComponent *new_component = new TubeNode(*slice_to_be_sampled, t);
      //    
      //  if(parent == NULL) // no parent, the tube has one slice
      //  {
      //    delete m_component;
      //    m_component = new_component;
      //  }
      //  
      //  else
      //  {
      //    TubeComponent *first_component = ((TubeNode*)parent)->m_first_component;
      //    TubeComponent *second_component = ((TubeNode*)parent)->m_second_component;
      //    
      //    if(slice_to_be_sampled == (TubeSlice*)first_component)
      //    {
      //      delete first_component;
      //      ((TubeNode*)parent)->m_first_component = new_component;
      //    }
      //      
      //    else if(slice_to_be_sampled == (TubeSlice*)second_component)
      //    {
      //      delete second_component;
      //      ((TubeNode*)parent)->m_second_component = new_component;
      //    }
      //    
      //    else
      //      throw Exception("Tube::sample", "unhandled case");
      //  }
      //  
      //  m_component->updateSlicesNumber();
      //  set(gate, t);
      //}
    }

    //void Tube::sample(const vector<double>& v_bounds)
    //{
    //  // todo: remove this?
    //  
    //  if(v_bounds.empty())
    //    return;
    //  
    //  vector<double> v_first_bounds, v_last_bounds;
    //  
    //  int mid = v_bounds.size() / 2;
    //  for(int i = 0 ; i < v_bounds.size() ; i++)
    //  {
    //    if(i < mid) v_first_bounds.push_back(v_bounds[i]);
    //    else if(i <= mid) sample(v_bounds[i]);
    //    else v_last_bounds.push_back(v_bounds[i]);
    //  }
    //  
    //  sample(v_first_bounds);
    //  sample(v_last_bounds);
    //}
    //
    //TubeComponent* Tube::getTubeComponent()
    //{
    //  // todo: remove this?
    //  //return m_component;
    //}

    // Access values

    const IntervalVector Tube::codomain() const
    {
      // todo: use tree structure instead
      TubeSlice *slice = getFirstSlice();
      IntervalVector codomain(dim(), Interval::EMPTY_SET);
      while(slice != NULL)
      {
        codomain |= slice->codomain();
        slice = slice->nextSlice();
      }
      return codomain;
    }

    double Tube::volume() const
    {
      TubeSlice *slice = getFirstSlice();
      double volume = 0.;
      while(slice != NULL)
      {
        volume += slice->volume();
        slice = slice->nextSlice();
      }
      return volume;
    }

    const IntervalVector Tube::operator[](int slice_id) const
    {
      return getSlice(slice_id)->codomain();
    }

    const IntervalVector Tube::operator[](double t) const
    {
      return getSlice(t)->operator[](t);
    }

    const IntervalVector Tube::operator[](const Interval& t) const
    {
      DomainException::check(*this, t);

      if(t.is_degenerated())
        return operator[](t.lb());

      // todo: use tree structure instead
      const TubeSlice *slice = getSlice(t.lb());
      const TubeSlice *last_slice = getSlice(t.ub());
      if(last_slice->domain().lb() != t.ub())
        last_slice = last_slice->nextSlice();

      IntervalVector codomain(dim(), Interval::EMPTY_SET);

      while(slice != NULL && slice != last_slice)
      {
        codomain |= slice->codomain();
        slice = slice->nextSlice();
      }

      return codomain;
    }

    Interval Tube::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      // todo: use tree structure instead
      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return Interval::EMPTY_SET;

      const TubeSlice *slice = getSlice(intersection.lb());
      while(slice != NULL && slice->domain().lb() <= intersection.ub())
      {
        invert |= slice->invert(y, intersection);
        slice = slice->nextSlice();
      }

      return invert;
    }

    void Tube::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      v_t.clear();

      // todo: use tree structure instead
      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return;

      const TubeSlice *slice = getSlice(intersection.lb());
      while(slice != NULL && slice->domain().lb() <= intersection.ub())
      {
        Interval local_invert = slice->invert(y, intersection);
        if(local_invert.is_empty() && !invert.is_empty())
        {
          v_t.push_back(invert);
          invert.set_empty();
        }

        else
          invert |= local_invert;

        slice = slice->nextSlice();
      }

      if(!invert.is_empty())
        v_t.push_back(invert);
    }

    const pair<IntervalVector,IntervalVector> Tube::eval(const Interval& t) const
    {
      // todo: use tree structure instead

      pair<IntervalVector,IntervalVector> enclosed_bounds
        = make_pair(IntervalVector(dim(), Interval::EMPTY_SET), IntervalVector(dim(), Interval::EMPTY_SET));

      Interval intersection = t & domain();
      if(t.is_empty())
        return enclosed_bounds;

      const TubeSlice *slice = getSlice(intersection.lb());
      while(slice != NULL && slice->domain().lb() <= intersection.ub())
      {
        pair<IntervalVector,IntervalVector> local_eval = slice->eval(intersection);
        enclosed_bounds.first |= local_eval.first;
        enclosed_bounds.second |= local_eval.second;
        slice = slice->nextSlice();
      }

      return enclosed_bounds;
    }

    const IntervalVector Tube::interpol(double t, const Tube& derivative) const
    {
      return interpol(Interval(t), derivative);
      // todo: check a faster implementation for this degenerate case?
    }

    const IntervalVector Tube::interpol(const Interval& t, const Tube& derivative) const
    {
      //IntervalVector y;
      //CtcDeriv ctc;
      //Interval t_ = t;
      //ctc.contract(*this, derivative, t_, y);
      //return y;
    }

    double Tube::maxThickness()
    {
      int first_id_max_thickness;
      return maxThickness(first_id_max_thickness);
    }

    double Tube::maxThickness(int& first_id_max_thickness)
    {
      int i = 0;
      double max_thickness = 0.;

      TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        if(slice->codomain().max_diam() > max_thickness)
        {
          max_thickness = slice->codomain().max_diam();
          first_id_max_thickness = i;
        }

        slice = slice->nextSlice();
        i++;
      }

      return max_thickness;
    }

    // Tests

    bool Tube::operator==(const Tube& x) const
    {
      DimensionException::check(*this, x);
      // todo: common with other same-type methods
      if(x.nbSlices() != nbSlices())
        return false;

      TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(*slice != *slice_x)
          return false;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }

      return true;
    }

    bool Tube::operator!=(const Tube& x) const
    {
      DimensionException::check(*this, x);
      // todo: common with other same-type methods
      if(x.nbSlices() != nbSlices())
        return true;

      TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(*slice != *slice_x)
          return true;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }
      
      return false;
    }

    bool Tube::isSubset(const Tube& x) const
    {
      DimensionException::check(*this, x);
      // todo: common with other same-type methods
      StructureException::check(*this, x);

      TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isSubset(*slice_x))
          return true;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }
      
      return false;
    }

    bool Tube::isStrictSubset(const Tube& x) const
    {
      // todo: common with other same-type methods
      StructureException::check(*this, x);
      DimensionException::check(*this, x);

      TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isStrictSubset(*slice_x))
          return true;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }
      
      return false;
    }

    bool Tube::isEmpty() const
    {
      // todo: common with other same-type methods

      TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isEmpty())
          return true;
        slice = slice->nextSlice();
      }
      
      return false;
    }

    bool Tube::encloses(const Trajectory& x) const
    {
      // todo: common with other same-type methods
      DomainException::check(*this, x);
      DimensionException::check(*this, x);

      TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        if(!slice->encloses(x))
          return false;
        slice = slice->nextSlice();
      }
      
      return true;
    }

    // Setting values

    void Tube::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);

      TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        slice->set(y);
        slice = slice->nextSlice();
      }
    }

    void Tube::set(const IntervalVector& y, int slice_id)
    {
      DimensionException::check(*this, y);
      getSlice(slice_id)->set(y);
    }

    void Tube::set(const IntervalVector& y, double t)
    {
      DimensionException::check(*this, y);
      sample(t, y);
    }

    void Tube::set(const IntervalVector& y, const Interval& t)
    {
      DimensionException::check(*this, y);

      if(t.is_degenerated())
        set(y, t.lb());

      else
      {
        sample(t.lb(), y);
        sample(t.ub(), y);

        int i = input2index(t.lb());
        TubeSlice *slice = getSlice(i);

        for( ; i <= input2index(t.ub()) && slice != NULL ; i++)
        {
          if((t & slice->domain()).is_degenerated())
            continue;
          slice->set(y);
          slice = slice->nextSlice();
        }
      }
    }

    void Tube::set(const Function& f)
    {
      DimensionException::check(*this, f);
      TubeSlice *slice, *first_slice = getFirstSlice();

      // Setting envelopes
      slice = first_slice;
      while(slice != NULL)
      {
        IntervalVector iv_domain(1, slice->domain());
        slice->setEnvelope(f.eval_vector(iv_domain));
        slice = slice->nextSlice();
      }

      // Setting gates
      slice = first_slice;
      while(slice != NULL)
      {
        IntervalVector iv_domain_input(1, slice->domain().lb());
        slice->setInputGate(f.eval_vector(iv_domain_input));
        IntervalVector iv_domain_output(1, slice->domain().ub());
        slice->setOutputGate(f.eval_vector(iv_domain_output));
        slice = slice->nextSlice();
      }
    }

    void Tube::setEmpty()
    {
      IntervalVector empty_box(dim(), Interval::EMPTY_SET);
      set(empty_box);
    }

    Tube& Tube::inflate(double rad)
    {
      IntervalVector e(dim(), Interval(-rad,rad));

      TubeSlice *slice = getFirstSlice();
      TubeSlice *first_slice = slice;

      // Setting envelopes before gates' inflation
      while(slice != NULL)
      {
        slice->setEnvelope(slice->codomain() + e);
        slice = slice->nextSlice();
      }

      slice = first_slice;

      while(slice != NULL)
      {
        if(slice == first_slice)
          slice->setInputGate(slice->inputGate() + e);
        slice->setOutputGate(slice->outputGate() + e);
        slice = slice->nextSlice();
      }

      return *this;
    }

    // Bisection
    
    pair<Tube,Tube> Tube::bisect(double t, float ratio) const
    {
      pair<Tube,Tube> p = make_pair(*this,*this);

      LargestFirst bisector(0., ratio);

      try
      {
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect((*this)[t]);
        p.first.set(p_codomain.first, t);
        p.second.set(p_codomain.second, t);
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
      str << "Tube " << x.domain() << "↦" << x.codomain()
          << ", " << x.nbSlices()
          << " slice" << (x.nbSlices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    IntervalVector Tube::integral(double t) const
    {
      return integral(Interval(t));
    }

    IntervalVector Tube::integral(const Interval& t) const
    {
      //pair<IntervalVector,IntervalVector> partial_ti = partialIntegral(t);
      //return Interval(partial_ti.first.lb(), partial_ti.second.ub());
    }

    IntervalVector Tube::integral(const Interval& t1, const Interval& t2) const
    {
      //pair<IntervalVector,IntervalVector> integral_t1 = partialIntegral(t1);
      //pair<IntervalVector,IntervalVector> integral_t2 = partialIntegral(t2);
      //Vector lb = (integral_t2.first - integral_t1.first).lb();
      //Vector ub = (integral_t2.second - integral_t1.second).ub();
      //return Interval(min(lb, ub), max(lb, ub));
    }
//
    //pair<Interval,Interval> Tube::partialIntegral(const Interval& t) const
    //{
    //  checkPartialPrimitive();
    //  
    //  int index_lb = input2index(t.lb());
    //  int index_ub = input2index(t.ub());
    //  
    //  Interval integral_lb = Interval::EMPTY_SET;
    //  Interval integral_ub = Interval::EMPTY_SET;
    //  
    //  Interval intv_t_lb = getSlice(index_lb)->domain();
    //  Interval intv_t_ub = getSlice(index_ub)->domain();
    //  
    //  // Part A
    //  {
    //    pair<Interval,Interval> partial_primitive_first = getSlice(index_lb)->getPartialPrimitiveValue();
    //    Interval primitive_lb = Interval(partial_primitive_first.first.lb(), partial_primitive_first.second.ub());
    //      
    //    Interval y_first = (*this)[index_lb];
    //    Interval ta1 = Interval(intv_t_lb.lb(), t.lb());
    //    Interval ta2 = Interval(intv_t_lb.lb(), min(t.ub(), intv_t_lb.ub()));
    //    Interval tb1 = Interval(t.lb(), intv_t_lb.ub());
    //    Interval tb2 = Interval(min(t.ub(), intv_t_lb.ub()), intv_t_lb.ub());
    //    
    //    if(y_first.lb() < 0)
    //      integral_lb |= Interval(primitive_lb.lb() - y_first.lb() * tb2.diam(),
    //                              primitive_lb.lb() - y_first.lb() * tb1.diam());
    //    
    //    else if(y_first.lb() > 0)
    //      integral_lb |= Interval(primitive_lb.lb() + y_first.lb() * ta1.diam(),
    //                              primitive_lb.lb() + y_first.lb() * ta2.diam());
    //    
    //    if(y_first.ub() < 0)
    //      integral_ub |= Interval(primitive_lb.ub() + y_first.ub() * ta2.diam(),
    //                              primitive_lb.ub() + y_first.ub() * ta1.diam());
    //    
    //    else if(y_first.ub() > 0)
    //      integral_ub |= Interval(primitive_lb.ub() - y_first.ub() * tb1.diam(),
    //                              primitive_lb.ub() - y_first.ub() * tb2.diam());
    //  }
    //  
    //  // Part B
    //  if(index_ub - index_lb > 1)
    //  {
    //    pair<Interval,Interval> partial_primitive = m_component->getPartialPrimitiveValue(Interval(intv_t_lb.ub(), intv_t_ub.lb()));
    //    integral_lb |= partial_primitive.first;
    //    integral_ub |= partial_primitive.second;
    //  }
    //  
    //  // Part C
    //  if(index_lb != index_ub)
    //  {
    //    pair<Interval,Interval> partial_primitive_second = getSlice(index_ub)->getPartialPrimitiveValue();
    //    Interval primitive_ub = Interval(partial_primitive_second.first.lb(), partial_primitive_second.second.ub());
    //  
    //    Interval y_second = (*this)[index_ub];
    //    Interval ta = Interval(intv_t_ub.lb(), t.ub());
    //    Interval tb1 = intv_t_ub;
    //    Interval tb2 = Interval(t.ub(), intv_t_ub.ub());
    //  
    //    if(y_second.lb() < 0)
    //      integral_lb |= Interval(primitive_ub.lb() - y_second.lb() * tb2.diam(),
    //                              primitive_ub.lb() - y_second.lb() * tb1.diam());
    //    
    //    else if(y_second.lb() > 0)
    //      integral_lb |= Interval(primitive_ub.lb(),
    //                              primitive_ub.lb() + y_second.lb() * ta.diam());
    //    
    //    if(y_second.ub() < 0)
    //      integral_ub |= Interval(primitive_ub.ub() + y_second.ub() * ta.diam(),
    //                              primitive_ub.ub());
    //  
    //    else if(y_second.ub() > 0)
    //      integral_ub |= Interval(primitive_ub.ub() - y_second.ub() * tb1.diam(),
    //                              primitive_ub.ub() - y_second.ub() * tb2.diam());
    //  }
    //  
    //  return make_pair(integral_lb, integral_ub);
    //}

    pair<IntervalVector,IntervalVector> Tube::partialIntegral(const Interval& t1, const Interval& t2) const
    {
      //pair<IntervalVector,IntervalVector> integral_t1 = partialIntegral(t1);
      //pair<IntervalVector,IntervalVector> integral_t2 = partialIntegral(t2);
      //return make_pair((integral_t2.first - integral_t1.first),
      //                 (integral_t2.second - integral_t1.second));
    }

    // Contractors

    bool Tube::ctcFwd(const Tube& derivative)
    {
      DimensionException::check(*this, derivative);
      //CtcDeriv ctc;
      //return ctc.contractFwd(*this, derivative);
    }

    bool Tube::ctcBwd(const Tube& derivative)
    {
      DimensionException::check(*this, derivative);
      //CtcDeriv ctc;
      //return ctc.contractBwd(*this, derivative);
    }

    bool Tube::ctcFwdBwd(const Tube& derivative)
    {
      DimensionException::check(*this, derivative);
      //CtcDeriv ctc;
      //return ctc.contract(*this, derivative);
    }

    bool Tube::ctcEval(Interval& t, IntervalVector& z, const Tube& derivative, bool propagate)
    {
      DimensionException::check(*this, z);
      DimensionException::check(*this, derivative);
      //CtcEval ctc;
      //return ctc.contract(t, z, *this, derivative, propagate);
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

      serializeTube(bin_file, *this, version_number);

      int nb_trajs = v_trajs.size();
      bin_file.write((const char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < v_trajs.size() ; i++)
        serializeTrajectory(bin_file, v_trajs[i], version_number);

      bin_file.close();
    }

  // Protected methods

    // Integration

    //void Tube::checkPartialPrimitive() const
    //{
    //  // Warning: this method can only be called from the root (Tube class)
    //  // (because computation starts from 0)
    //  
    //  /*if(m_component->m_primitive_update_needed)
    //  {
    //    Interval sum_max = Interval(0);
    //    
    //    TubeSlice *slice = getFirstSlice();
    //    while(slice != NULL)
    //    {
    //      double dt = slice->domain().diam();
    //      Interval slice_codomain = slice->codomain();
    //      Interval integral_value = sum_max + slice_codomain * Interval(0., dt);
    //      slice->m_partial_primitive = make_pair(Interval(integral_value.lb(), integral_value.lb() + fabs(slice_codomain.lb() * dt)),
    //                                             Interval(integral_value.ub() - fabs(slice_codomain.ub() * dt), integral_value.ub()));
    //      slice->m_primitive_update_needed = true;
    //      sum_max += slice_codomain * dt;
    //      slice = slice->nextSlice();
    //    }
    //    
    //    m_component->checkPartialPrimitive(); // updating nodes from leafs information
    //  }
    //}

    // Serialization

    void Tube::deserialize(const string& binary_file_name, vector<Trajectory>& v_trajs)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("Tube::deserialize()", "error while opening file \"" + binary_file_name + "\"");

      deserializeTube(bin_file, *this);

      if(!bin_file.eof())
      {
        int nb_trajs;
        bin_file.read((char*)&nb_trajs, sizeof(int));
        for(int i = 0 ; i < nb_trajs ; i++)
        {
          Trajectory traj;
          deserializeTrajectory(bin_file, traj);
          v_trajs.push_back(traj);
        }
      }

      bin_file.close();
    }
}