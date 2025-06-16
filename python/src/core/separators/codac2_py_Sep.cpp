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
#include "codac2_py_Sep.h"
#include "codac2_py_Sep_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_SepInter.h"
#include "codac2_py_SepInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_SepUnion.h"
#include "codac2_py_SepUnion_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_SepNot.h"
#include "codac2_py_SepNot_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<SepBase,pySep> export_Sep(py::module& m)
{
  py::class_<BoxPair> exported_BoxPair(m, "BoxPair", BOXPAIR_MAIN);
  exported_BoxPair
  
    .def(py::init(
        [](const IntervalVector& inner, const IntervalVector& outer)
        {
          return std::make_unique<BoxPair>(inner, outer);
        }),
      BOXPAIR_BOXPAIR_CONST_INTERVALVECTOR_REF_CONST_INTERVALVECTOR_REF,
      "inner"_a, "outer"_a)

    .def("__iter__", [](const BoxPair &x)
        {
          return py::make_iterator(x._bp.begin(), x._bp.end());
        },
      py::keep_alive<0, 1>() /*  keep object alive while iterator exists */)

    .def("__repr__", [](const BoxPair& x) {
          std::ostringstream stream;
          stream << x;
          return std::string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_BOXPAIR_REF)
  ;

  py::class_<SepBase,pySep> py_sep(m, "SepBase");
  py_sep

    .def(py::init<Index>(),
      SEPBASE_SEPBASE_INDEX)

    .def("size", &SepBase::size,
      INDEX_SEPBASE_SIZE_CONST)

    .def("copy", [](SepBase& s)
        {
          return &s;
        },
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    // Intersection of separators

    .def("__and__", [](const SepBase& s1, const SepBase& s2)
        {
          return SepInter(s1.copy(),s2.copy());
        },
      SEPINTER_OPERATORAND_CONST_S1_REF_CONST_S2_REF)

    .def("__and__", [](const SepBase& s1, const IntervalVector& x2)
        {
          auto s2 = SepWrapper(x2);
          return SepInter(s1.copy(),s2.copy());
        },
      SEPINTER_OPERATORAND_CONST_INTERVALVECTOR_REF_CONST_S2_REF)

    .def("__rand__", [](const SepBase& s1, const IntervalVector& x2)
        {
          auto s2 = SepWrapper(x2);
          return SepInter(s1.copy(),s2.copy());
        },
      SEPINTER_OPERATORAND_CONST_INTERVALVECTOR_REF_CONST_S2_REF)

    // Union of separators

    .def("__or__", [](const SepBase& s1, const SepBase& s2)
        {
          return SepUnion(s1.copy(),s2.copy());
        },
      SEPUNION_OPERATOROR_CONST_S1_REF_CONST_S2_REF)

    .def("__or__", [](const SepBase& s1, const IntervalVector& x2)
        {
          auto s2 = SepWrapper(x2);
          return SepUnion(s1.copy(),s2.copy());
        },
      SEPUNION_OPERATOROR_CONST_INTERVALVECTOR_REF_CONST_S2_REF)

    .def("__ror__", [](const SepBase& s1, const IntervalVector& x2)
        {
          auto s2 = SepWrapper(x2);
          return SepUnion(s1.copy(),s2.copy());
        },
      SEPUNION_OPERATOROR_CONST_INTERVALVECTOR_REF_CONST_S2_REF)

    // Negation of separator

    .def("__invert__", [](const SepBase& s1)
        {
          return SepNot(s1.copy());
        },
      SEPNOT_OPERATORNOT_CONST_S_REF)

  ;

  return py_sep;
}