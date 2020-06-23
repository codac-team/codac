/** 
 *  \file
 *  Figure Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "tubex_Figure.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_Figure_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Figure(py::module& m)
{
  py::class_<Figure> figure(m, "Figure", FIGURE_MAIN);
  figure

    .def("view_box", &Figure::view_box,
      FIGURE_CONSTINTERVALVECTOR_VIEW_BOX)
  ;
}