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
#include <codac2_CtcDist.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcDist_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcDist(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcDist> exported(m, "CtcDist", pyctc, CTCDIST_MAIN);
  exported

    .def(py::init<>(),
      CTCDIST_CTCDIST)

    .def(CONTRACT_BOX_METHOD(CtcDist,
      VOID_CTCDIST_CONTRACT_INTERVALVECTOR_REF_CONST))

    .def("contract", (void (CtcDist::*)(Interval&,Interval&,Interval&,Interval&,Interval&) const)&CtcDist::contract,
      VOID_CTCDIST_CONTRACT_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF_INTERVAL_REF_CONST,
      "a1"_a, "a2"_a, "b1"_a, "b2"_a, "d"_a)
  ;
}