/* ============================================================================
 *  tubex-lib - DomainTubeException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "DomainTubeException.h"
#include "Tube.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

namespace tubex
{
  DomainTubeException::DomainTubeException(const Tube& x, int slice_index)
  {
    ostringstream os;
    os << "slice index out of range: ";
    os << "i=" << slice_index << " not in " << Interval(0,(x.size()-1)) << endl;
    m_what_msg = os.str();
  }

  DomainTubeException::DomainTubeException(const Tube& x, double t)
  {
    ostringstream os;
    os << "input out of range: ";
    os << "t=" << t << " not in " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainTubeException::DomainTubeException(const Tube& x, const Interval& intv_t)
  {
    ostringstream os;
    os << "interval input out of range: ";
    os << "t=" << intv_t << " not a subset of " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainTubeException::DomainTubeException(const Tube& x1, const Tube& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.size() != x2.size())
      os << endl << "Tubes of different slices number: " 
         << "n1=" << x1.size() << " and n2=" << x2.size();

    if(x1.domain() != x2.domain())
      os << endl << "Tubes of different domains: " 
         << "[t1]=" << x1.domain() << " and [t2]=" << x2.domain();

    os << endl;
    m_what_msg = os.str();
  }

  void checkDomain(const Tube& x, int slice_index)
  {
    if(slice_index < 0 || slice_index > x.size() - 1)
      throw DomainTubeException(x, slice_index);
  }

  void checkDomain(const Tube& x, double t)
  {
    if(!x.domain().contains(t))
      throw DomainTubeException(x, t);
  }

  void checkDomain(const Tube& x, const Interval& intv_t)
  {
    if(intv_t != x.domain() && !intv_t.is_interior_subset(x.domain()))
      throw DomainTubeException(x, intv_t);
  }

  void checkStructures(const Tube& x1, const Tube& x2)
  {
    if(x1.size() != x2.size() || x1.domain() != x2.domain())
      throw DomainTubeException(x1, x2);
  }
}