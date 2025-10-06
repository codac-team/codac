/** 
 *  \file
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2021 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <codac2_Interval.h>

using namespace codac2;
namespace py = pybind11;

// figures
void export_Figure2D(py::module& m);
void export_Figure3D(py::module& m);

// styles
void export_Color(py::module& m);
void export_ColorMap(py::module& m);
void export_PavingStyle(py::module& m);
void export_StyleProperties(py::module& m);


PYBIND11_MODULE(_graphics, m)
{
  m.doc() = "Python binding of Codac (graphics)";

  // styles
  export_Color(m);
  export_ColorMap(m);
  export_PavingStyle(m);
  export_StyleProperties(m);

  // figures
  export_Figure2D(m);
  export_Figure3D(m);
}