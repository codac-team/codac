/** 
 *  \file
 *  Ctc class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <memory>

namespace codac2
{
  class Ctc
  {
    public:

      virtual std::shared_ptr<Ctc> copy() const = 0;
  };
}