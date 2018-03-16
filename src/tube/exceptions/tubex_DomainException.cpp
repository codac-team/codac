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

#include "tubex_DomainException.h"
#include "tubex_Subtube.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

namespace tubex
{
  DomainException::DomainException(const Subtube& x, int slice_index)
  {
    ostringstream os;
    os << "slice index out of range: ";
    os << "i=" << slice_index << " not in " << Interval(0,(x.nbSlices()-1)) << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const Subtube& x, double t)
  {
    ostringstream os;
    os << "input out of range: ";
    os << "t=" << t << " not in " << x.domain() << endl;
    m_what_msg = os.str();
  }

  DomainException::DomainException(const Subtube& x, const Interval& intv_t)
  {
    ostringstream os;
    os << "interval input out of range: ";
    os << "t=" << intv_t << " not a subset of " << x.domain() << endl;
    m_what_msg = os.str();
  }

  void check(const Subtube& x, int slice_index)
  {
    if(slice_index < 0 || slice_index > x.nbSlices() - 1)
      throw DomainException(x, slice_index);
  }

  void check(const Subtube& x, double t)
  {
    if(!x.domain().contains(t))
      throw DomainException(x, t);
  }

  void check(const Subtube& x, const Interval& intv_t)
  {
    if(intv_t != x.domain() && !intv_t.is_interior_subset(x.domain()))
      throw DomainException(x, intv_t);
  }
}