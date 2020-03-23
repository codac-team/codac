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

namespace py = pybind11;;


void export_Tube(py::module&m);
void export_TubeVector(py::module& m);
void export_Trajectory(py::module& m);
void export_Function(py::module& m);
void export_Slice(py::module& m);


PYBIND11_MODULE(tube, m)
{
  // py::module m("pyibex", "python binding of ibex core fonctionnality");
  // m.doc() = "python binding of ibex core fonctionnality";
  export_Tube(m);
  export_TubeVector(m);
  // export_Trajectory(m);
  export_Slice(m);
  export_Function(m);
  // m.attr("ibex_version") = _IBEX_VERSION_;
  // return m.ptr();

}

