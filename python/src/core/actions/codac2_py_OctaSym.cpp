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
#include "codac2_py_matlab.h"
#include "codac2_py_Ctc.h"
#include "codac2_py_Sep.h"
#include <codac2_OctaSym.h>
#include <codac2_template_tools.h>
#include <codac2_CtcAction.h>
#include <codac2_SepAction.h>
#include "codac2_py_OctaSym_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_OctaSym(py::module& m)
{
  py::class_<OctaSym> exported(m, "OctaSym", OCTASYM_MAIN);
  exported

    .def(py::init(
        [](const std::list<int_type>& l)
        {
          vector<int> v(l.size());

          size_t i = 0;
          for(const auto& li : l)
          {
            matlab::test_integer(li);
            v[i++] = matlab::input_index(li);
          }

          return std::make_unique<OctaSym>(v);
        }),
      OCTASYM_OCTASYM_CONST_VECTOR_INT_REF,
      "s"_a)

    .def("invert", &OctaSym::invert,
      OCTASYM_OCTASYM_INVERT_CONST)

    .def(py::self * py::self,
      OCTASYM_OCTASYM_OPERATORMUL_CONST_OCTASYM_REF_CONST)

    .def(py::self == py::self)

    .def(py::self != py::self)

    .def("__call__", [](const OctaSym& a, const IntervalVector& x) { return a(x); },
      INTERVALVECTOR_OCTASYM_OPERATORCALL_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

    .def("__call__", [](const OctaSym& a, const pyCtcIntervalVector& c) { return CtcAction(c.copy(),a); },
      CTCACTION_OCTASYM_OPERATORCALL_CONST_C_REF_CONST,
      "c"_a)

    .def("__call__", [](const OctaSym& a, const SepBase& s) { return SepAction(s.copy(),a); },
      SEPACTION_OCTASYM_OPERATORCALL_CONST_S_REF_CONST,
      "s"_a)

    .def("__repr__", [](const OctaSym& s) {
          std::ostringstream stream;
          stream << s;
          return stream.str(); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_OCTASYM_REF)
    
  ;
  
  py::implicitly_convertible<py::list,OctaSym>();
}