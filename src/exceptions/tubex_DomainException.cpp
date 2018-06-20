/* ============================================================================
 *  tubex-lib - DomainException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include <string>
#include <sstream>
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  DomainException::DomainException(const TubeSlice& x, double t)
  {
    ostringstream os;
    os << "input out of range: ";
    os << "t=" << t << " not in " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const Trajectory& x, double t)
  {
    ostringstream os;
    os << "input out of range: ";
    os << "t=" << t << " not in " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const TubeSlice& x, const Interval& t)
  {
    ostringstream os;
    os << "interval input out of range: ";
    os << "t=" << t << " not a subset of " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const Trajectory& x, const Interval& t)
  {
    ostringstream os;
    os << "interval input out of range: ";
    os << "t=" << t << " not a subset of " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const TubeSlice& x1, const TubeSlice& x2)
  {
    ostringstream os;
    os << "variables are not defined over the same domain: " << x1.domain() << "!=" << x2.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const Trajectory& x1, const Trajectory& x2)
  {
    ostringstream os;
    os << "trajectories are not defined over the same domain: " << x1.domain() << "!=" << x2.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const TubeSlice& x1, const Trajectory& x2)
  {
    ostringstream os;
    os << "trajectory's domain is not a subset of tube's domain: " << x2.domain() << ", " << x1.domain() << endl;
    m_what_msg = os.str();
  }

    DomainException::DomainException(const Tube& x, int slice_index)
    {
      ostringstream os;
      os << "slice index out of range: ";
      os << "i=" << slice_index << " not in " << Interval(0,(x.nbSlices()-1)) << endl;
      m_what_msg = os.str();
    }

    DomainException::DomainException(const Tube& x, double t)
    {
      ostringstream os;
      os << "input out of range: ";
      os << "t=" << t << " not in " << x.domain() << endl;
      m_what_msg = os.str();
    }

    DomainException::DomainException(const Tube& x, const Interval& t)
    {
      ostringstream os;
      os << "interval input out of range: ";
      os << "t=" << t << " not a subset of " << x.domain() << endl;
      m_what_msg = os.str();
    }

    DomainException::DomainException(const Tube& x1, const Tube& x2)
    {
      ostringstream os;
      os << "variables are not defined over the same domain: " << x1.domain() << "!=" << x2.domain() << endl;
      m_what_msg = os.str();
    }

    DomainException::DomainException(const Tube& x1, const Trajectory& x2)
    {
      ostringstream os;
      os << "trajectory's domain is not a subset of tube's domain: " << x2.domain() << ", " << x1.domain() << endl;
      m_what_msg = os.str();
    }

  void DomainException::check(const TubeSlice& x, double t)
  {
    if(!x.domain().contains(t))
      throw DomainException(x, t);
  }

  void DomainException::check(const Trajectory& x, double t)
  {
    if(!x.domain().contains(t))
      throw DomainException(x, t);
  }

  void DomainException::check(const TubeSlice& x, const Interval& t)
  {
    if(t.is_empty() || !t.is_subset(x.domain()))
      throw DomainException(x, t);
  }

  void DomainException::check(const Trajectory& x, const Interval& t)
  {
    if(t.is_empty() || !t.is_subset(x.domain()))
      throw DomainException(x, t);
  }

  void DomainException::check(const TubeSlice& x1, const TubeSlice& x2)
  {
    if(x1.domain() != x2.domain())
      throw DomainException(x1, x2);
  }

  void DomainException::check(const Trajectory& x1, const Trajectory& x2)
  {
    if(x1.domain() != x2.domain())
      throw DomainException(x1, x2);
  }

  void DomainException::check(const TubeSlice& x1, const Trajectory& x2)
  {
    if(!x1.domain().is_subset(x2.domain()))
      throw DomainException(x1, x2);
  }

    void DomainException::check(const Tube& x, int slice_index)
    {
      if(slice_index < 0 || slice_index >= x.nbSlices())
        throw DomainException(x, slice_index);
    }

    void DomainException::check(const Tube& x, double t)
    {
      if(!x.domain().contains(t))
        throw DomainException(x, t);
    }

    void DomainException::check(const Tube& x, const Interval& t)
    {
      if(t.is_empty() || !t.is_subset(x.domain()))
        throw DomainException(x, t);
    }

    void DomainException::check(const Tube& x1, const Tube& x2)
    {
      if(x1.domain() != x2.domain())
        throw DomainException(x1, x2);
    }

    void DomainException::check(const Tube& x1, const Trajectory& x2)
    {
      if(!x1.domain().is_subset(x2.domain()))
        throw DomainException(x1, x2);
    }
}