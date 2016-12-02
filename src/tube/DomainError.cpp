/* ============================================================================
 *  tube-lib - DomainError exception class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "DomainError.h"
#include "Tube.h"
#include <string>
#include <sstream>

using namespace std;
using namespace ibex;


DomainError::DomainError(const Tube& x, int slice_index)
{
  ostringstream os;
  os << "slice index out of range: ";
  os << "i=" << slice_index << " not in " << Interval(0,(x.size()-1)) << endl;
  m_what_msg = os.str();
}

DomainError::DomainError(const Tube& x, double t)
{
  ostringstream os;
  os << "input out of range: ";
  os << "t=" << t << " not in " << x.domain() << endl;
  m_what_msg = os.str();
}

DomainError::DomainError(const Tube& x, const Interval& intv_t)
{
  ostringstream os;
  os << "interval input out of range: ";
  os << "t=" << intv_t << " not a subset of " << x.domain() << endl;
  m_what_msg = os.str();
}

DomainError::DomainError(const Tube& x1, const Tube& x2)
{
  ostringstream os;
  os << "Unable to perform an operation over tubes of different structure";

  if(x1.size() != x2.size())
    os << endl << "Tubes of different slices number: " 
       << "n1=" << x1.size() << " and n2=" << x2.size();

  if(x1.domain() != x2.domain())
    os << endl << "Tubes of different domains: " 
       << "[t1]=" << x1.domain() << " and [t2]=" << x2.domain();

  os << endl;
  m_what_msg = os.str();
}

const char* DomainError::what() const throw()
{
  return m_what_msg.c_str();
}

std::ostream& operator<<(std::ostream& os, const DomainError& e)
{
  os << e.what();
  return os;
}

void checkDomain(const Tube& x, int slice_index)
{
  if(slice_index < 0 || slice_index > x.size() - 1)
    throw DomainError(x, slice_index);
}

void checkDomain(const Tube& x, double t)
{
  if(!x.domain().contains(t))
    throw DomainError(x, t);
}

void checkDomain(const Tube& x, const Interval& intv_t)
{
  if(intv_t != x.domain() && !intv_t.is_interior_subset(x.domain()))
    throw DomainError(x, intv_t);
}

void checkStructures(const Tube& x1, const Tube& x2)
{
  if(x1.size() != x2.size() || x1.domain() != x2.domain())
    throw DomainError(x1, x2);
}