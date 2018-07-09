/* ============================================================================
 *  tubex-lib - TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeVector.h"
#include "tubex_Exception.h"
#include "tubex_DomainException.h"
#include "tubex_TubeVectorSerialization.h"
#include "tubex_TrajectoryVectorSerialization.h"
#include "tubex_DimensionException.h"
#include "tubex_StructureException.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "ibex_LargestFirst.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeVector::TubeVector(const Interval& domain, int dim)
      : TubeVector(domain, IntervalVector(dim))
    {
      DomainException::check(domain);
      DimensionException::check(dim);
    }

    TubeVector::TubeVector(const Interval& domain, const IntervalVector& codomain)
    {
      DomainException::check(domain);
      DimensionException::check(codomain.size());

      // By default, the tube is defined as one single slice
      TubeSlice *slice = new TubeSlice(domain, codomain);
      slice->setTubeReference(this);
      m_v_slices.push_back(slice);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, int dim)
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      DimensionException::check(dim);

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
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const IntervalVector& codomain)
      : TubeVector(domain, timestep, codomain.size())
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const tubex::Fnc& f)
      : TubeVector(domain, timestep, f.imageDim())
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      // todo: check nb of input args (one only)
      *this = f.eval(TubeVector(*this, IntervalVector(1)));
    }

    TubeVector::TubeVector(const TubeVector& x)
    {
      *this = x;
    }

    TubeVector::TubeVector(const TubeVector& x, const IntervalVector& codomain)
      : TubeVector(x)
    {
      DimensionException::check(x, codomain);
      set(codomain);
    }

    TubeVector::TubeVector(const TrajectoryVector& traj, double timestep)
      : TubeVector(traj.domain(), timestep, traj.dim())
    {
      DomainException::check(timestep);
      setEmpty();
      *this |= traj;
    }

    TubeVector::TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep)
      : TubeVector(lb.domain(), timestep, lb.dim())
    {
      DomainException::check(timestep);
      DimensionException::check(lb, ub);
      setEmpty();
      *this |= lb;
      *this |= ub;
    }

    TubeVector::TubeVector(const string& binary_file_name)
    {
      vector<TrajectoryVector> v_trajs;
      deserialize(binary_file_name, v_trajs);
    }
    
    TubeVector::TubeVector(const string& binary_file_name, TrajectoryVector& traj)
    {
      vector<TrajectoryVector> v_trajs;
      deserialize(binary_file_name, v_trajs);

      if(v_trajs.size() == 0)
        throw Exception("TubeVector constructor", "unable to deserialize a TrajectoryVector");

      traj = v_trajs[0];
    }

    TubeVector::TubeVector(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {
      deserialize(binary_file_name, v_trajs);
      if(v_trajs.size() == 0)
        throw Exception("TubeVector constructor", "unable to deserialize some TrajectoryVector");
    }
    
    TubeVector::~TubeVector()
    {
      m_v_slices.clear();
    }

    const TubeVector TubeVector::primitive() const
    {
      TubeVector primitive(*this, IntervalVector(dim(), Interval::ALL_REALS));
      primitive.set(IntervalVector(dim(), 0.), primitive.domain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contractFwd(primitive, *this);
      return primitive;
    }

    const TubeVector& TubeVector::operator=(const TubeVector& x)
    {
      m_v_slices.clear();
      TubeSlice *prev_slice = NULL;
      const TubeSlice *slice_x = x.getFirstSlice();

      while(slice_x != NULL)
      {
        TubeSlice *slice = new TubeSlice(*slice_x);
        slice->setTubeReference(this);
        m_v_slices.push_back(slice);

        if(prev_slice != NULL)
          TubeSlice::chainSlices(prev_slice, slice);
        prev_slice = slice;
        slice_x = slice_x->nextSlice();
      }

      return *this;
    }

    const Interval TubeVector::domain() const
    {
      // todo: read from tree structure
      return Interval(m_v_slices.front()->domain().lb(),
                      m_v_slices.back()->domain().ub());
    }

    int TubeVector::dim() const
    {
      return m_v_slices[0]->dim();
    }
  
    // Slices structure

    int TubeVector::nbSlices() const
    {
      return m_v_slices.size();
    }

    TubeSlice* TubeVector::getSlice(int slice_id)
    {
      StructureException::check(*this, slice_id);
      return const_cast<TubeSlice*>(static_cast<const TubeVector&>(*this).getSlice(slice_id));
    }

    const TubeSlice* TubeVector::getSlice(int slice_id) const
    {
      StructureException::check(*this, slice_id);
      return m_v_slices[slice_id];
    }

    TubeSlice* TubeVector::getSlice(double t)
    {
      DomainException::check(*this, t);
      return const_cast<TubeSlice*>(static_cast<const TubeVector&>(*this).getSlice(t));
    }

    const TubeSlice* TubeVector::getSlice(double t) const
    {
      DomainException::check(*this, t);
      return getSlice(input2index(t));
    }

    TubeSlice* TubeVector::getFirstSlice()
    {
      return const_cast<TubeSlice*>(static_cast<const TubeVector&>(*this).getFirstSlice());
    }

    const TubeSlice* TubeVector::getFirstSlice() const
    {
      return m_v_slices[0];
    }

    TubeSlice* TubeVector::getLastSlice()
    {
      return const_cast<TubeSlice*>(static_cast<const TubeVector&>(*this).getLastSlice());
    }

    const TubeSlice* TubeVector::getLastSlice() const
    {
      return m_v_slices[nbSlices() - 1];
    }

    TubeSlice* TubeVector::getWiderSlice()
    {
      return const_cast<TubeSlice*>(static_cast<const TubeVector&>(*this).getWiderSlice());
    }

    const TubeSlice* TubeVector::getWiderSlice() const
    {
      double max_domain_width = 0.;
      const TubeSlice *wider_slice, *slice = getFirstSlice();

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

    int TubeVector::input2index(double t) const
    {
      DomainException::check(*this, t);
      // todo: read from tree structure

      int i = -1;
      const TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        i++;
        if(t < slice->domain().ub())
          return i;
        slice = slice->nextSlice();
      }

      return i;
    }

    void TubeVector::sample(double t)
    {
      DomainException::check(*this, t);

      TubeSlice *slice_to_be_sampled = getSlice(t);
      TubeSlice *next_slice = slice_to_be_sampled->nextSlice();

      if(slice_to_be_sampled->domain().lb() == t || slice_to_be_sampled->domain().ub() == t)
      {
        // No degenerate slice,
        // the method has no effect.
        return;
      }

      // Creating new slice
      TubeSlice *new_slice = new TubeSlice(*slice_to_be_sampled);
      new_slice->setTubeReference(this);
      new_slice->setDomain(Interval(t, slice_to_be_sampled->domain().ub()));
      slice_to_be_sampled->setDomain(Interval(slice_to_be_sampled->domain().lb(), t));

      vector<TubeSlice*>::iterator it = find(m_v_slices.begin(), m_v_slices.end(), slice_to_be_sampled);
      m_v_slices.insert(++it, new_slice);

      // Updated slices structure
      TubeSlice::chainSlices(new_slice, next_slice);
      TubeSlice::chainSlices(slice_to_be_sampled, new_slice);
      new_slice->setInputGate(new_slice->codomain());
    }

    void TubeVector::sample(double t, const IntervalVector& gate)
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, gate);
      sample(t);
      TubeSlice *slice = getSlice(t);
      if(t == slice->domain().lb())
        slice->setInputGate(gate);
      else
        slice->setOutputGate(gate);
    }

    // Access values

    const IntervalVector TubeVector::codomain() const
    {
      return codomainBox();
    }

    double TubeVector::volume() const
    {
      const TubeSlice *slice = getFirstSlice();
      double volume = 0.;
      while(slice != NULL)
      {
        volume += slice->volume();
        slice = slice->nextSlice();
      }
      return volume;
    }

    const IntervalVector TubeVector::operator[](int slice_id) const
    {
      StructureException::check(*this, slice_id);
      return getSlice(slice_id)->codomain();
    }

    const IntervalVector TubeVector::operator[](double t) const
    {
      DomainException::check(*this, t);
      return getSlice(t)->operator[](t);
    }

    const IntervalVector TubeVector::operator[](const Interval& t) const
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

    const Interval TubeVector::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      TubeVector derivative(*this, Interval::ALL_REALS); // todo: optimize this
      return invert(y, derivative, search_domain);
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      TubeVector derivative(*this, Interval::ALL_REALS); // todo: optimize this
      v_t.clear();
      invert(y, v_t, derivative, search_domain);
    }

    const Interval TubeVector::invert(const IntervalVector& y, const TubeVector& derivative, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      StructureException::check(*this, derivative);
      DomainException::check(*this, derivative);

      // todo: use tree structure instead
      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return Interval::EMPTY_SET;

      const TubeSlice *slice_x = getSlice(intersection.lb());
      const TubeSlice *slice_xdot = derivative.getSlice(intersection.lb());
      while(slice_x != NULL && slice_x->domain().lb() < intersection.ub())
      {
        if(slice_x->codomain().intersects(y))
          invert |= slice_x->invert(y, *slice_xdot, intersection);

        slice_x = slice_x->nextSlice();
        slice_xdot = slice_xdot->nextSlice();
      }

      return invert;
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const TubeVector& derivative, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      v_t.clear();

      // todo: use tree structure instead
      Interval invert = Interval::EMPTY_SET;
      Interval intersection = search_domain & domain();
      if(intersection.is_empty())
        return;

      const TubeSlice *slice_x = getSlice(intersection.lb());
      const TubeSlice *slice_xdot = derivative.getSlice(intersection.lb());
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

        slice_x = slice_x->nextSlice();
        slice_xdot = slice_xdot->nextSlice();
      }

      if(!invert.is_empty())
        v_t.push_back(invert);
    }

    const pair<IntervalVector,IntervalVector> TubeVector::eval(const Interval& t) const
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

    const IntervalVector TubeVector::interpol(double t, const TubeVector& derivative) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, derivative);
      StructureException::check(*this, derivative);

      const TubeSlice *slice_x = getSlice(t);
      if(slice_x->domain().lb() == t || slice_x->domain().ub() == t)
        return (*slice_x)[t];

      return interpol(Interval(t), derivative);
      // todo: check a faster implementation for this degenerate case?
    }

    const IntervalVector TubeVector::interpol(const Interval& t, const TubeVector& derivative) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, derivative);
      StructureException::check(*this, derivative);

      IntervalVector interpol(dim(), Interval::EMPTY_SET);

      const TubeSlice *slice_x = getSlice(t.lb());
      const TubeSlice *slice_xdot = derivative.getSlice(t.lb());
      while(slice_x != NULL && slice_x->domain().lb() < t.ub())
      {
        interpol |= slice_x->interpol(t & slice_x->domain(), *slice_xdot);
        slice_x = slice_x->nextSlice();
        slice_xdot = slice_xdot->nextSlice();
      }

      return interpol;
    }

    double TubeVector::maxThickness() const
    {
      int first_id_max_thickness;
      return maxThickness(first_id_max_thickness);
    }

    double TubeVector::maxThickness(int& first_id_max_thickness) const
    {
      int i = 0;
      double max_thickness = 0.;

      const TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        if(slice->codomain().is_unbounded())
        {
          first_id_max_thickness = i;
          return numeric_limits<double>::infinity();
        }

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

    double TubeVector::maxGateThickness(double& t) const
    {
      const TubeSlice *slice = getFirstSlice();

      if(slice->inputGate().is_unbounded())
      {
        t = slice->domain().lb();
        return numeric_limits<double>::infinity();
      }

      double max_thickness = slice->inputGate().max_diam();

      while(slice != NULL)
      {
        if(slice->outputGate().is_unbounded())
        {
          t = slice->domain().ub();
          return numeric_limits<double>::infinity();
        }

        if(slice->outputGate().max_diam() > max_thickness)
        {
          max_thickness = slice->outputGate().max_diam();
          t = slice->domain().ub();
        }

        slice = slice->nextSlice();
      }

      return max_thickness;
    }

    // Tests

    bool TubeVector::operator==(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      // todo: common with other same-type methods
      if(x.nbSlices() != nbSlices())
        return false;

      const TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(*slice != *slice_x)
          return false;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }

      return true;
    }

    bool TubeVector::operator!=(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      // todo: common with other same-type methods
      if(x.nbSlices() != nbSlices())
        return true;

      const TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(*slice != *slice_x)
          return true;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }
      
      return false;
    }

    bool TubeVector::isSubset(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      StructureException::check(*this, x);
      // todo: common with other same-type methods

      const TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isSubset(*slice_x))
          return true;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }
      
      return false;
    }

    bool TubeVector::isStrictSubset(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      StructureException::check(*this, x);
      // todo: common with other same-type methods

      const TubeSlice *slice = getFirstSlice(), *slice_x = x.getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isStrictSubset(*slice_x))
          return true;
        slice = slice->nextSlice();
        slice_x = slice_x->nextSlice();
      }
      
      return false;
    }

    bool TubeVector::isEmpty() const
    {
      // todo: common with other same-type methods

      const TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isEmpty())
          return true;
        slice = slice->nextSlice();
      }
      
      return false;
    }

    bool TubeVector::encloses(const TrajectoryVector& x) const
    {
      // todo: common with other same-type methods
      DomainException::check(*this, x);
      DimensionException::check(*this, x);

      const TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        if(!slice->encloses(x))
          return false;
        slice = slice->nextSlice();
      }
      
      return true;
    }

    // Setting values

    void TubeVector::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);

      TubeSlice *slice = getFirstSlice();
      while(slice != NULL)
      {
        slice->set(y);
        slice = slice->nextSlice();
      }
    }

    void TubeVector::set(const IntervalVector& y, int slice_id)
    {
      DimensionException::check(*this, y);
      StructureException::check(*this, slice_id);
      getSlice(slice_id)->set(y);
    }

    void TubeVector::set(const IntervalVector& y, double t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);
      sample(t, y);
    }

    void TubeVector::set(const IntervalVector& y, const Interval& t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);

      if(t.is_degenerated())
        set(y, t.lb());

      else
      {
        sample(t.lb());
        sample(t.ub());

        TubeSlice *slice = getSlice(input2index(t.lb()));
        while(slice != NULL && !(t & slice->domain()).is_degenerated())
        {
          slice->set(y);
          slice = slice->nextSlice();
        }
      }
    }

    void TubeVector::setEmpty()
    {
      IntervalVector empty_box(dim(), Interval::EMPTY_SET);
      set(empty_box);
    }

    const TubeVector& TubeVector::inflate(double rad)
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
    
    pair<TubeVector,TubeVector> TubeVector::bisect(double t, float ratio) const
    {
      DomainException::check(*this, t);
      pair<TubeVector,TubeVector> p = make_pair(*this,*this);

      LargestFirst bisector(0., ratio);

      try
      {
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect((*this)[t]);
        p.first.set(p_codomain.first, t);
        p.second.set(p_codomain.second, t);
      }

      catch(ibex::NoBisectableVariableException&)
      {
        throw Exception("TubeVector::bisect", "unable to bisect, degenerated slice (ibex::NoBisectableVariableException)");
      };

      return p;
    }

    // String
    
    ostream& operator<<(ostream& str, const TubeVector& x)
    {
      str << "TubeVector " << x.domain() << "↦" << x.codomain()
          << ", " << x.nbSlices()
          << " slice" << (x.nbSlices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    const IntervalVector TubeVector::integral(double t) const
    {
      DomainException::check(*this, t);
      return integral(Interval(t));
    }

    const IntervalVector TubeVector::integral(const Interval& t) const
    {
      DomainException::check(*this, t);
      pair<IntervalVector,IntervalVector> partial_ti = partialIntegral(t);
      return IntervalVector(partial_ti.first.lb()) | partial_ti.second.ub();
    }

    const IntervalVector TubeVector::integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);
      pair<IntervalVector,IntervalVector> integral_t1 = partialIntegral(t1);
      pair<IntervalVector,IntervalVector> integral_t2 = partialIntegral(t2);
      Vector lb = (integral_t2.first - integral_t1.first).lb();
      Vector ub = (integral_t2.second - integral_t1.second).ub();
      return IntervalVector(lb) | ub;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partialIntegral(const Interval& t) const
    {
      // todo: use tree structure here
      DomainException::check(*this, t);

      Interval intv_t;
      const TubeSlice *slice = getFirstSlice();
      pair<IntervalVector,IntervalVector> p_integ
        = make_pair(IntervalVector(dim(), 0.), IntervalVector(dim(), 0.)), p_integ_uncertain(p_integ);

      while(slice != NULL && slice->domain().lb() < t.ub())
      {
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
            pair<IntervalVector,IntervalVector> p_integ_temp(p_integ_uncertain);
            p_integ_uncertain.first += Interval(0., intv_t.diam()) * slice->codomain().lb();
            p_integ_uncertain.second += Interval(0., intv_t.diam()) * slice->codomain().ub();
          
            p_integ.first |= p_integ_uncertain.first;
            p_integ.second |= p_integ_uncertain.second;

            p_integ_uncertain.first = p_integ_temp.first + intv_t.diam() * slice->codomain().lb();
            p_integ_uncertain.second = p_integ_temp.second + intv_t.diam() * slice->codomain().ub();
          }

        slice = slice->nextSlice();
      }

      return p_integ;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partialIntegral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);
      pair<IntervalVector,IntervalVector> integral_t1 = partialIntegral(t1);
      pair<IntervalVector,IntervalVector> integral_t2 = partialIntegral(t2);
      return make_pair((integral_t2.first - integral_t1.first),
                       (integral_t2.second - integral_t1.second));
    }

    // Contractors

    bool TubeVector::ctcDeriv(const TubeVector& v)
    {
      CtcDeriv ctc;
      return ctc.contract(*this, v);
    }

    bool TubeVector::ctcEval(Interval& t, IntervalVector& z, TubeVector& w)
    {
      CtcEval ctc;
      return ctc.contract(t, z, *this, w);
    }
      
    // Serialization

    void TubeVector::serialize(const string& binary_file_name, int version_number) const
    {
      vector<TrajectoryVector> v_trajs;
      serialize(binary_file_name, v_trajs, version_number);
    }

    void TubeVector::serialize(const string& binary_file_name, const TrajectoryVector& traj, int version_number) const
    {
      vector<TrajectoryVector> v_trajs;
      v_trajs.push_back(traj);
      serialize(binary_file_name, v_trajs, version_number);
    }
    
    void TubeVector::serialize(const string& binary_file_name, const vector<TrajectoryVector>& v_trajs, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serializeTubeVector(bin_file, *this, version_number);

      int nb_trajs = v_trajs.size();
      bin_file.write((const char*)&nb_trajs, sizeof(int));
      for(int i = 0 ; i < v_trajs.size() ; i++)
        serializeTrajectoryVector(bin_file, v_trajs[i], version_number);

      bin_file.close();
    }

  // Protected methods

    // Access values

    const IntervalVector TubeVector::codomainBox() const
    {
      // todo: use tree structure instead
      const TubeSlice *slice = getFirstSlice();
      IntervalVector codomain(dim(), Interval::EMPTY_SET);
      while(slice != NULL)
      {
        codomain |= slice->codomainBox();
        slice = slice->nextSlice();
      }
      return codomain;
    }

    // Integration

    // Serialization

    void TubeVector::deserialize(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::deserialize()", "error while opening file \"" + binary_file_name + "\"");

      deserializeTubeVector(bin_file, *this);

      if(!bin_file.eof())
      {
        int nb_trajs;
        bin_file.read((char*)&nb_trajs, sizeof(int));
        for(int i = 0 ; i < nb_trajs ; i++)
        {
          TrajectoryVector traj;
          deserializeTrajectoryVector(bin_file, traj);
          v_trajs.push_back(traj);
        }
      }

      bin_file.close();
    }
}