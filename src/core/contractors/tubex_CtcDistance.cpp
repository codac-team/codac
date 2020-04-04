/** 
 *  CtcDistance class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcDistance.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDistance::CtcDistance()
    : CtcFunction("a[2]", "b[2]", "d", "d = sqrt((a[0]-b[0])^2+(a[1]-b[1])^2)")
  {

  }

  void CtcDistance::contract(vector<Domain*>& v_domains)
  {
    assert(v_domains.size() == 3);

    assert(v_domains[0]->type() == DomainType::INTERVAL_VECTOR
        || v_domains[0]->type() == DomainType::TUBE_VECTOR);
    assert(v_domains[1]->type() == DomainType::INTERVAL_VECTOR
        || v_domains[1]->type() == DomainType::TUBE_VECTOR);
    assert(v_domains[2]->type() == DomainType::INTERVAL
        || v_domains[2]->type() == DomainType::TUBE);

    contract(v_domains);
  }
}