/* ============================================================================
 *  tubex-lib - AbstractTube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_ABSTRACTTUBE_H__
#define __TUBEX_ABSTRACTTUBE_H__

#include "tubex_DynamicalItem.h"

namespace tubex
{
  class AbstractTube : public DynamicalItem
  {
    public:

      // Definition
      virtual int dim() const = 0;
      virtual const ibex::Interval domain() const = 0;

      // Slices structure
      virtual int nb_slices() const = 0;
      virtual int input2index(double t) const = 0;
      virtual void sample(double t) = 0;

      // Access values
      virtual double volume() const = 0;

      // String
      virtual const std::string class_name() const = 0;
      friend std::ostream& operator<<(std::ostream& str, const AbstractTube& x);

      // Tests
      static bool share_same_slicing(const AbstractTube& x1, const AbstractTube& x2);
  };
}

#endif