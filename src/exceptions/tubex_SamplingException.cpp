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
#include "tubex_SamplingException.h"
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  SamplingException::SamplingException(const TubeSlice& x1, const TubeSlice& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.domain() != x2.domain())
      os << endl << "TubeSlices with different domains: " 
         << "t1=" << x1.domain() << " and t2=" << x2.domain();

    os << endl;
    m_what_msg = os.str();
  }

  SamplingException::SamplingException(const TubeVector& x, int slice_index)
  {
    ostringstream os;
    os << "slice index out of range: ";
    os << "i=" << slice_index << " not in " << Interval(0,(x.nbSlices()-1)) << endl;
    m_what_msg = os.str();
  }

  SamplingException::SamplingException(const TubeVector& x1, const TubeVector& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.nbSlices() != x2.nbSlices())
      os << endl << "TubeVector of different slices number: " 
         << "n1=" << x1.nbSlices() << " and n2=" << x2.nbSlices();

    os << endl;
    m_what_msg = os.str();
  }

  void SamplingException::check(const TubeVector& x1, const TubeVector& x2)
  {
    DomainException::check(x1, x2);
    if(x1.nbSlices() != x2.nbSlices())
      throw SamplingException(x1, x2);

    const TubeSlice *slice_x1 = x1.getFirstSlice();
    const TubeSlice *slice_x2 = x2.getFirstSlice();
    
    while(slice_x1 != NULL)
    {
      if(slice_x1->domain() != slice_x2->domain())
        throw SamplingException(*slice_x1, *slice_x2);

      slice_x1 = slice_x1->nextSlice();
      slice_x2 = slice_x2->nextSlice();
    }
  }

  void SamplingException::check(const TubeVector& x, int slice_index)
  {
    if(slice_index < 0 || slice_index >= x.nbSlices())
      throw SamplingException(x, slice_index);
  }
}