/* ============================================================================
 *  tubex-lib - Subtube class (integration methods)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Subtube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#ifdef _OPENMP
  #include <omp.h> // for multithreading
#endif

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    Interval Subtube::integral(double t) const
    {

    }

    Interval Subtube::integral(const Interval& t) const
    {

    }
    
    Interval Subtube::integral(const Interval& t1, const Interval& t2) const
    {

    }
    
    pair<Interval,Interval> Subtube::partialIntegral(const Interval& t) const
    {

    }
    
    pair<Interval,Interval> Subtube::partialIntegral(const Interval& t1, const Interval& t2) const
    {

    }

  // Protected methods

    void Subtube::computePartialPrimitive(bool build_from_leafs) const
    {

    }
    
    void Subtube::flagFuturePrimitiveComputation() const // todo: slice index?
    {

    }

    bool Subtube::primitiveUpdateNeeded() const
    {
      return m_primitive_computation_needed;
    }
    
    pair<Interval,Interval> Subtube::getPartialPrimitiveValue(const Interval& t) const
    {

    }
}