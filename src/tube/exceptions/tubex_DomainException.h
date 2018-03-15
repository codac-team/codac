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
   * Thrown when two tubes do not share the same domain
   * or have a different data structure.
   */
  class DomainException : public Exception
  {
    public:

      DomainException(const Subtube& x, int slice_index);
      DomainException(const Subtube& x, double t);
      DomainException(const Subtube& x, const ibex::Interval& intv_t);
      DomainException(const Subtube& x1, const Subtube& x2);
  };

  void checkDomain(const Subtube& x, int slice_index);
  void checkDomain(const Subtube&, double t);
  void checkDomain(const Subtube& x, const ibex::Interval& intv_t);
  void checkStructures(const Subtube& x1, const Subtube& x2);
}

#endif