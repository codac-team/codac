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

      DomainException(const TubeSlice& x, double t);
      DomainException(const Trajectory& x, double t);
      DomainException(const TubeSlice& x, const ibex::Interval& t);
      DomainException(const Trajectory& x, const ibex::Interval& t);
      DomainException(const TubeSlice& x1, const TubeSlice& x2);
      DomainException(const Trajectory& x1, const Trajectory& x2);
      DomainException(const TubeSlice& x1, const Trajectory& x2);

      DomainException(const TubeVector& x, int slice_index);
      DomainException(const TubeVector& x, double t);
      DomainException(const TubeVector& x, const ibex::Interval& t);
      DomainException(const TubeVector& x1, const TubeVector& x2);
      DomainException(const TubeVector& x1, const Trajectory& x2);
        
      static void check(const ibex::Interval& domain);
      static void check(double timestep);

      static void check(const TubeSlice& x, double t);
      static void check(const Trajectory& x, double t);
      static void check(const TubeSlice& x, const ibex::Interval& t);
      static void check(const Trajectory& x, const ibex::Interval& t);
      static void check(const TubeSlice& x1, const TubeSlice& x2);
      static void check(const Trajectory& x1, const Trajectory& x2);
      static void check(const TubeSlice& x1, const Trajectory& x2);
        
      static void check(const TubeVector& x, int slice_index);
      static void check(const TubeVector& x, double t);
      static void check(const TubeVector& x, const ibex::Interval& t);
      static void check(const TubeVector& x1, const TubeVector& x2);
      static void check(const TubeVector& x1, const Trajectory& x2);
  };
}

#endif