/* ============================================================================
 *  tubex-lib - StructureException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_STRUCTUREEXCEPTION_H__
#define __TUBEX_STRUCTUREEXCEPTION_H__

#include "tubex_Exception.h"
#include "tubex_TubeSlice.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /**
   * \brief Structure error exception.
   *
   * Thrown when two tubes do not share
   * the same data structure.
   */
  class StructureException : public Exception
  {
    public:

      StructureException(const TubeSlice& x1, const TubeSlice& x2);
      StructureException(const TubeVector& x, int slice_index);
      StructureException(const TubeVector& x1, const TubeVector& x2);
      
      static void check(const TubeVector& x1, const TubeVector& x2);
      static void check(const TubeVector& x, int slice_index);
  };
}

#endif