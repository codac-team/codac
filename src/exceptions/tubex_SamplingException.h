/* ============================================================================
 *  tubex-lib - SamplingException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_SAMPLINGEXCEPTION_H__
#define __TUBEX_SAMPLINGEXCEPTION_H__

#include "tubex_Exception.h"
#include "tubex_TubeSlice.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /**
   * \brief Structure error exception.
   *
   * Thrown when two tubes do not share
   * the same data structure.
   */
  class SamplingException : public Exception
  {
    public:

      SamplingException(const TubeSlice& x1, const TubeSlice& x2);
      SamplingException(const TubeVector& x, int slice_index);
      SamplingException(const TubeVector& x1, const TubeVector& x2);
      
      static void check(const TubeVector& x1, const TubeVector& x2);
      static void check(const TubeVector& x, int slice_index);
  };
}

#endif