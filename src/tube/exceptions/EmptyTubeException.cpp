/* ============================================================================
 *  tube-lib - EmptyTubeException class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "EmptyTubeException.h"
#include "Tube.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;

EmptyTubeException::EmptyTubeException(const Tube& x)
{
  Interval intv_t_emptiness = Interval::EMPTY_SET;
  for(int i = 0 ; i < x.size() ; i++)
    if(x[i].is_empty())
      intv_t_emptiness |= x.domain(i);
    
  ostringstream os;
  os << "emptiness over [t]=" << intv_t_emptiness << endl;
  m_what_msg = os.str();
}

const char* EmptyTubeException::what() const throw()
{
  return m_what_msg.c_str();
}

std::ostream& operator<<(std::ostream& os, const EmptyTubeException& e)
{
  os << e.what();
  return os;
}

void checkEmptiness(const Tube& x)
{
  if(x.isEmpty())
    throw EmptyTubeException(x);
}