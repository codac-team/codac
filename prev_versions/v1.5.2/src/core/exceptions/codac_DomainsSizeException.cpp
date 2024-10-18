/** 
 *  DomainsSizeException class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_DomainsSizeException.h"

using namespace std;

namespace codac
{
  DomainsSizeException::DomainsSizeException(const string& ctc_name)
    : Exception(ctc_name + "::contract",
                "domains not consistent by their dimensions (all vector should be of same dimension)\n")
  {

  }
}