/* ============================================================================
 *  tube-lib - DomainTubeException class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef DOMAINTUBEEXCEPTION_HEADER
#define DOMAINTUBEEXCEPTION_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "Tube.h"
#include "TubeException.h"

/**
 * \brief Domain error exception.
 *
 * Thrown when two tubes do not share the same domain
 * or have a different data structure.
 */
class DomainTubeException : public TubeException
{
  public:

    DomainTubeException(const Tube& x, int slice_index);
    DomainTubeException(const Tube& x, double t);
    DomainTubeException(const Tube& x, const ibex::Interval& intv_t);
    DomainTubeException(const Tube& x1, const Tube& x2);
};

void checkDomain(const Tube& x, int slice_index);
void checkDomain(const Tube&, double t);
void checkDomain(const Tube& x, const ibex::Interval& intv_t);
void checkStructures(const Tube& x1, const Tube& x2);

#endif