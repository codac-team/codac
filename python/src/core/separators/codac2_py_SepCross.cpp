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
#include <codac2_SepCross.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepCross_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepCross(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepCross> exported(m, "SepCross", pysep, SEPCROSS_MAIN);
  exported

    .def(py::init<const Segment&, const IntervalVector&>(),
      SEPCROSS_SEPCROSS_CONST_SEGMENT_REF_CONST_INTERVALVECTOR_REF,
      "e"_a, "r"_a)

    .def("separate", &SepCross::separate,
      DOC_TO_BE_DEFINED,
      "x"_a)
  ;
}