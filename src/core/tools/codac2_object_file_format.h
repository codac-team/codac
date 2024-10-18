/** 
 *  \file codac2_object_file_format.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include <string>
#include "codac2_IntervalVector.h"

namespace codac2
{
  void export_to_ObjectFileFormat(const std::list<IntervalVector>& l, const std::string& file_name, const std::string& color = "black");
}