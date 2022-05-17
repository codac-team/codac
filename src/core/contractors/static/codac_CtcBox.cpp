/** 
 *  CtcBox class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcBox.h"

using namespace std;

namespace codac
{
  void CtcBox::contract(IntervalVector& x)
  {
    // Checking the dimension of the input box
    assert(_b.size() == x.size());

    // Contracting the input box around the support box
    x &= _b;
  }
}