/** 
 *  DynamicalItem class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_DynamicalItem.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  bool DynamicalItem::valid_domain(const ibex::Interval& domain)
  {
    return !domain.is_empty() && !domain.is_unbounded() && !domain.is_degenerated();
  }
}