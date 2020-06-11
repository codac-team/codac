/** 
 *  TubeVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_TubeVector.h"
#include "tubex_Exception.h"
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "ibex_LargestFirst.h"
#include "tubex_serialize_trajectories.h"
#include "ibex_NoBisectableVariableException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    TubeVector::TubeVector()
    {

    }

    TubeVector::TubeVector(const Interval& tdomain, int n)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      assert(n > 0);
      assert(valid_tdomain(tdomain));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Tube(tdomain);
    }

    TubeVector::TubeVector(const Interval& tdomain, const IntervalVector& codomain)
      : TubeVector(tdomain, codomain.size())
    {
      assert(valid_tdomain(tdomain));
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& tdomain, double timestep, int n)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      assert(n > 0);
      assert(timestep >= 0.);
      assert(valid_tdomain(tdomain));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Tube(tdomain, timestep);
    }
    
    TubeVector::TubeVector(const Interval& tdomain, double timestep, const IntervalVector& codomain)
      : TubeVector(tdomain, timestep, codomain.size())
    {
      assert(timestep >= 0.);
      assert(valid_tdomain(tdomain));
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& tdomain, double timestep, const TFnc& f)
      : TubeVector(tdomain, timestep, f.image_dim())
    {
      assert(timestep >= 0.);
      assert(valid_tdomain(tdomain));
      assert(f.nb_vars() == 0 && "function's inputs must be limited to system variable");

      // A copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(*this);
      *this = f.eval_vector(input);
    }

    TubeVector::TubeVector(const std::vector<Interval>& v_tdomains, const std::vector<IntervalVector>& v_codomains)
      : m_n(v_codomains[0].size()), m_v_tubes(new Tube[m_n])
    {
      assert(v_tdomains.size() == v_codomains.size());
      assert(!v_tdomains.empty());

      vector<Interval> v_scalar_codomains[size()];

      for(size_t i = 0 ; i < v_codomains.size() ; i++)
      {
        if(i > 0) assert(v_codomains[i].size() == v_codomains[i-1].size());

        for(int j = 0 ; j < size() ; j++)
          v_scalar_codomains[j].push_back(v_codomains[i][j]);
      }

      for(int j = 0 ; j < size() ; j++)
        (*this)[j] = Tube(v_tdomains, v_scalar_codomains[j]);
    }

    TubeVector::TubeVector(initializer_list<Tube> list)
      : m_n(list.size()), m_v_tubes(new Tube[list.size()])
    {
      assert(list.size() > 0);
      std::copy(list.begin(), list.end(), m_v_tubes);
    }

    TubeVector::TubeVector(const TubeVector& x)
    {
      *this = x;
    }

    TubeVector::TubeVector(const TubeVector& x, const IntervalVector& codomain)
      : TubeVector(x)
    {
      assert(codomain.size() == x.size());
      set(codomain);
    }

    TubeVector::TubeVector(int n, const Tube& x)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      assert(n > 0);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = x;
    }

    TubeVector::TubeVector(const TrajectoryVector& traj, double timestep)
      : TubeVector(traj.tdomain(), timestep, traj.size())
    {
      assert(timestep >= 0.);
      set_empty();
      *this |= traj;
    }

    TubeVector::TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep)
      : TubeVector(lb, timestep)
    {
      assert(timestep >= 0.);
      assert(lb.tdomain() == ub.tdomain());
      assert(lb.size() == ub.size());
      *this |= ub;
    }

    TubeVector::TubeVector(const string& binary_file_name)
    {
      TrajectoryVector *traj;
      deserialize(binary_file_name, traj);
    }
    
    TubeVector::TubeVector(const string& binary_file_name, TrajectoryVector *&traj)
    {
      deserialize(binary_file_name, traj);
      if(traj == NULL)
        throw Exception("Tube constructor",
                        "unable to deserialize Trajectory object");
    }
    
    TubeVector::~TubeVector()
    {
      delete[] m_v_tubes;
    }

    const TubeVector TubeVector::primitive() const
    {
      Vector c(size(), 0.);
      return primitive(c);
    }

    const TubeVector TubeVector::primitive(const IntervalVector& c) const
    {
      TubeVector primitive(*this, IntervalVector(size())); // a copy of this initialized to nx[-oo,oo]
      primitive.set(c, primitive.tdomain().lb());
      CtcDeriv ctc_deriv;
      ctc_deriv.contract(primitive, static_cast<const TubeVector&>(*this), TimePropag::FORWARD);
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

      return *this;
    }

    const Interval TubeVector::tdomain() const
    {
      Interval t = (*this)[0].tdomain();
      for(int i = 1 ; i < size() ; i++)
        assert(t == (*this)[i].tdomain() && "all components do not have the same tdomain");
      return t;
    }

    int TubeVector::size() const
    {
      return m_n;
    }

    void TubeVector::resize(int n)
    {
      assert(n > 0);

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
      assert(start_index >= 0);
      assert(end_index < size());
      assert(start_index <= end_index);
      TubeVector subvec(tdomain(), end_index - start_index + 1);
      for(int i = 0 ; i < subvec.size() ; i++)
        subvec[i] = (*this)[i + start_index];
      return subvec;
    }
    
    void TubeVector::put(int start_index, const TubeVector& subvec)
    {
      assert(start_index >= 0);
      assert(start_index + subvec.size() <= size());
      for(int i = 0 ; i < subvec.size() ; i++)
        (*this)[i + start_index] = subvec[i];
    }
  
    // Slices structure

    int TubeVector::nb_slices() const
    {
      int n = (*this)[0].nb_slices();
      for(int i = 1 ; i < size() ; i++)
        assert(n == (*this)[i].nb_slices() && "all components do not have the same number of slices");
      return n;
    }

    int TubeVector::time_to_index(double t) const
    {
      assert(tdomain().contains(t));
      int index = (*this)[0].time_to_index(t);
      for(int i = 1 ; i < size() ; i++)
        assert((*this)[0].nb_slices() == (*this)[i].nb_slices() && "all components do not have the same number of slices");
      return index;
    }

    void TubeVector::sample(double t)
    {
      assert(tdomain().contains(t));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(t);
    }

    void TubeVector::sample(double t, const IntervalVector& gate)
    {
      assert(tdomain().contains(t));
      assert(size() == gate.size());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(t, gate[i]);
    }

    void TubeVector::sample(const Tube& x)
    {
      assert(tdomain() == x.tdomain());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(x);
    }

    void TubeVector::sample(const TubeVector& x)
    {
      assert(tdomain() == x.tdomain());
      assert(size() == x.size());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(x[i]);
    }

    // Accessing values

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
      assert(index >= 0 && index < size());
      return const_cast<Tube&>(static_cast<const TubeVector&>(*this).operator[](index));
    }

    const Tube& TubeVector::operator[](int index) const
    {
      assert(index >= 0 && index < size());
      return m_v_tubes[index];
    }

    const IntervalVector TubeVector::operator()(int slice_id) const
    {
      assert(slice_id >= 0 && slice_id < nb_slices());
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](slice_id);
      return box;
    }

    const IntervalVector TubeVector::operator()(double t) const
    {
      assert(tdomain().contains(t));
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](t);
      return box;
    }

    const IntervalVector TubeVector::operator()(const Interval& t) const
    {
      assert(tdomain().is_superset(t));
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](t);
      return box;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::eval(const Interval& t) const
    {
      assert(tdomain().is_superset(t));

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
      assert(tdomain().contains(t));
      assert(size() == v.size());
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      // In Tube class, Tube::interpol(double, Tube) faster than Tube::interpol(Interval, Tube)
      IntervalVector eval(size());
      for(int i = 0 ; i < size() ; i++)
        eval[i] = (*this)[i].interpol(t, v[i]);
      return eval;
    }

    const IntervalVector TubeVector::interpol(const Interval& t, const TubeVector& v) const
    {
      assert(tdomain().is_superset(t));
      assert(size() == v.size());
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      IntervalVector eval(size());
      for(int i = 0 ; i < size() ; i++)
        eval[i] = (*this)[i].interpol(t, v[i]);
      return eval;
    }

    #define macro_invert_union(invert_method, slice_deriv_init_fwd, slice_deriv_init_bwd, slice_deriv_iter_fwd, slice_deriv_iter_bwd) \
      assert(size() == y.size()); \
      assert(search_tdomain.intersects(tdomain())); \
       \
      /* The method finds the lower bound in forward, */ \
      /* then the upper one in backward, */ \
      /* and returns their union. */ \
       \
      Interval restricted_tdomain = tdomain() & search_tdomain; \
       \
      const Slice **v_s = new const Slice*[size()]; \
      const Slice **v_v = new const Slice*[size()]; \
      for(int i = 0 ; i < size() ; i++) \
      { \
        v_s[i] = (*this)[i].slice(restricted_tdomain.lb()); \
        slice_deriv_init_fwd; \
      } \
       \
      Interval inversion_lb = Interval::EMPTY_SET; \
      Interval inversion_ub = Interval::EMPTY_SET; \
       \
      while(v_s[0] != NULL \
        && v_s[0]->tdomain().lb() < restricted_tdomain.ub() \
        && inversion_lb.is_empty()) \
      { \
        inversion_lb = v_s[0]->tdomain(); \
        for(int i = 0 ; i < size() && !inversion_lb.is_empty() ; i++) \
          inversion_lb &= invert_method; \
         \
        for(int i = 0 ; i < size() ; i++) \
        { \
          v_s[i] = v_s[i]->next_slice(); \
          slice_deriv_iter_fwd; \
        } \
      } \
       \
      for(int i = 0 ; i < size() ; i++) \
      { \
        v_s[i] = (*this)[i].slice(restricted_tdomain.ub()); \
        slice_deriv_init_bwd; \
      } \
       \
      while(v_s[0] != NULL \
        && v_s[0]->tdomain().ub() > restricted_tdomain.lb() \
        && inversion_ub.is_empty()) \
      { \
        inversion_ub = v_s[0]->tdomain(); \
        for(int i = 0 ; i < size() && !inversion_ub.is_empty() ; i++) \
          inversion_ub &= invert_method; \
         \
        for(int i = 0 ; i < size() ; i++) \
        { \
          v_s[i] = v_s[i]->prev_slice(); \
          slice_deriv_iter_bwd; \
        } \
      } \
       \
      delete[] v_s; \
      delete[] v_v; \
       \
      return inversion_lb | inversion_ub; \

    const Interval TubeVector::invert(const IntervalVector& y, const Interval& search_tdomain) const
    {
      macro_invert_union(
        v_s[i]->invert(y[i], restricted_tdomain), 
        {}, {}, {}, {});
    }

    const Interval TubeVector::invert(const IntervalVector& y, const TubeVector& v, const Interval& search_tdomain) const
    {
      assert(v.size() == y.size());
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      macro_invert_union(
        v_s[i]->invert(y[i], *v_v[i], restricted_tdomain), 
        v_v[i] = v[i].slice(restricted_tdomain.lb()),
        v_v[i] = v[i].slice(restricted_tdomain.ub()),
        v_v[i] = v_v[i]->next_slice(),
        v_v[i] = v_v[i]->prev_slice());
    }

    #define macro_invert_subsets(invert_method, slice_deriv_init, slice_deriv_iter) \
      assert(size() == y.size()); \
      assert(search_tdomain.intersects(tdomain())); \
       \
      v_t.clear(); \
      Interval restricted_tdomain = tdomain() & search_tdomain; \
       \
      const Slice **v_s = new const Slice*[size()]; \
      const Slice **v_v = new const Slice*[size()]; \
      for(int i = 0 ; i < size() ; i++) \
      { \
        v_s[i] = (*this)[i].slice(restricted_tdomain.lb()); \
        slice_deriv_init; \
      } \
       \
      Interval ti = Interval::EMPTY_SET; \
       \
      while(v_s[0] != NULL && v_s[0]->tdomain().lb() < restricted_tdomain.ub()) \
      { \
        Interval inversion; \
        for(int i = 0 ; i < size() && !inversion.is_empty() ; i++) \
          inversion &= invert_method; \
         \
        ti |= inversion; \
         \
        if(inversion.is_empty() && !ti.is_empty()) \
        { \
          v_t.push_back(ti); \
          ti.set_empty(); \
        } \
         \
        for(int i = 0 ; i < size() ; i++) \
        { \
          v_s[i] = v_s[i]->next_slice(); \
          slice_deriv_iter; \
        } \
      } \
       \
      if(!ti.is_empty()) \
        v_t.push_back(ti); \
       \
      delete[] v_s; \
      delete[] v_v; \

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_tdomain) const
    {
      macro_invert_subsets(
        v_s[i]->invert(y[i], restricted_tdomain), 
        {}, 
        {});
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const TubeVector& v, const Interval& search_tdomain) const
    {
      assert(v.size() == y.size());
      assert(tdomain() == v.tdomain());
      assert(same_slicing(*this, v));

      macro_invert_subsets(
        v_s[i]->invert(y[i], *v_v[i], restricted_tdomain), 
        v_v[i] = v[i].slice(restricted_tdomain.lb()),
        v_v[i] = v_v[i]->next_slice());
    }

    const TrajectoryVector TubeVector::diam(bool gates_thicknesses) const
    {
      TrajectoryVector thickness(size());
      for(int i = 0 ; i < size() ; i++)
        thickness[i] = (*this)[i].diam(gates_thicknesses);
      return thickness;
    }

    const TrajectoryVector TubeVector::diam(const TubeVector& v) const
    {
      TrajectoryVector thickness(size());
      for(int i = 0 ; i < size() ; i++)
        thickness[i] = (*this)[i].diam(v[i]);
      return thickness;
    }

    const Vector TubeVector::max_diam() const
    {
      Vector thickness(size());
      for(int i = 0 ; i < size() ; i++)
        thickness[i] = (*this)[i].max_diam();
      return thickness;
    }

    const Trajectory TubeVector::diag(bool gates_thicknesses) const
    {
      return diag(0, size()-1, gates_thicknesses);
    }

    const Trajectory TubeVector::diag(int start_index, int end_index, bool gates_thicknesses) const
    {
      assert(start_index >= 0);
      assert(end_index < size());
      assert(start_index <= end_index);

      Trajectory diag_traj;
      TrajectoryVector diams = diam(gates_thicknesses);

      for(map<double,double>::const_iterator it = diams[0].sampled_map().begin() ; it != diams[0].sampled_map().end() ; it++)
      {
        double diag = 0.;
        for(int i = start_index ; i <= end_index ; i++)
          diag += std::pow(it->second, 2);
        diag_traj.set(std::sqrt(diag), it->first);
      }

      return diag_traj;
    }

    // Tests

    bool TubeVector::operator==(const TubeVector& x) const
    {
      if(size() != x.size())
        return false;

      for(int i = 0 ; i < size() ; i++)
        if((*this)[i] != x[i])
          return false;

      return true;
    }

    bool TubeVector::operator!=(const TubeVector& x) const
    {
      if(size() != x.size())
        return true;

      for(int i = 0 ; i < size() ; i++)
        if((*this)[i] != x[i])
          return true;

      return false;
    }

    #define macro_sets_comparison(f) \
      \
      assert(size() == x.size()); \
      for(int i = 0 ; i < size() ; i++) \
        if(!(*this)[i].f(x[i])) \
          return false; \
      return true; \

    bool TubeVector::is_subset(const TubeVector& x) const
    {
      macro_sets_comparison(is_subset);
    }

    bool TubeVector::is_strict_subset(const TubeVector& x) const
    {
      return is_subset(x) && *this != x;
    }

    bool TubeVector::is_interior_subset(const TubeVector& x) const
    {
      macro_sets_comparison(is_interior_subset);
    }

    bool TubeVector::is_strict_interior_subset(const TubeVector& x) const
    {
      return is_interior_subset(x) && *this != x;
    }

    bool TubeVector::is_superset(const TubeVector& x) const
    {
      macro_sets_comparison(is_superset);
    }

    bool TubeVector::is_strict_superset(const TubeVector& x) const
    {
      return is_superset(x) && *this != x;
    }

    bool TubeVector::is_empty() const
    {
      for(int i = 0 ; i < size() ; i++)
        if((*this)[i].is_empty())
          return true;
      return false;
    }

    const BoolInterval TubeVector::contains(const TrajectoryVector& x) const
    {
      assert(size() == x.size());

      BoolInterval result = YES;
      for(int i = 0 ; i < size() ; i++)
      {
        BoolInterval b = (*this)[i].contains(x[i]);
        if(b == NO) return NO;
        else if(b == MAYBE) result = MAYBE;
      }

      return result;
    }

    bool TubeVector::overlaps(const TubeVector& x, float ratio) const
    {
      assert(tdomain() == x.tdomain());
      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].overlaps(x[i], ratio))
          return false;
      return true;
    }

    // Setting values

    void TubeVector::set(const IntervalVector& y)
    {
      assert(size() == y.size());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i]);
    }

    void TubeVector::set(const IntervalVector& y, int slice_id)
    {
      assert(size() == y.size());
      assert(slice_id >= 0 && slice_id < nb_slices());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], slice_id);
    }

    void TubeVector::set(const IntervalVector& y, double t)
    {
      assert(size() == y.size());
      assert(tdomain().contains(t));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], t);
    }

    void TubeVector::set(const IntervalVector& y, const Interval& t)
    {
      assert(size() == y.size());
      assert(tdomain().is_superset(t));
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
      return inflate(Vector(size(),rad));
    }

    const TubeVector& TubeVector::inflate(const Vector& rad)
    {
      assert(size() == rad.size());

      for(int i = 0 ; i < size() ; i++)
      {
        assert(rad[i] >= 0.);
        (*this)[i].inflate(rad[i]);
      }

      return *this;
    }

    const TubeVector& TubeVector::inflate(const TrajectoryVector& rad)
    {
      assert(size() == rad.size());
      assert(tdomain() == rad.tdomain());
      assert(rad.codomain().is_subset(IntervalVector(rad.size(), Interval::POS_REALS))
        && "positive TrajectoryVector");
      
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].inflate(rad[i]);

      return *this;
    }

    void TubeVector::shift_tdomain(double shift_ref)
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].shift_tdomain(shift_ref);
    }

    // Bisection
    
    const pair<TubeVector,TubeVector> TubeVector::bisect(double t, float ratio) const
    {
      assert(tdomain().contains(t));
      assert(Interval(0.,1.).interior_contains(ratio));

      pair<TubeVector,TubeVector> p = make_pair(*this,*this);
      LargestFirst bisector(0., ratio);

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
    
    const pair<TubeVector,TubeVector> TubeVector::bisect(double t, int dim_id, float ratio) const
    {
      assert(tdomain().contains(t));
      assert(Interval(0.,1.).interior_contains(ratio));
      assert(dim_id >= 0 && dim_id < size());

      pair<TubeVector,TubeVector> p = make_pair(*this,*this);
      LargestFirst bisector(0., ratio);

      try
      {
        // Note: LargestFirst is useless here (bisection on one-dimensional interval vector)
        pair<IntervalVector,IntervalVector> p_codomain = bisector.bisect(IntervalVector(1, (*this)[dim_id](t)));
        p.first[dim_id].set(p_codomain.first[0], t);
        p.second[dim_id].set(p_codomain.second[0], t);
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
      str << x.class_name() << " (dim " << x.size() << ") "
          << x.tdomain() << "↦" << x.codomain_box()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Tree synthesis structure

    void TubeVector::enable_synthesis(bool enable) const
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].enable_synthesis(enable);
    }

    // Integration

    const IntervalVector TubeVector::integral(double t) const
    {
      assert(tdomain().contains(t));
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t);
      return integ_box;
    }

    const IntervalVector TubeVector::integral(const Interval& t) const
    {
      assert(tdomain().is_superset(t));
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t);
      return integ_box;
    }

    const IntervalVector TubeVector::integral(const Interval& t1, const Interval& t2) const
    {
      assert(tdomain().is_superset(t1));
      assert(tdomain().is_superset(t2));
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t1, t2);
      return integ_box;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t) const
    {
      assert(tdomain().is_superset(t));
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
      assert(tdomain().is_superset(t1));
      assert(tdomain().is_superset(t2));
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
      
    // Serialization

    void TubeVector::serialize(const string& binary_file_name, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serialize_TubeVector(bin_file, *this, version_number);
      bin_file.close();
    }

    void TubeVector::serialize(const string& binary_file_name, const TrajectoryVector& traj, int version_number) const
    {
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serialize_TubeVector(bin_file, *this, version_number);
      char c = 0; bin_file.write(&c, 1); // writing a bit to separate the two objects
      serialize_TrajectoryVector(bin_file, traj, version_number);
      bin_file.close();
    }
    
    bool TubeVector::same_slicing(const TubeVector& x1, const Tube& x2)
    {
      for(int i = 0 ; i < x1.size() ; i++)
        if(!Tube::same_slicing(x1[i], x2))
          return false;
      return true;
    }
    
    bool TubeVector::same_slicing(const TubeVector& x1, const TubeVector& x2)
    {
      assert(x1.size() == x2.size());
      for(int i = 0 ; i < x1.size() ; i++)
        if(!Tube::same_slicing(x1[i], x2[i]))
          return false;
      return true;
    }

    const TubeVector TubeVector::hull(const list<TubeVector>& l_tubes)
    {
      assert(!l_tubes.empty());
      list<TubeVector>::const_iterator it = l_tubes.begin();
      TubeVector hull = *it;
      for(++it ; it != l_tubes.end() ; ++it)
        hull |= *it;
      return hull;
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

    void TubeVector::deserialize(const string& binary_file_name, TrajectoryVector *&traj)
    {
      ifstream bin_file(binary_file_name.c_str(), ios::in | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::deserialize()", "error while opening file \"" + binary_file_name + "\"");
      
      TubeVector *ptr;
      deserialize_TubeVector(bin_file, ptr);
      *this = *ptr;
      
      char c; bin_file.get(c); // reading a bit of separation

      if(!bin_file.eof())
        deserialize_TrajectoryVector(bin_file, traj);

      else
        traj = NULL;

      delete ptr;
      bin_file.close();
    }
}