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

#ifndef EmptyException_HEADER
#define EmptyException_HEADER

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include "tubex_Tube.h"
#include "tubex_Exception.h"

namespace tubex
{
  /**
   * \brief Emptiness error exception.
   *
   * Thrown when a tube is empty.
   */
  class EmptyException : public Exception
  {
    public:

      EmptyException(const Tube& x);
  };

  void checkEmptiness(const Tube& x);
}

#endif