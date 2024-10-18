/** 
 *  Approx Python binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <limits>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_Approx.h>
#include "codac2_py_Approx_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void _export_Approx(py::module& m, const string& class_name)
{
  py::class_<Approx<T>> exported_class(m, class_name.c_str(), APPROX_MAIN);
  exported_class
  
    .def(py::init<const T&,double>(),
      APPROX_T_APPROX_CONST_T_REF_DOUBLE,
      "x"_a, "eps"_a = std::numeric_limits<double>::epsilon()*10)

    .def("__eq__", [](const Approx<T>& x1, const T& x2) { return x1 == x2; }, 
      BOOL_OPERATOREQ_CONST_T_REF_CONST_APPROX_T_REF)

    .def("__repr__", [](const Approx<T>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_APPROX_T_REF)
  ;
}

void export_Approx(py::module& m)
{
  _export_Approx<Interval>(m, "Approx_Interval");
  _export_Approx<Vector>(m, "Approx_Vector");
  _export_Approx<IntervalVector>(m, "Approx_IntervalVector");
  _export_Approx<Matrix>(m, "Approx_Matrix");
  _export_Approx<IntervalMatrix>(m, "Approx_IntervalMatrix");
}