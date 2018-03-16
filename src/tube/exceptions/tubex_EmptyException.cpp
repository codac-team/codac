/* ============================================================================
 *  tubex-lib - EmptyException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_EmptyException.h"
#include "tubex_Subtube.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

namespace tubex
{
  EmptyException::EmptyException(const Subtube& x)
  {
    ostringstream os;
    os << "emptiness over ";

    if(x.image() == Interval::EMPTY_SET)
      os << "the whole domain";

    else
    {
      Interval intv_t_emptiness = Interval::EMPTY_SET;
      for(int i = 0 ; i < x.nbSlices() ; i++)
        if(x[i].is_empty())
          intv_t_emptiness |= x.sliceDomain(i);
      os << "[t]=" << intv_t_emptiness << endl;
    }
    
    m_what_msg = os.str();
  }

  void EmptyException::check(const Subtube& x)
  {
    if(x.isEmpty())
      throw EmptyException(x);

    for(int i = 0 ; i < x.nbSlices() ; i++)
      if(x[i].is_empty())
        throw EmptyException(x);
  }
}