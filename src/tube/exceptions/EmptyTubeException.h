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

#ifndef EmptyTubeException_HEADER
#define EmptyTubeException_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "Tube.h"
#include "TubeException.h"

/**
 * \brief Emptiness error exception.
 *
 * Thrown when a tube is empty.
 */
class EmptyTubeException : public TubeException
{
  public:

    EmptyTubeException(const Tube& x);
};

void checkEmptiness(const Tube& x);

#endif