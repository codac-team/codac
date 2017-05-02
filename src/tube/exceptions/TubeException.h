/* ============================================================================
 *  tube-lib - TubeException class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBEEXCEPTION_HEADER
#define TUBEEXCEPTION_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "Tube.h"

/**
 * \brief TubeException abstract class.
 *
 * Thrown when necessary.
 */
class TubeException : public std::exception
{
  public:

    TubeException() {};
    TubeException(const std::string& function_name, const std::string& custom_message);
    ~TubeException() {};

    virtual const char* what() const throw();

  protected:

    std::string m_what_msg;
};

std::ostream& operator<<(std::ostream& os, const TubeException& e);

#endif