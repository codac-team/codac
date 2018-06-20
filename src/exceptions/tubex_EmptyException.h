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

#include "tubex_Exception.h"
#include "tubex_TubeSlice.h"
#include "tubex_Trajectory.h"
#include "tubex_Tube.h"

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

      EmptyException(const TubeSlice& x);
      EmptyException(const Tube& x);
      // todo: equivalent constructor for Tube class

      static void check(const TubeSlice& x);
      static void check(const Tube& x);
  };
}

#endif