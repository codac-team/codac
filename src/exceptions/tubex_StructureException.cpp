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
#include "tubex_StructureException.h"
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  StructureException::StructureException(const TubeSlice& x1, const TubeSlice& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.domain() != x2.domain())
      os << endl << "TubeSlices with different domains: " 
         << "t1=" << x1.domain() << " and t2=" << x2.domain();

    os << endl;
    m_what_msg = os.str();
  }

    StructureException::StructureException(const Tube& x1, const Tube& x2)
    {
      ostringstream os;
      os << "unable to perform an operation over tubes of different structure";

      if(x1.nbSlices() != x2.nbSlices())
        os << endl << "TubeSlices of different slices number: " 
           << "n1=" << x1.nbSlices() << " and n2=" << x2.nbSlices();

      os << endl;
      m_what_msg = os.str();
    }

    void StructureException::check(const Tube& x1, const Tube& x2)
    {
      DomainException::check(x1, x2);
      if(x1.nbSlices() != x2.nbSlices())
        throw StructureException(x1, x2);

      TubeSlice *slice_x1 = x1.getFirstSlice();
      TubeSlice *slice_x2 = x2.getFirstSlice();

      while(slice_x1 != NULL)
      {
        if(slice_x1->domain() != slice_x2->domain())
          throw StructureException(*slice_x1, *slice_x2);

        slice_x1 = slice_x1->nextSlice();
        slice_x2 = slice_x2->nextSlice();
      }
    }
}