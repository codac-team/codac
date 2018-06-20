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
#include "tubex_TubeSlice.h"
#include "tubex_Trajectory.h"
#include "tubex_Tube.h"

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

      DomainException(const TubeSlice& x, double t);
      DomainException(const Trajectory& x, double t);
      DomainException(const TubeSlice& x, const ibex::Interval& t);
      DomainException(const Trajectory& x, const ibex::Interval& t);
      DomainException(const TubeSlice& x1, const TubeSlice& x2);
      DomainException(const Trajectory& x1, const Trajectory& x2);
      DomainException(const TubeSlice& x1, const Trajectory& x2);

      DomainException(const Tube& x, int slice_index);
      DomainException(const Tube& x, double t);
      DomainException(const Tube& x, const ibex::Interval& t);
      DomainException(const Tube& x1, const Tube& x2);
      DomainException(const Tube& x1, const Trajectory& x2);
        
      static void check(const TubeSlice& x, double t);
      static void check(const Trajectory& x, double t);
      static void check(const TubeSlice& x, const ibex::Interval& t);
      static void check(const Trajectory& x, const ibex::Interval& t);
      static void check(const TubeSlice& x1, const TubeSlice& x2);
      static void check(const Trajectory& x1, const Trajectory& x2);
      static void check(const TubeSlice& x1, const Trajectory& x2);
        
      static void check(const Tube& x, int slice_index);
      static void check(const Tube& x, double t);
      static void check(const Tube& x, const ibex::Interval& t);
      static void check(const Tube& x1, const Tube& x2);
      static void check(const Tube& x1, const Trajectory& x2);
  };
}

#endif