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
#include "tubex_SlicingException.h"
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  SlicingException::SlicingException(const Slice& x1, const Slice& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.domain() != x2.domain())
      os << endl << "Slices with different domains: " 
         << "t1=" << x1.domain() << " and t2=" << x2.domain();

    os << endl;
    m_what_msg = os.str();
  }

  SlicingException::SlicingException(const Tube& x, int slice_index)
  {
    ostringstream os;
    os << "slice index out of range: ";
    os << "i=" << slice_index << " not in " << Interval(0,(x.nb_slices()-1)) << endl;
    m_what_msg = os.str();
  }

  SlicingException::SlicingException(const Tube& x1, const Tube& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.nb_slices() != x2.nb_slices())
      os << endl << "Tube(Vector) of different slices number: " 
         << "n1=" << x1.nb_slices() << " and n2=" << x2.nb_slices();

    os << endl;
    m_what_msg = os.str();
  }

  void SlicingException::check(const Tube& x, int slice_index)
  {
    if(slice_index < 0 || slice_index >= x.nb_slices())
      throw SlicingException(x, slice_index);
  }

  void SlicingException::check(const Tube& x1, const Tube& x2)
  {
    if(!Tube::same_slicing(x1, x2))
      throw SlicingException(x1, x2);
  }

  void SlicingException::check(const Tube& x1, const TubeVector& x2)
  {
    if(!TubeVector::same_slicing(x1, x2))
      throw SlicingException(x1, x2[0]);
  }

  void SlicingException::check(const TubeVector& x, int slice_index)
  {
    check(x[0], slice_index);
  }

  void SlicingException::check(const TubeVector& x1, const Tube& x2)
  {
    if(!TubeVector::same_slicing(x1, x2))
      throw SlicingException(x1[0], x2);
  }

  void SlicingException::check(const TubeVector& x1, const TubeVector& x2)
  {
    if(!TubeVector::same_slicing(x1, x2))
      throw SlicingException(x1[0], x2[0]);
  }
}