/** 
 *  \file
 *  Codac binding (unsupported)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <codac2_Interval.h>

using namespace codac2;
namespace py = pybind11;


PYBIND11_MODULE(_unsupported, m)
{
  m.doc() = "Python binding of Codac (unsupported)";

}