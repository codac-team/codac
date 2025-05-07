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
#include <codac2_CtcCross.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcCross_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcCross(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcCross> exported_cross(m, "CtcCross", pyctc, CTCCROSS_MAIN);
  exported_cross

    .def(py::init<const Segment&, const IntervalVector&>(),
      CTCCROSS_CTCCROSS_CONST_SEGMENT_REF_CONST_INTERVALVECTOR_REF,
      "e"_a, "r"_a)

    .def(CONTRACT_BOX_METHOD(CtcCross,
      VOID_CTCCROSS_CONTRACT_INTERVALVECTOR_REF_CONST))
  ;

  py::class_<CtcNoCross> exported_nocross(m, "CtcNoCross", pyctc, CTCNOCROSS_MAIN);
  exported_nocross

    .def(py::init<const Segment&, const IntervalVector&>(),
      CTCNOCROSS_CTCNOCROSS_CONST_SEGMENT_REF_CONST_INTERVALVECTOR_REF,
      "e"_a, "r"_a)

    .def(CONTRACT_BOX_METHOD(CtcNoCross,
      VOID_CTCNOCROSS_CONTRACT_INTERVALVECTOR_REF_CONST))
  ;
}