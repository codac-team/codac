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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <codac2_SepCtcBoundary.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepCtcBoundary_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepCtcBoundary(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepCtcBoundary> exported(m, "SepCtcBoundary", pysep, SEPCTCBOUNDARY_MAIN);
  exported

    .def(py::init(
        [](const CtcBase<IntervalVector>& ctc_boundary, const std::function<BoolInterval(const Vector&)>& inside_test)
        {
          return std::make_unique<SepCtcBoundary>(ctc_boundary.copy(),inside_test);
        }),
      SEPCTCBOUNDARY_SEPCTCBOUNDARY_CONST_C_REF_CONST_FUNCTION_BOOLINTERVAL_CONST_VECTOR_REF__REF,
      "ctc_boundary"_a, "inside_test"_a)

    .def("separate", &SepCtcBoundary::separate,
      BOXPAIR_SEPCTCBOUNDARY_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}