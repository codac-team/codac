/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_SepPolygon.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepPolygon_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepPolygon(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepPolygon> exported(m, "SepPolygon", pysep, SEPPOLYGON_MAIN);
  exported

    .def(py::init<const Polygon&>(),
      SEPPOLYGON_SEPPOLYGON_CONST_POLYGON_REF,
      "p"_a)

    .def("separate", &SepPolygon::separate,
      BOXPAIR_SEPPOLYGON_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
  
  py::implicitly_convertible<Polygon,SepPolygon>();
}