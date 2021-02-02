/** 
 *  DynamicalItem class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_DynamicalItem.h"

using namespace std;
using namespace ibex;

namespace codac
{
  DynamicalItem::~DynamicalItem()
  {

  }
  
  bool DynamicalItem::valid_tdomain(const Interval& tdomain)
  {
    return !tdomain.is_empty() && !tdomain.is_unbounded() && !tdomain.is_degenerated();
  }
}