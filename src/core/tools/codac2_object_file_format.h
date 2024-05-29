/** 
 *  \file
 *  OFF features
 *  
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <list>
#include <string>
#include "codac2_IntervalVector.h"

namespace codac2
{
  void export_to_ObjectFileFormat(const std::list<IntervalVector>& l, const std::string& file_name, const std::string& color = "black");
}