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

#include "tubex_Exception.h"
#include "tubex_DynamicalItem.h"
#include "tubex_TubeVector.h"

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

      DomainException(const ibex::Interval& domain);
      DomainException(double timestep);
      DomainException(const DynamicalItem& x, const ibex::Interval& t);
      DomainException(const DynamicalItem& x1, const DynamicalItem& x2);
        
      static void check(const ibex::Interval& domain);
      static void check(double timestep);
      static void check(const DynamicalItem& x, const ibex::Interval& t);
      static void check(const DynamicalItem& x1, const DynamicalItem& x2);
  };
}

#endif