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


DomainError::DomainError(const Tube& x1, const Tube& x2) :
        m_x1_slices_number(x1.size()), m_x2_slices_number(x2.size()),
        m_x1_domain(x1.domain()), m_x2_domain(x2.domain())
{
  m_what_msg = new string;
}

DomainError::~DomainError()
{
  delete m_what_msg;
}

const char* DomainError::what() const throw()
{
  ostringstream os;
  os << "DomainError: unable to perform an operation over tubes of different structure";

  if(m_x1_slices_number != m_x2_slices_number)
    os << endl << "Tubes of different slices number: " 
       << "n1=" << m_x1_slices_number << " and n2=" << m_x2_slices_number;

  if(m_x1_domain != m_x2_domain)
    os << endl << "Tubes of different domains: " 
       << "[t1]=" << m_x1_domain << " and [t2]=" << m_x2_domain;

  os << endl;
  *m_what_msg = os.str();
  return m_what_msg->c_str();
}

std::ostream& operator<<(std::ostream& os, const DomainError& e)
{
  os << e.what();
  return os;
}

void checkDomain(const Tube& x1, const Tube& x2)
{
  if(x1.size() != x2.size() || x1.domain() != x2.domain())
    throw DomainError(x1, x2);
}