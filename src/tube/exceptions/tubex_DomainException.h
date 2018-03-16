/* ============================================================================
 *  tubex-lib - DomainException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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
#include "tubex_Subtube.h"
#include "tubex_Exception.h"

namespace tubex
{
  /**
   * \brief Domain error exception.
   *
   * Thrown when an access to a tube is impossible.
   */
  class DomainException : public Exception
  {
    public:

      DomainException(const Subtube& x, int slice_index);
      DomainException(const Subtube& x, double t);
      DomainException(const Subtube& x, const ibex::Interval& intv_t);
      // todo: equivalent constructor for Tube class
        
      static void check(const Subtube& x, int slice_index);
      static void check(const Subtube& x, double t);
      static void check(const Subtube& x, const ibex::Interval& intv_t);
  };
}

#endif