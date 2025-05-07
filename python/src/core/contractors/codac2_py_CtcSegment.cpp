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
#include <codac2_CtcSegment.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcSegment_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcSegment(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcSegment> exported(m, "CtcSegment", pyctc, CTCSEGMENT_MAIN);
  exported

    .def(py::init<const IntervalVector&,const IntervalVector&>(),
      CTCSEGMENT_CTCSEGMENT_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "a"_a, "b"_a)

    .def(py::init<const Segment&>(),
      CTCSEGMENT_CTCSEGMENT_CONST_SEGMENT_REF,
      "ab"_a)

    .def(CONTRACT_BOX_METHOD(CtcSegment,
      VOID_CTCSEGMENT_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}