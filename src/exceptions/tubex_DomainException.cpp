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
  DomainException::DomainException(const ibex::Interval& domain)
  {
    ostringstream os;
    os << "invalid domain: " << domain << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(double timestep)
  {
    ostringstream os;
    os << "invalid timestep: " << timestep << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const DynamicalItem& x, const Interval& t)
  {
    ostringstream os;
    os << "interval input out of range: ";
    os << "t=" << t << " not a subset of " << x.domain()
       << " (object " << x.class_name() << ")" << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const DynamicalItem& x1, const DynamicalItem& x2)
  {
    ostringstream os;
    os << "objects are not defined over the same domain: "
       << x1.domain() << "(" << x1.class_name() << ") != "
       << x2.domain() << "(" << x2.class_name() << ")" << endl;
    m_what_msg = os.str();
  }

  void DomainException::check(const ibex::Interval& domain)
  {
    if(domain.is_empty() || domain.is_unbounded() || domain.is_degenerated())
      throw DomainException(domain);
  }

  void DomainException::check(double timestep)
  {
    if(timestep < 0.)
      throw DomainException(timestep);
    // if 0. (default value), equivalent to no sampling
  }

  void DomainException::check(const DynamicalItem& x, const Interval& t)
  {
    if(t.is_empty() || !t.is_subset(x.domain()))
      throw DomainException(x, t);
  }

  void DomainException::check(const DynamicalItem& x1, const DynamicalItem& x2)
  {
    if(typeid(x1) == typeid(TubeSlice) && typeid(x2) != typeid(TubeSlice))
    {
      if(!x1.domain().is_subset(x2.domain()))
        throw DomainException(x1, x2);
    }

    else if(typeid(x2) == typeid(TubeSlice) && typeid(x1) != typeid(TubeSlice))
    {
      if(!x2.domain().is_subset(x1.domain()))
        throw DomainException(x1, x2);
    }

    else if(x1.domain() != x2.domain())
      throw DomainException(x1, x2);
  }
}