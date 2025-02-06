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
#include <codac2_CtcInter.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcInter<IntervalVector>> exported(m, "CtcInter", pyctc, CTCINTER_MAIN);
  exported

    .def(py::init<Index>(),
      CTCINTER_X_CTCINTER_INDEX,
      "n"_a)

    .def(py::init(
        [](const CtcBase<IntervalVector>& c)
        {
          return std::make_unique<CtcInter<IntervalVector>>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c.copy()));
        }),
      CTCINTER_X_CTCINTER_CONST_C_REF,
      "c"_a)

    .def(py::init(
        [](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          return std::make_unique<CtcInter<IntervalVector>>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        }),
      CTCINTER_X_CTCINTER_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a)

    .def(CONTRACT_BOX_METHOD(CtcInter<IntervalVector>,
      VOID_CTCINTER_X_CONTRACT_X_REF_CONST))

    .def("__iand__", [](CtcInter<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          c1 &= std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy());
          return c1;
        },
      CTCINTER_X_REF_CTCINTER_X_OPERATORANDEQ_CONST_C_REF)
  ;
}