/* ============================================================================
 *  tubex-lib - TrajectoryVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <sstream>
#include "tubex_TrajectoryVector.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Definition

  TrajectoryVector::TrajectoryVector(int n)
    : m_n(n), m_v_trajs(new Trajectory[n])
  {
    DimensionException::check(n);
  }

  TrajectoryVector::TrajectoryVector(const TrajectoryVector& traj)
  {
    *this = traj;
  }

  TrajectoryVector::TrajectoryVector(const Interval& domain, const tubex::Function& f)
    : TrajectoryVector(f.image_dim())
  {
    // todo: check thickness of f? (only thin functions should be allowed)
    DomainException::check(domain);

    // setting values for each component
    for(int i = 0 ; i < size() ; i++)
      (*this)[i] = Trajectory(domain, f, i);
  }

  TrajectoryVector::TrajectoryVector(const map<double,Vector>& map_values)
  {
    if(!map_values.empty())
    {
      m_n = map_values.begin()->second.size();
      m_v_trajs = new Trajectory[m_n];

      typename map<double,Vector>::const_iterator it_map;
      for(it_map = map_values.begin() ; it_map != map_values.end() ; it_map++)
      {
        for(int i = 0 ; i < size() ; i++)
          (*this)[i].set(it_map->first, it_map->second[i]);

        // todo: check m_n == it_map->second.size(), or:
        DimensionException::check(map_values.begin()->second, it_map->second);
      }

    }
  }

  TrajectoryVector::~TrajectoryVector()
  {
    if(m_v_trajs != NULL)
      delete[] m_v_trajs;
  }

  const TrajectoryVector& TrajectoryVector::operator=(const TrajectoryVector& x)
  {
    m_n = x.size();
    m_v_trajs = new Trajectory[m_n];
    for(int i = 0 ; i < size() ; i++)
      (*this)[i] = x[i];
  }

  int TrajectoryVector::size() const
  {
    return m_n;
  }

  // Access values

  // deprecated: const map<double,Vector>& TrajectoryVector::get_map() const
  // deprecated: {
  // deprecated:   return m_map_values;
  // deprecated: }

  // todo: where to store function? const tubex::Function* TrajectoryVector::get_function() const
  // todo: where to store function? {
  // todo: where to store function?   return m_function;
  // todo: where to store function? }

  const Interval TrajectoryVector::domain() const
  {
    return (*this)[0].domain();
  }

  const IntervalVector TrajectoryVector::codomain() const
  {
    return codomain_box();
  }

  Trajectory& TrajectoryVector::operator[](int index)
  {
    // todo: check index
    return const_cast<Trajectory&>(static_cast<const TrajectoryVector&>(*this).operator[](index));
    // todo: check quickness of cast instead of direct access to m_v_tubes[]
  }

  const Trajectory& TrajectoryVector::operator[](int index) const
  {
    // todo: check index
    return m_v_trajs[index];
  }

  const Vector TrajectoryVector::operator()(double t) const
  {
    DomainException::check(*this, t);

    Vector v(size());
    for(int i = 0 ; i < size() ; i++)
      v[i] = (*this)[i](t);
    return v;
  }
  
  const IntervalVector TrajectoryVector::operator()(const Interval& t) const
  {
    DomainException::check(*this, t);
    IntervalVector v(size());
    for(int i = 0 ; i < size() ; i++)
      v[i] = (*this)[i](t);
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
    DimensionException::check(*this, x);
    for(int i = 0 ; i < size() ; i++)
      if((*this)[i] != x[i])
        return false;
    return true;
  }
  
  bool TrajectoryVector::operator!=(const TrajectoryVector& x) const
  {
    DimensionException::check(*this, x);
    for(int i = 0 ; i < size() ; i++)
      if((*this)[i] != x[i])
        return true;
    return false;
  }

  // Setting values

  void TrajectoryVector::set(double t, const Vector& y)
  {
    // todo: do something in case of undefined traj vector
    // todo: check size vector y

    for(int i = 0 ; i < size() ; i++)
      (*this)[i].set(t, y[i]);
  }

  void TrajectoryVector::truncate_domain(const Interval& domain)
  {
    DomainException::check(domain);
    for(int i = 0 ; i < size() ; i++)
      (*this)[i].truncate_domain(domain);
  }

  void TrajectoryVector::shift_domain(double shift_ref)
  {
    for(int i = 0 ; i < size() ; i++)
      (*this)[i].shift_domain(shift_ref);
  }

  // String
  
  std::ostream& operator<<(std::ostream& str, const TrajectoryVector& x)
  {
    str << "TrajectoryVector " << x.domain() << "↦" << x.codomain();
    // todo: more info?
    str << flush;
    return str;
  }

  const IntervalVector TrajectoryVector::codomain_box() const
  {
    IntervalVector box(size(), Interval::EMPTY_SET);
    for(int i = 0 ; i < size() ; i++)
      box[i] |= (*this)[i].codomain();
    return box;
  }
}