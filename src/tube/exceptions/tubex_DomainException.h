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
#include "tubex_Tube.h"
#include "tubex_Exception.h"

namespace tubex
{
  /**
   * \brief Domain error exception.
   *
   * Thrown when two tubes do not share the same domain
   * or have a different data structure.
   */
  class DomainException : public Exception
  {
    public:

      DomainException(const Tube& x, int slice_index);
      DomainException(const Tube& x, double t);
      DomainException(const Tube& x, const ibex::Interval& intv_t);
      DomainException(const Tube& x1, const Tube& x2);
  };

  void checkDomain(const Tube& x, int slice_index);
  void checkDomain(const Tube&, double t);
  void checkDomain(const Tube& x, const ibex::Interval& intv_t);
  void checkStructures(const Tube& x1, const Tube& x2);
}

#endif