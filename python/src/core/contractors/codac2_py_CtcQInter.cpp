/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_CtcQInter.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcQInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcQInter(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcQInter> exported(m, "CtcQInter", pyctc, CTCQINTER_MAIN);
  exported

    .def(py::init(
        [](unsigned int q, const CtcBase<IntervalVector>& c)
        {
          return std::make_unique<CtcQInter>(q,
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c.copy()));
        }),
      CTCQINTER_CTCQINTER_UNSIGNED_INT_CONST_C_REF,
      "q"_a, "c"_a)

    .def(py::init(
        [](unsigned int q, const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          return std::make_unique<CtcQInter>(q,
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        }),
      CTCQINTER_CTCQINTER_UNSIGNED_INT_CONST_C_REF_VARIADIC,
      "q"_a, "c1"_a, "c2"_a)

    .def(py::init(
        [](unsigned int q, const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3)
        {
          return std::make_unique<CtcQInter>(q,
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c3.copy()));
        }),
      CTCQINTER_CTCQINTER_UNSIGNED_INT_CONST_C_REF_VARIADIC,
      "q"_a, "c1"_a, "c2"_a, "c3"_a)

    .def("nb", &CtcQInter::nb,
      SIZET_CTCQINTER_NB_CONST)

    .def(CONTRACT_BOX_METHOD(CtcQInter,
      VOID_CTCQINTER_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}