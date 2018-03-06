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
#include "tubex_Tube.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

namespace tubex
{
  EmptyException::EmptyException(const Tube& x)
  {
    ostringstream os;
    os << "emptiness over ";

    if(x.image() == Interval::EMPTY_SET)
      os << "the whole domain";

    else
    {
      Interval intv_t_emptiness = Interval::EMPTY_SET;
      for(int i = 0 ; i < x.size() ; i++)
        if(x[i].is_empty())
          intv_t_emptiness |= x.domain(i);
      os << "[t]=" << intv_t_emptiness << endl;
    }
    
    m_what_msg = os.str();
  }

  void checkEmptiness(const Tube& x)
  {
    if(x.isEmpty())
      throw EmptyException(x);

    for(int i = 0 ; i < x.size() ; i++)
      if(x[i].is_empty())
        throw EmptyException(x);
  }
}