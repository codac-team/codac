/** 
 *  \file
 *  Wall Python binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_Wall.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Wall_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Wall(py::module& m)
{
  py::class_<Wall> wall(m, "Wall", WALL_MAIN);
  wall

    .def(py::init<const Vector&,const Vector&>(), "todo")
    .def("contains", &Wall::contains, "todo", "p"_a)
    .def(py::self & py::self)
    // For MATLAB compatibility.
    //.def_static("inter", [](const Wall& x, const Wall& y) { return x&y; })
    // For MATLAB compatibility.
    .def("inter", [](const Wall& s, const Wall& y) { return s&y; })
    .def_readwrite("c1", &Wall::c1)
    .def_readwrite("c2", &Wall::c2)
  ;

  m.def("shorter_dist_to_walls", &shorter_dist_to_walls, "todo", "v_walls"_a, "p"_a, "bearing"_a);
  m.def("shorter_contact_to_walls", &shorter_contact_to_walls, "todo", "v_walls"_a, "p"_a);
}