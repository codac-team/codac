/** 
 *  CtcConstell class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "codac_CtcConstell.h"

using namespace std;
using namespace ibex;

namespace codac
{
  CtcConstell::CtcConstell(const vector<IntervalVector>& map)
    : Ctc(2), m_map(map)
  {
    // todo: binary tree for logarithmic complexity
  }

  CtcConstell::CtcConstell(const list<IntervalVector>& map)
    : Ctc(2)
  {
    for(const auto& b : map)
      m_map.push_back(b);
    // todo: binary tree for logarithmic complexity
  }

  CtcConstell::~CtcConstell()
  {

  }

  void CtcConstell::contract(IntervalVector &a)
  {
    assert(a.size() == 2);
    IntervalVector union_result(2, Interval::EMPTY_SET);

    for(const auto& mj : m_map)
      union_result |= a & mj.subvector(0,1);
    a = union_result;
  }
}