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

// PEIBOS
void export_Parallelepiped(py::module& m);
void export_Zonotope(py::module& m);
void export_peibos(py::module& m);

PYBIND11_MODULE(_unsupported, m)
{
  m.doc() = "Python binding of Codac (unsupported)";

  // PEIBOS
  export_Parallelepiped(m);
  export_Zonotope(m);
  export_peibos(m);

}