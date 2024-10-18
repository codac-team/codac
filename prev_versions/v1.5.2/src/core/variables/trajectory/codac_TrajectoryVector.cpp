/** 
 *  TrajectoryVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include "codac_TrajectoryVector.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Public methods

    // Definition

    TrajectoryVector::TrajectoryVector()
    {
      
    }

    TrajectoryVector::TrajectoryVector(int n)
      : m_n(n), m_v_trajs(new Trajectory[n])
    {
      assert(n > 0);
    }

    TrajectoryVector::TrajectoryVector(const Interval& tdomain, const TFunction& f)
      : TrajectoryVector(f.image_dim())
    {
      assert(valid_tdomain(tdomain));
      assert(f.nb_var() == 0 && "function's inputs must be limited to system variable");
      // todo: check thickness of f? (only thin functions should be allowed)

      // Setting values for each component
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Trajectory(tdomain, f[i]);
    }

    TrajectoryVector::TrajectoryVector(const Interval& tdomain, const TFunction& f, double timestep)
      : TrajectoryVector(tdomain, f)
    {
      assert(timestep > 0.);
      sample(timestep);
    }

    TrajectoryVector::TrajectoryVector(const map<double,Vector>& map_values)
    {
      typename map<double,Vector>::const_iterator it_map;
      for(it_map = map_values.begin() ; it_map != map_values.end() ; it_map++)
      {
        assert((size() == 0 // nothing added yet
             || size() == it_map->second.size()) && "vectors of map_values of different dimensions");
        set(it_map->second, it_map->first);
      }

      // todo: optimize this by building Trajectory objects here directly?
    }

    TrajectoryVector::TrajectoryVector(const vector<map<double,double> >& v_map_values)
      : TrajectoryVector(v_map_values.size())
    {
      assert(!v_map_values.empty());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = Trajectory(v_map_values[i]);
    }
    
    TrajectoryVector::TrajectoryVector(const list<double>& list_t, const list<Vector>& list_x)
    {
      assert(!list_t.empty());
      assert(list_t.size() == list_x.size());

      #ifdef DEBUG
      int n = list_x.begin()->size();
      #endif

      list<double>::const_iterator it_t = list_t.begin();
      list<Vector>::const_iterator it_x = list_x.begin();
      for( ;
          it_t != list_t.end() && it_x != list_x.end();
          ++it_t, ++it_x)
      {
        assert(it_x->size() == n);
        set(*it_x, *it_t);
      }

      // todo: optimize this by building Trajectory objects here directly?
    }

    TrajectoryVector::TrajectoryVector(int n, const Trajectory& x)
      : TrajectoryVector(n)
    {
      assert(n > 0);
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = x;
    }

    TrajectoryVector::TrajectoryVector(initializer_list<Trajectory> list)
      : TrajectoryVector(list.size())
    {
      assert(list.size() > 0);
      std::copy(list.begin(), list.end(), m_v_trajs);
    }

    TrajectoryVector::TrajectoryVector(const TrajectoryVector& traj)
    {
      *this = traj;
    }

    TrajectoryVector::~TrajectoryVector()
    {
      if(m_v_trajs)
        delete[] m_v_trajs;
    }

    const TrajectoryVector& TrajectoryVector::operator=(const TrajectoryVector& x)
    {
      { // Destroying already existing components
        if(m_v_trajs)
          delete[] m_v_trajs;
      }

      m_n = x.size();
      
      m_v_trajs = new Trajectory[m_n];
      for(int i = 0 ; i < size() ; i++)
        (*this)[i] = x[i]; // copy of each component

      return *this;
    }

    int TrajectoryVector::size() const
    {
      return m_n;
    }

    void TrajectoryVector::resize(int n)
    {
      assert(n > 0);

      if(n == size())
        return;

      Trajectory *new_vec = new Trajectory[n];

      int i = 0;
      for(; i < size() && i < n ; i++)
        new_vec[i] = m_v_trajs[i];

      for(; i < n ; i++)
        new_vec[i] = Trajectory();

      if(m_v_trajs) // (m_v_trajs == nullptr) may happen when default constructor is used
        delete[] m_v_trajs;

      m_n = n;
      m_v_trajs = new_vec;
    }

    const TrajectoryVector TrajectoryVector::subvector(int start_index, int end_index) const
    {
      assert(start_index >= 0);
      assert(end_index < size());
      assert(start_index <= end_index);
      TrajectoryVector subvec(end_index - start_index + 1);
      for(int i = 0 ; i < subvec.size() ; i++)
        subvec[i] = (*this)[i + start_index];
      return subvec;
    }

    void TrajectoryVector::put(int start_index, const TrajectoryVector& subvec)
    {
      assert(start_index >= 0);
      assert(start_index + subvec.size() <= size());
      for(int i = 0 ; i < subvec.size() ; i++)
        (*this)[i + start_index] = subvec[i];
    }

    // Accessing values

    const Interval TrajectoryVector::tdomain() const
    {
      return (*this)[0].tdomain();
    }

    const IntervalVector TrajectoryVector::codomain() const
    {
      return codomain_box();
    }

    Trajectory& TrajectoryVector::operator[](int index)
    {
      assert(index >= 0 && index < size());
      return const_cast<Trajectory&>(static_cast<const TrajectoryVector&>(*this).operator[](index));
    }

    const Trajectory& TrajectoryVector::operator[](int index) const
    {
      assert(index >= 0 && index < size());
      return m_v_trajs[index];
    }

    const Vector TrajectoryVector::operator()(double t) const
    {
      assert(tdomain().contains(t));
      Vector v(size());
      for(int i = 0 ; i < size() ; i++)
        v[i] = (*this)[i](t);
      return v;
    }
    
    const IntervalVector TrajectoryVector::operator()(const Interval& t) const
    {
      assert(tdomain().is_superset(t));
      IntervalVector v(size());
      for(int i = 0 ; i < size() ; i++)
        v[i] = (*this)[i](t);
      return v;
    }
    
    const Vector TrajectoryVector::first_value() const
    {
      Vector v(size());
      for(int i = 0 ; i < size() ; i++)
        v[i] = (*this)[i].first_value();
      return v;
    }

    const Vector TrajectoryVector::last_value() const
    {
      Vector v(size());
      for(int i = 0 ; i < size() ; i++)
        v[i] = (*this)[i].last_value();
      return v;
    }
    
    // Tests

    bool TrajectoryVector::not_defined() const
    {
      for(int i = 0 ; i < size() ; i++)
        if((*this)[i].not_defined())
          return true;
      return false;
    }

    bool TrajectoryVector::operator==(const TrajectoryVector& x) const
    {
      if(size() != x.size())
        return false;
      for(int i = 0 ; i < size() ; i++)
        if((*this)[i] != x[i])
          return false;
      return true;
    }
    
    bool TrajectoryVector::operator!=(const TrajectoryVector& x) const
    {
      if(size() != x.size())
        return true;
      for(int i = 0 ; i < size() ; i++)
        if((*this)[i] != x[i])
          return true;
      return false;
    }

    // Setting values

    void TrajectoryVector::set(const Vector& y, double t)
    {
      if(m_n == 0)
      {
        m_n = y.size();
        m_v_trajs = new Trajectory[m_n];
      }

      assert(size() == y.size());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].set(y[i], t);
    }

    TrajectoryVector& TrajectoryVector::truncate_tdomain(const Interval& t)
    {
      assert(valid_tdomain(t));
      assert(tdomain().is_superset(t));
      for(int i = 0 ; i < size() ; i++)
        if(!(*this)[i].not_defined())
          (*this)[i].truncate_tdomain(t);
      return *this;
    }

    TrajectoryVector& TrajectoryVector::shift_tdomain(double shift_ref)
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].shift_tdomain(shift_ref);
      return *this;
    }
    
    bool TrajectoryVector::same_tdomain_forall_components() const
    {
      for(int i = 1 ; i < size() ; i++)
        if((*this)[i].tdomain() != (*this)[0].tdomain())
          return false;
      return true;
    }

    TrajectoryVector& TrajectoryVector::sample(double dt)
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(dt);
      return *this;
    }

    TrajectoryVector& TrajectoryVector::sample(const Trajectory& x)
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(x);
      return *this;
    }

    TrajectoryVector& TrajectoryVector::sample(const TrajectoryVector& x)
    {
      assert(size() == x.size());
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].sample(x[i]);
      return *this;
    }

    TrajectoryVector& TrajectoryVector::make_continuous()
    {
      for(int i = 0 ; i < size() ; i++)
        (*this)[i].make_continuous();
      return *this;
    }
    
    // Integration
    
    const TrajectoryVector TrajectoryVector::primitive(const Vector& c) const
    {
      assert(c.size() == size());
      TrajectoryVector x(size());

      for(int i = 0 ; i < size() ; i++)
        x[i] = (*this)[i].primitive(c[i]);

      return x;
    }
    
    const TrajectoryVector TrajectoryVector::primitive(const Vector& c, double dt) const
    {
      assert(dt > 0.);
      assert(c.size() == size());
      TrajectoryVector x(size());

      for(int i = 0 ; i < size() ; i++)
        x[i] = (*this)[i].primitive(c[i], dt);

      return x;
    }
    
    const TrajectoryVector TrajectoryVector::diff() const
    {
      TrajectoryVector x(size());

      for(int i = 0 ; i < size() ; i++)
        x[i] = (*this)[i].diff();

      return x;
    }

    // String
    
    std::ostream& operator<<(std::ostream& str, const TrajectoryVector& x)
    {
      str << "TrajectoryVector (dim " << x.size() << ") "
          << x.tdomain() << "↦" << x.codomain() << flush;
      return str;
    }

  // Protected methods

    const IntervalVector TrajectoryVector::codomain_box() const
    {
      IntervalVector box(size(), Interval::EMPTY_SET);
      for(int i = 0 ; i < size() ; i++)
        box[i] |= (*this)[i].codomain();
      return box;
    }
}