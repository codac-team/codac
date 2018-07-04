/* ============================================================================
 *  tubex-lib - DimensionException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef DimensionException_HEADER
#define DimensionException_HEADER

#include "ibex_Fnc.h"
#include "tubex_Exception.h"
#include "tubex_DynamicalItem.h"
#include "tubex_VibesFigure_Tube.h"

namespace tubex
{
  /**
   * \brief Dimension error exception.
   *
   * Thrown when two vector objects are not of same dimension.
   */
  class DimensionException : public Exception
  {
    public:

      DimensionException(int dim, const std::string& comment = "");
      DimensionException(const ibex::Vector& x1, const ibex::Vector& x2);
      DimensionException(const DynamicalItem& x, int i);
      DimensionException(const DynamicalItem& x, const ibex::IntervalVector& box);
      DimensionException(const DynamicalItem& x, const ibex::Fnc& f);
      DimensionException(const DynamicalItem& x1, const DynamicalItem& x2);
      DimensionException(const VibesFigure_Tube& fig, const DynamicalItem& x);

      static void check(int dim);
      static void checkScalar(int dim);
      static void check(const ibex::Vector& x1, const ibex::Vector& x2);
      static void check(const DynamicalItem& x, int i);
      static void check(const DynamicalItem& x, const ibex::IntervalVector& box);
      static void check(const DynamicalItem& x, const ibex::Fnc& f);
      static void check(const DynamicalItem& x1, const DynamicalItem& x2);
      static void check(const VibesFigure_Tube& fig, const DynamicalItem& x);
  };
}

#endif