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
#include <codac2_template_tools.h>
#include <codac2_CtcPolygon.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcPolygon_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcPolygon(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcPolygon> exported(m, "CtcPolygon", pyctc, CTCPOLYGON_MAIN);
  exported

    .def(py::init<const Polygon&>(),
      CTCPOLYGON_CTCPOLYGON_CONST_POLYGON_REF,
      "p"_a)

    .def(CONTRACT_BOX_METHOD(CtcPolygon,
      VOID_CTCPOLYGON_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;

  py::implicitly_convertible<Polygon,CtcPolygon>();
}