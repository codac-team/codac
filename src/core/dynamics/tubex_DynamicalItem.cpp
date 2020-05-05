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
  DynamicalItem::~DynamicalItem()
  {

  }
  
  bool DynamicalItem::valid_tdomain(const Interval& tdomain)
  {
    return !tdomain.is_empty() && !tdomain.is_unbounded() && !tdomain.is_degenerated();
  }
}