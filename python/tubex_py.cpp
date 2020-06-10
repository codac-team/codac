/** 
 *  \file
 *  Tube binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
#include <pybind11/pybind11.h>

namespace py = pybind11;

void export_Tube(py::module&m);
void export_TubeVector(py::module& m);
void export_Trajectory(py::module& m);
void export_TFunction(py::module& m);
void export_Slice(py::module& m);
void export_contractors(py::module& m);
void export_graphics(py::module& m);
void export_VIBesFig(py::module& m);
void export_ContractorNetwork(py::module& m);
void export_DataLoader(py::module& m);
void export_arithmetic(py::module& m);

PYBIND11_MODULE(tube, m)
{
  m.doc() = "python binding of tubex lib";
  export_Tube(m);
  export_TubeVector(m);
  export_Trajectory(m);
  export_contractors(m);
  export_Slice(m);
  export_TFunction(m);
  export_graphics(m);
  export_VIBesFig(m);
  export_ContractorNetwork(m);
  export_DataLoader(m);
  export_arithmetic(m);

  // m.attr("ibex_version") = _IBEX_VERSION_;
  // return m.ptr();
}