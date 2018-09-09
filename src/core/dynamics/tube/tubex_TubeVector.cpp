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
#include "tubex_CtcDeriv.h"
#include "tubex_CtcEval.h"
#include "tubex_arithmetic.h"
#include "ibex_LargestFirst.h"
#include "tubex_serializ_trajectories.h"
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

    TubeVector::TubeVector(const Interval& domain, int n)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      assert(n > 0);
      assert(valid_domain(domain));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Tube(domain);
    }

    TubeVector::TubeVector(const Interval& domain, const IntervalVector& codomain)
      : TubeVector(domain, codomain.size())
    {
      assert(valid_domain(domain));
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, int n)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      assert(n > 0);
      assert(timestep >= 0.);
      assert(valid_domain(domain));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Tube(domain, timestep);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const IntervalVector& codomain)
      : TubeVector(domain, timestep, codomain.size())
    {
      assert(timestep >= 0.);
      assert(valid_domain(domain));
      set(codomain);
    }
    
    TubeVector::TubeVector(const Interval& domain, double timestep, const tubex::Fnc& f)
      : TubeVector(domain, timestep, f.image_dim())
    {
      assert(timestep >= 0.);
      assert(valid_domain(domain));
      assert(f.nb_vars() == 0 && "function's inputs must be limited to system variable");

      // A copy of this is sent anyway in order to know the data structure to produce
      TubeVector input(*this);
      *this = f.eval_vector(input);
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

    TubeVector::TubeVector(int n, const Tube& x)
      : m_n(n), m_v_tubes(new Tube[n])
    {
      assert(n > 0);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = x;
    }

    TubeVector::TubeVector(const TrajectoryVector& traj, double timestep)
      : TubeVector(traj.domain(), timestep, traj.size())
    {
      assert(timestep >= 0.);
      set_empty();
      *this |= traj;
    }

    TubeVector::TubeVector(const TrajectoryVector& lb, const TrajectoryVector& ub, double timestep)
      : TubeVector(lb, timestep)
    {
      assert(timestep >= 0.);
      assert(lb.domain() == ub.domain());
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

      return *this;
    }

    const Interval TubeVector::domain() const
    {
      Interval t = (*this)[0].domain();
      for(int i = 1 ; i < size() ; i++)
        assert(t == (*this)[i].domain() && "all components do not have the same definition domain");
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
      TubeVector subvec(domain(), end_index - start_index + 1);
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

    int TubeVector::input2index(double t) const
    {
      assert(domain().contains(t));
      int index = (*this)[0].input2index(t);
      for(int i = 1 ; i < size() ; i++)
        assert((*this)[0].nb_slices() == (*this)[i].nb_slices() && "all components do not have the same number of slices");
      return index;
    }

    void TubeVector::sample(double t)
    {
      assert(domain().contains(t));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(t);
    }

    void TubeVector::sample(double t, const IntervalVector& gate)
    {
      assert(domain().contains(t));
      assert(size() == gate.size());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(t, gate[i]);
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
      assert(domain().contains(t));
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](t);
      return box;
    }

    const IntervalVector TubeVector::operator()(const Interval& t) const
    {
      assert(domain().is_superset(t));
      IntervalVector box(size());
      for(int i = 0 ; i < size() ; i++)
        box[i] = (*this)[i](t);
      return box;
    }

    const Interval TubeVector::invert(const IntervalVector& y, const Interval& search_domain) const
    {
      assert(size() == y.size());
      Interval t;
      for(int i = 0 ; i < size() ; i++)
        t &= (*this)[i].invert(y[i], search_domain);
      return t;
    }

    #define macro_invert(invert_method) \
      assert(size() == y.size()); \
       \
      v_t.clear(); \
      for(int i = 0 ; i < size() ; i++) \
      { \
        vector<Interval> v_t_i, v_t_new; \
        invert_method; \
        \
        if(i > 0) \
        { \
          v_t = v_t_i; \
          continue; \
        } \
        \
        for(int a = 0 ; a < v_t.size() ; a++) \
          for(int b = 0 ; b < v_t_i.size() ; b++) \
          { \
            Interval t = v_t[a] & v_t_i[b]; \
            if(!t.is_empty()) \
              v_t_new.push_back(t); \
          } \
        \
        v_t = v_t_new; \
      } \
      /* todo: merge v_t items? */ \

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const Interval& search_domain) const
    {
      assert(size() == y.size());
      macro_invert((*this)[i].invert(y[i], v_t_i, search_domain));
    }

    const Interval TubeVector::invert(const IntervalVector& y, const TubeVector& v, const Interval& search_domain) const
    {
      assert(size() == y.size());
      assert(size() == v.size());
      assert(domain() == v.domain());
      assert(same_slicing(*this, v));

      Interval t;
      for(int i = 0 ; i < size() ; i++)
        t &= (*this)[i].invert(y[i], v[i], search_domain);
      return t;
    }

    void TubeVector::invert(const IntervalVector& y, vector<Interval> &v_t, const TubeVector& v, const Interval& search_domain) const
    {
      assert(size() == v.size());
      assert(domain() == v.domain());
      assert(same_slicing(*this, v));
      macro_invert((*this)[i].invert(y[i], v_t_i, v[i], search_domain));
    }

    const pair<IntervalVector,IntervalVector> TubeVector::eval(const Interval& t) const
    {
      assert(domain().is_superset(t));

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
      assert(domain().contains(t));
      assert(size() == v.size());
      assert(domain() == v.domain());
      assert(same_slicing(*this, v));

      // In Tube class, Tube::interpol(double, Tube) faster than Tube::interpol(Interval, Tube)
      IntervalVector eval(size());
      for(int i = 0 ; i < size() ; i++)
        eval[i] = (*this)[i].interpol(t, v[i]);
      return eval;
    }

    const IntervalVector TubeVector::interpol(const Interval& t, const TubeVector& v) const
    {
      assert(domain().is_superset(t));
      assert(size() == v.size());
      assert(domain() == v.domain());
      assert(same_slicing(*this, v));

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

    bool TubeVector::contains(const TrajectoryVector& x) const
    {
      assert(size() == x.size());
      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].contains(x[i]))
          return false;
      return true;
    }

    bool TubeVector::overlaps(const TubeVector& x, float ratio) const
    {
      assert(domain() == x.domain());
      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].overlaps(x[i]))
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
      assert(domain().contains(t));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], t);
    }

    void TubeVector::set(const IntervalVector& y, const Interval& t)
    {
      assert(size() == y.size());
      assert(domain().is_superset(t));
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], t);
    }

    void TubeVector::set_empty()
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set_empty();
    }

    const TubeVector& TubeVector::inflate(const Vector& rad)
    {
      assert(size() && rad.size());
      for(int i = 0 ; i < size() ; i++)
      {
        assert(rad[i] >= 0.);
        (*this)[i].inflate(rad[i]);
      }
    }

    const TubeVector& TubeVector::inflate(const TrajectoryVector& rad)
    {
      assert(size() == rad.size());
      assert(domain() == rad.domain());
      assert(rad.codomain().is_subset(IntervalVector(rad.size(), Interval::POS_REALS))
        && "positive TrajectoryVector");
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].inflate(rad[i]);
    }

    // Bisection
    
    const pair<TubeVector,TubeVector> TubeVector::bisect(double t, float ratio) const
    {
      assert(domain().contains(t));
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

    // String
    
    ostream& operator<<(ostream& str, const TubeVector& x)
    {
      str << x.class_name() << " " << x.domain() << "↦" << x.codomain_box()
          << ", " << x.nb_slices()
          << " slice" << (x.nb_slices() > 1 ? "s" : "")
          << flush;
      return str;
    }

    // Static methods

    const TubeVector TubeVector::hull(const list<TubeVector>& l_tubes)
    {
      assert(!l_tubes.empty());
      list<TubeVector>::const_iterator it = l_tubes.begin();
      TubeVector hull = *it;
      for(++it ; it != l_tubes.end() ; ++it)
        hull |= *it;
      return hull;
    }

    // Integration

    const IntervalVector TubeVector::integral(double t) const
    {
      assert(domain().contains(t));
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t);
      return integ_box;
    }

    const IntervalVector TubeVector::integral(const Interval& t) const
    {
      assert(domain().is_superset(t));
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t);
      return integ_box;
    }

    const IntervalVector TubeVector::integral(const Interval& t1, const Interval& t2) const
    {
      assert(domain().is_superset(t1));
      assert(domain().is_superset(t2));
      IntervalVector integ_box(size());
      for(int i = 0 ; i < size() ; i++)
        integ_box[i] = (*this)[i].integral(t1, t2);
      return integ_box;
    }

    const pair<IntervalVector,IntervalVector> TubeVector::partial_integral(const Interval& t) const
    {
      assert(domain().is_superset(t));
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
      assert(domain().is_superset(t1));
      assert(domain().is_superset(t2));
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
      assert(size() == traj.size());
      ofstream bin_file(binary_file_name.c_str(), ios::out | ios::binary);

      if(!bin_file.is_open())
        throw Exception("TubeVector::serialize()", "error while writing file \"" + binary_file_name + "\"");

      serialize_TubeVector(bin_file, *this, version_number);
      char c; bin_file.write(&c, 1); // writing a bit to separate the two objects
      serialize_TrajectoryVector(bin_file, traj, version_number);
      bin_file.close();
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

      bin_file.close();
    }
}