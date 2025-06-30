/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_Ctc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_CtcUnion.h"
#include "codac2_py_CtcUnion_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_CtcInter.h"
#include "codac2_py_CtcInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector> export_CtcIntervalVector(py::module& m)
{
  py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector> py_ctc_iv(m, "Ctc_IntervalVector_");
  py_ctc_iv

    .def(py::init<Index>(),
      CTCBASE_X_CTCBASE_INDEX)

    .def("size", &CtcBase<IntervalVector>::size,
      INDEX_CTCBASE_X_SIZE_CONST)

    .def("copy", [](const CtcBase<IntervalVector>& c)
        {
          return &c;
        },
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    // Intersection of contractors

    .def("__and__", [](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          return CtcInter<IntervalVector>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        },
      CTCINTERTYPE_TYPENAMEC1_CONTRACTEDTYPES_CTC_OPERATORAND_CONST_C1_REF_CONST_C2_REF)

    .def("__and__", [](const CtcBase<IntervalVector>& c1, const IntervalVector& x2)
        {
          auto c2 = CtcWrapper(x2);
          return CtcInter<IntervalVector>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        },
      CTCINTER_INTERVALVECTOR_OPERATORAND_CONST_INTERVALVECTOR_REF_CONST_C2_REF)

    .def("__rand__", [](const CtcBase<IntervalVector>& c1, const IntervalVector& x2)
        {
          auto c2 = CtcWrapper(x2);
          return CtcInter<IntervalVector>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        },
      CTCINTER_INTERVALVECTOR_OPERATORAND_CONST_INTERVALVECTOR_REF_CONST_C2_REF)

    // Union of contractors

    .def("__or__", [](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2)
        {
          return CtcUnion<IntervalVector>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        },
      CTCUNIONTYPE_TYPENAMEC1_CONTRACTEDTYPES_CTC_OPERATOROR_CONST_C1_REF_CONST_C2_REF)

    .def("__or__", [](const CtcBase<IntervalVector>& c1, const IntervalVector& x2)
        {
          auto c2 = CtcWrapper(x2);
          return CtcUnion<IntervalVector>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        },
      CTCUNION_INTERVALVECTOR_OPERATOROR_CONST_INTERVALVECTOR_REF_CONST_C2_REF)

    .def("__ror__", [](const CtcBase<IntervalVector>& c1, const IntervalVector& x2)
        {
          auto c2 = CtcWrapper(x2);
          return CtcUnion<IntervalVector>(
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c1.copy()),
            std::dynamic_pointer_cast<CtcBase<IntervalVector>>(c2.copy()));
        },
      CTCUNION_INTERVALVECTOR_OPERATOROR_CONST_INTERVALVECTOR_REF_CONST_C2_REF)

  ;

  return py_ctc_iv;
}