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

/**
 * \brief Emptiness error exception.
 *
 * Thrown when a tube is empty.
 */
class EmptyTubeException : public std::exception
{
  public:

    EmptyTubeException(const Tube& x);
    ~EmptyTubeException() {};

    virtual const char* what() const throw();

    std::string m_what_msg;
};

std::ostream& operator<<(std::ostream& os, const EmptyTubeException& e);
void checkEmptiness(const Tube& x);

#endif