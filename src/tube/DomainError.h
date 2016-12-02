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

#ifndef DomainError_HEADER
#define DomainError_HEADER

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
class DomainError : public std::exception
{
  public:

    DomainError(const Tube& x, int slice_index);
    DomainError(const Tube& x, double t);
    DomainError(const Tube& x, const ibex::Interval& intv_t);
    DomainError(const Tube& x1, const Tube& x2);
    ~DomainError() {};

    virtual const char* what() const throw();

    std::string m_what_msg;
};

std::ostream& operator<<(std::ostream& os, const DomainError& e);
void checkDomain(const Tube& x, int slice_index);
void checkDomain(const Tube&, double t);
void checkDomain(const Tube& x, const ibex::Interval& intv_t);
void checkStructures(const Tube& x1, const Tube& x2);

#endif