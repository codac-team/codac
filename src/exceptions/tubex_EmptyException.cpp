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
#include "tubex_TubeNode.h"
#include "tubex_TubeSlice.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

namespace tubex
{
  EmptyException::EmptyException(const TubeNode& x)
  {
    ostringstream os;
    os << "emptiness over ";

    if(x.codomain().is_empty())
      os << "the whole domain";

    else
    {
      Interval intv_t_emptiness = Interval::EMPTY_SET;
      TubeSlice *slice = x.getFirstSlice();
      while(slice != NULL)
      {
        if(slice->isEmpty())
        {
          if(slice->inputGate().is_empty())
            intv_t_emptiness |= slice->inputGate();
          if(slice->codomain().is_empty())
            intv_t_emptiness |= slice->codomain();
          if(slice->outputGate().is_empty())
            intv_t_emptiness |= slice->outputGate();
        }
        slice = slice->nextSlice();
      }
      os << "[t]=" << intv_t_emptiness << endl;
    }
    
    m_what_msg = os.str();
  }

  void EmptyException::check(const TubeNode& x)
  {
    if(x.isEmpty())
      throw EmptyException(x);
  }
}