/** 
 *  codac2_OutputFigure.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure.h"
#include "codac2_OutputFigure.h"

using namespace std;
using namespace codac2;


const size_t& OutputFigure::i() const
{
  return _fig.axes()[0].dim_id;
}

const size_t& OutputFigure::j() const
{
  return _fig.axes()[1].dim_id;
}