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

#include <string>
#include <sstream>
#include "tubex_EmptyException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  EmptyException::EmptyException(const TubeSlice& x)
  {
    ostringstream os;
    os << "empty slice";    
    m_what_msg = os.str();
  }

  void EmptyException::check(const TubeSlice& x)
  {
    if(x.is_empty())
      throw EmptyException(x);
  }

  EmptyException::EmptyException(const TubeVector& x)
  {
    ostringstream os;
    os << "emptiness over ";

    if(x.codomain().is_empty())
      os << "the whole domain";

    else
    {
      Interval intv_t_emptiness = Interval::EMPTY_SET;
      const TubeSlice *slice = x.get_first_slice();
      while(slice != NULL)
      {
        if(slice->is_empty())
        {
          if(slice->input_gate().is_empty())
            intv_t_emptiness |= slice->domain().lb();
          if(slice->codomain().is_empty())
            intv_t_emptiness |= slice->domain();
          if(slice->output_gate().is_empty())
            intv_t_emptiness |= slice->domain().ub();
        }
        slice = slice->next_slice();
      }
      os << "[t]=" << intv_t_emptiness << endl;
    }
    
    m_what_msg = os.str();
  }

  void EmptyException::check(const TubeVector& x)
  {
    if(x.is_empty())
      throw EmptyException(x);
  }
}