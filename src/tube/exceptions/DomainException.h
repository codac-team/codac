/* ============================================================================
 *  tube-lib - DomainException class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef DomainException_HEADER
#define DomainException_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "Tube.h"

/**
 * \brief Domain error exception.
 *
 * Thrown when two tubes do not share the same domain
 * or have a different data structure.
 */
class DomainException : public std::exception
{
  public:

    DomainException(const Tube& x, int slice_index);
    DomainException(const Tube& x, double t);
    DomainException(const Tube& x, const ibex::Interval& intv_t);
    DomainException(const Tube& x1, const Tube& x2);
    ~DomainException() {};

    virtual const char* what() const throw();

    std::string m_what_msg;
};

std::ostream& operator<<(std::ostream& os, const DomainException& e);
void checkDomain(const Tube& x, int slice_index);
void checkDomain(const Tube&, double t);
void checkDomain(const Tube& x, const ibex::Interval& intv_t);
void checkStructures(const Tube& x1, const Tube& x2);

#endif