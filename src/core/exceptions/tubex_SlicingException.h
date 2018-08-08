/* ============================================================================
 *  tubex-lib - SlicingException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_SLICINGEXCEPTION_H__
#define __TUBEX_SLICINGEXCEPTION_H__

#include "tubex_Exception.h"
#include "tubex_Slice.h"
#include "tubex_TubeVector.h"
#include "tubex_AbstractTube.h"

namespace tubex
{
  /**
   * \brief Structure error exception.
   *
   * Thrown when two tubes do not share
   * the same data structure.
   */
  class SlicingException : public Exception
  {
    public:

      SlicingException(const Slice& x1, const Slice& x2);
      //SlicingException(const TubeVector& x, int slice_index);
      //SlicingException(const TubeVector& x1, const TubeVector& x2);
      SlicingException(const AbstractTube& x, int slice_index);
      SlicingException(const AbstractTube& x1, const AbstractTube& x2);
      
      //static void check(const TubeVector& x1, const TubeVector& x2);
      //static void check(const TubeVector& x, int slice_index);
      static void check(const AbstractTube& x, int slice_index);
      static void check(const AbstractTube& x1, const AbstractTube& x2);
  };
}

#endif