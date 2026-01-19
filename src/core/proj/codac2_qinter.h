/** 
 *  \file codac2_qinter.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Clement Carbonnel, Benoit Desrochers, Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include "codac2_IntervalVector.h"

namespace codac2
{
  // Approximate Q-intersection, using Luc Jaulin's algorithm
  // Author: Clement Carbonnel, Benoit Desrochers, Simon Rohou
  // The q-intersection of n boxes corresponds to the set of all elements
  // which belong to at least q of these boxes.
  IntervalVector qinter(unsigned int q, const std::list<IntervalVector>& l);
}