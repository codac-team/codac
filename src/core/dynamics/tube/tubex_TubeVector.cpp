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

    TubeVector::TubeVector()
    {
      // todo: remove this?
    }

    TubeVector::TubeVector(const Interval& domain, int n)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      DomainException::check(domain);
      DimensionException::check(n);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Tube(domain);
    }

    TubeVector::TubeVector(const Interval& domain, const IntervalVector& codomain)
      : TubeVector(domain, codomain.size())
    {
      DomainException::check(domain);
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, int n)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      DimensionException::check(n);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Tube(domain, timestep);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const IntervalVector& codomain)
      : TubeVector(domain, timestep, codomain.size())
    {
      DomainException::check(domain);
      DomainException::check(timestep);
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const tubex::Fnc& f)
      : TubeVector(domain, timestep, f.image_dim())
    {
      // todo: check scalar f, image dim
      DomainException::check(domain);
      DomainException::check(timestep);

      if(f.nb_vars() != 0)
        throw Exception("TubeVector constructor",
                        "function's inputs not limited to system variable");
      // A copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(*this);
      *this = f.eval_vector(input);
      // todo: delete something here?
    }

    TubeVector::TubeVector(const TubeVector& x)
    {
      *this = x;
    }

    TubeVector::TubeVector(const TubeVector& x, const IntervalVector& codomain)
      : TubeVector(x)
    {
      set(codomain);
    }

    TubeVector::TubeVector(const Tube& x)
      : TubeVector(1, x)
    {
      
    }

    TubeVector::TubeVector(int n, const Tube& x)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = x;
    }

    TubeVector::TubeVector(const TrajectoryVector& traj, double timestep)
      : TubeVector(traj.domain(), timestep, traj.size())
    {
      DomainException::check(timestep);
      set_empty();
      *this |= traj;
    }

    TubeVector::TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep)
      : TubeVector(lb, timestep)
    {
      DomainException::check(timestep);
      DimensionException::check(lb, ub);
      *this |= ub;
    }

    TubeVector::TubeVector(const string& binary_file_name)
    {
      // todo
    }
    
    TubeVector::TubeVector(const string& binary_file_name, TrajectoryVector& traj)
    {
      // todo
    }

    TubeVector::TubeVector(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {
      // todo
    }
    
    TubeVector::~TubeVector()
    {
      delete[] m_v_tubes;
    }

    const TubeVector TubeVector::primitive() const
    {
      TubeVector primitive(*this, IntervalVector(size())); // a copy of this initialized to nx[-oo,oo]
      primitive.set(Vector(size(), 0.), primitive.domain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, static_cast<const TubeVector&>(*this), FORWARD);
      return primitive;
    }

    const TubeVector& TubeVector::operator=(const TubeVector& x)
    {
      { // Destroying already existing components
        if(m_v_tubes != NULL)
          delete[] m_v_tubes;
      }

      m_n = x.size();
      m_v_tubes = new Tube[m_n];

      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = x[i]; // copy of each component
    }

    const Interval TubeVector::domain() const
    {
      // All components share the same domain
      return (*this)[0].domain();
    }

    int TubeVector::size() const
    {
      return m_n;
    }

    void TubeVector::resize(int n)
    {
      // todo: check n

      if(n == size())
        return;

      Tube *new_vec = new Tube[n];

      int i = 0;
      for(; i < size() && i < n ; i++)
        new_vec[i] = m_v_tubes[i];

      for(; i < n ; i++)
        new_vec[i] = Tube(m_v_tubes[0], Interval::ALL_REALS); // same slicing is used

      if(m_v_tubes != NULL) // (m_v_tubes == NULL) may happen when default constructor is used
        delete[] m_v_tubes;

      m_n = n;
      m_v_tubes = new_vec;
    }
    
    const TubeVector TubeVector::subvector(int start_index, int end_index) const
    {
      // todo: check start_index, end_index
      TubeVector subvec(domain(), end_index - start_index + 1);
      for(int i = 0 ; i < subvec.size() ; i++)
        subvec[i] = (*this)[i + start_index];
      return subvec;
    }
    
    void TubeVector::put(int start_index, const TubeVector& subvec)
    {
      // todo: check size subvec
      // todo: check structure tubes
      for(int i = 0 ; i < subvec.size() ; i++)
        (*this)[i + start_index] = subvec[i];
    }
  
    // Slices structure

    int TubeVector::nb_slices() const
    {
      // All components share the same slicing
      return (*this)[0].nb_slices();
    }

    int TubeVector::input2index(double t) const
    {
      DomainException::check(*this, t);
      // All components share the same slicing
      return (*this)[0].input2index(t);
    }

    void TubeVector::sample(double t)
    {
      DomainException::check(*this, t);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(t);
    }

    void TubeVector::sample(double t, const IntervalVector& gate)
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, gate);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(t, gate[i]);
    }
    
    bool TubeVector::share_same_slicing(const Tube& x1, const TubeVector& x2)
    {
      return Tube::share_same_slicing(x1, x2[0]);
    }
    
    bool TubeVector::share_same_slicing(const TubeVector& x1, const TubeVector& x2)
    {
      return Tube::share_same_slicing(x1[0], x2[0]);
    }

    // Access values

    const IntervalVector TubeVector::codomain() const
    {
      return codomain_box();
    }

    double TubeVector::volume() const
    {
      double vol = 0.;
      for(int i = 0 ; i < size() ; i++)
        vol += (*this)[i].volume();
      return vol;
    }

    Tube& TubeVector::operator[](int index)
    {
      // todo: check index
      return const_cast<Tube&>(static_cast<const TubeVector&>(*this).operator[](index));
      // todo: check quickness of cast instead of direct access to m_v_tubes[]
    }

    const Tube& TubeVector::operator[](int index) const
    {
      // todo: check index
      return m_v_tubes[index];
    }

    const IntervalVector TubeVector::operator()(int slice_id) const
    {
      SlicingException::check(*this, slice_id);
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](slice_id);
      return box;
    }

    const IntervalVector TubeVector::operator()(double t) const
    {
      DomainException::check(*this, t);
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](t);
      return box;
    }

    const IntervalVector TubeVector::operator()(const Interval& t) const
    {
      DomainException::check(*this, t);
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](t);
      return box;
    }

    const Interval TubeVector::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      Interval t;
      for(int i = 0 ; i < size() ; i++)
        t &= (*this)[i].invert(y[i], search_domain);
      return t;
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      // todo
    }

    const Interval TubeVector::invert(const IntervalVector& y, const TubeVector& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      SlicingException::check(*this, v);
      DomainException::check(*this, v);
      // todo
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const TubeVector& v, const Interval& search_domain) const
    {
      DimensionException::check(*this, y);
      // todo
    }

    const pair<IntervalVector,IntervalVector> TubeVector::eval(const Interval& t) const
    {
      pair<IntervalVector,IntervalVector> p_eval
        = make_pair(IntervalVector(size()), IntervalVector(size()));

      for(int i = 0 ; i < size() ; i++)
      {
        pair<Interval,Interval> p_eval_i = (*this)[i].eval(t);
        p_eval.first[i] = p_eval_i.first;
        p_eval.second[i] = p_eval_i.second;
      }

      return p_eval;
    }

    const IntervalVector TubeVector::interpol(double t, const TubeVector& v) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, v);
      SlicingException::check(*this, v);

      // In Tube class, Tube::interpol(double, Tube) faster than Tube::interpol(Interval, Tube)
      IntervalVector eval(size());
      for(int i = 0 ; i < size() ; i++)
        eval[i] = (*this)[i].interpol(t, v[i]);
      return eval;
    }

    const IntervalVector TubeVector::interpol(const Interval& t, const TubeVector& v) const
    {
      DomainException::check(*this, t);
      DimensionException::check(*this, v);
      SlicingException::check(*this, v);

      IntervalVector eval(size());
      for(int i = 0 ; i < size() ; i++)
        eval[i] = (*this)[i].interpol(t, v[i]);
      return eval;
    }

    const Vector TubeVector::max_thickness() const
    {
      Vector thickness(size());
      for(int i = 0 ; i < size() ; i++)
        thickness[i] = (*this)[i].max_thickness();
      return thickness;
    }

    const Vector TubeVector::max_gate_thickness() const
    {
      double t; // unused
      Vector thickness(size());
      for(int i = 0 ; i < size() ; i++)
        thickness[i] = (*this)[i].max_gate_thickness(t);
      return thickness;
    }

    // Tests

    bool TubeVector::operator==(const TubeVector& x) const
    {
      if(size() != x.size())
        return false;

      SlicingException::check(*this, x);

      for(int i = 0 ; i < size() ; i++)
        if((*this)[i] != x[i])
          return false;

      return true;
    }

    bool TubeVector::operator!=(const TubeVector& x) const
    {
      if(size() != x.size())
        return true;

      SlicingException::check(*this, x);

      for(int i = 0 ; i < size() ; i++)
        if((*this)[i] != x[i])
          return true;

      return false;
    }

    bool TubeVector::is_subset(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      SlicingException::check(*this, x);

      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].is_subset(x[i]))
          return false;
      return true;
    }

    bool TubeVector::is_strict_subset(const TubeVector& x) const
    {
      DimensionException::check(*this, x);
      SlicingException::check(*this, x);

      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].is_strict_subset(x[i]))
          return false;
      return true;
    }

    bool TubeVector::is_empty() const
    {
      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].is_empty())
          return false;
      return true;
    }

    bool TubeVector::encloses(const TrajectoryVector& x) const
    {
      DimensionException::check(*this, x);
      
      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].encloses(x[i]))
          return false;
      return true;
    }

    // Setting values

    void TubeVector::set(const IntervalVector& y)
    {
      DimensionException::check(*this, y);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i]);
    }

    void TubeVector::set(const IntervalVector& y, int slice_id)
    {
      DimensionException::check(*this, y);
      SlicingException::check(*this, slice_id);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], slice_id);
    }

    void TubeVector::set(const IntervalVector& y, double t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], t);
    }

    void TubeVector::set(const IntervalVector& y, const Interval& t)
    {
      DimensionException::check(*this, y);
      DomainException::check(*this, t);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], t);
    }

    void TubeVector::set_empty()
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set_empty();
    }

    const TubeVector& TubeVector::inflate(double rad)
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].inflate(rad);
    }

    const TubeVector& TubeVector::inflate(const TrajectoryVector& rad)
    {
      DimensionException::check(*this, rad);
      DomainException::check(*this, rad);
      // todo: for(int i = 0 ; i < size() ; i++)
      // todo:   (*this)[i].inflate(rad[i]);
    }

    // Bisection
    
    const pair<TubeVector,TubeVector> TubeVector::bisect(double t, float ratio) const
    {
      DomainException::check(*this, t);
      pair<TubeVector,TubeVector> p = make_pair(*this,*this);

      LargestFirst bisector(0., ratio); // todo: bisect according to another rule than largest first?

      try
      {
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect((*this)(t));
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
      str << x.class_name() << " " << x.domain() << "↦" << x.codomain_box()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Integration

    const IntervalVector TubeVector::integral(double t) const
    {
      DomainException::check(*this, t);

      // In Tube class, Tube::integral(double) faster than Tube::integral(Interval)
      // todo: deprecated comment
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t);
      return integ_box;
    }

    const IntervalVector TubeVector::integral(const Interval& t) const
    {
      DomainException::check(*this, t);
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t);
      return integ_box;
    }

    const IntervalVector TubeVector::integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t1, t2);
      return integ_box;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t) const
    {
      DomainException::check(*this, t);
      pair<IntervalVector,IntervalVector> p_integ
        = make_pair(IntervalVector(size()), IntervalVector(size()));
      for(int i = 0 ; i < size() ; i++)
      {
        pair<Interval,Interval> p_integ_i = (*this)[i].partial_integral(t);
        p_integ.first[i] = p_integ_i.first;
        p_integ.second[i] = p_integ_i.second;
      }
      return p_integ;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t1, const Interval& t2) const
    {
      DomainException::check(*this, t1);
      DomainException::check(*this, t2);
      pair<IntervalVector,IntervalVector> p_integ
        = make_pair(IntervalVector(size()), IntervalVector(size()));
      for(int i = 0 ; i < size() ; i++)
      {
        pair<Interval,Interval> p_integ_i = (*this)[i].partial_integral(t1, t2);
        p_integ.first[i] = p_integ_i.first;
        p_integ.second[i] = p_integ_i.second;
      }
      return p_integ;
    }

    // Contractors

    bool TubeVector::ctc_deriv(const TubeVector& v)
    {
      // todo: remove this?
    }

    bool TubeVector::ctc_eval(Interval& t, IntervalVector& z, TubeVector& w)
    {
      // todo: remove this?
    }
      
    // Serialization

    void TubeVector::serialize(const string& binary_file_name, int version_number) const
    {
      // todo
    }

    void TubeVector::serialize(const string& binary_file_name, const TrajectoryVector& traj, int version_number) const
    {
      // todo
    }
    
    void TubeVector::serialize(const string& binary_file_name, const vector<TrajectoryVector>& v_trajs, int version_number) const
    {
      // todo
    }

  // Protected methods

    // Access values

    const IntervalVector TubeVector::codomain_box() const
    {
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i].codomain();
      return box;
    }

    // Integration

    // Serialization

    void TubeVector::deserialize(const string& binary_file_name, vector<TrajectoryVector>& v_trajs)
    {
      // todo
    }
}