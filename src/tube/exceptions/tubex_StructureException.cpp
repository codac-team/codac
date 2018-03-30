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

#include "tubex_StructureException.h"
#include "tubex_TubeNode.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

namespace tubex
{
  StructureException::StructureException(const TubeNode& x1, const TubeNode& x2)
  {
    ostringstream os;
    os << "unable to perform an operation over tubes of different structure";

    if(x1.nbSlices() != x2.nbSlices())
      os << endl << "TubeNodes of different slices number: " 
         << "n1=" << x1.nbSlices() << " and n2=" << x2.nbSlices();

    if(x1.domain() != x2.domain())
      os << endl << "TubeNodes of different domains: " 
         << "[t1]=" << x1.domain() << " and [t2]=" << x2.domain();

    os << endl;
    m_what_msg = os.str();
  }

  void check(const TubeNode& x1, const TubeNode& x2)
  {
    if(x1.nbSlices() != x2.nbSlices() || x1.domain() != x2.domain())
      throw StructureException(x1, x2);
  }
}