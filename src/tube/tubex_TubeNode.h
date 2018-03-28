/* ============================================================================
 *  tubex-lib - TubeNode class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef TUBENODE_HEADER
#define TUBENODE_HEADER

#include "ibex.h"

namespace tubex
{
  class TubeNode
  {
    public:

      TubeNode(const ibex::Interval& domain, const ibex::Interval& codomain);
      ~TubeNode() {};

      const ibex::Interval& domain() const;
      const ibex::Interval& codomain() const;
      const double volume() const;

    protected:

    /** Class variables **/

      ibex::Interval m_domain;
      ibex::Interval m_codomain;
      double m_volume;
  };
}

#endif