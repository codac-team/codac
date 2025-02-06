/** 
 *  codac2_OutputFigure2D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure2D.h"
#include "codac2_OutputFigure2D.h"

using namespace std;
using namespace codac2;


const Index& OutputFigure2D::i() const
{
  return _fig.i();
}

const Index& OutputFigure2D::j() const
{
  return _fig.j();
}