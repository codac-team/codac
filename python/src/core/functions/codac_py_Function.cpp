/** 
 *  \file
 *  Function Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac_Function.h"
#include "ibex_SyntaxError.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
// todo: #include "codac_py_Function_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


typedef const char* cc_ptr;

void export_Function(py::module& m)
{
  py::register_exception_translator([](exception_ptr p)
  {
    try {
      if(p) rethrow_exception(p);
    }
    catch(const ibex::SyntaxError &e) {
      stringstream ss;
      ss << "IBEX function parser error:" << endl;
      ss << e;
      PyErr_SetString(PyExc_RuntimeError, ss.str().c_str());
    }
  });

  py::class_<Function> function(m, "Function");
  function

  // Definition

    .def(py::init<cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr,cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>())
    .def(py::init<cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr,cc_ptr>())
    
    .def("nb_arg", &Function::nb_arg)
    .def("diff", &Function::diff)
    .def("__repr__", [](const Function& x) { ostringstream str; str << x; return str.str(); })
  
  // Evaluations

    .def("eval", (Interval (Function::*)(const IntervalVector& x) const) &Function::eval)
    .def("eval", [](Function& f, Interval& x) { return f.eval_vector(IntervalVector(1, x)); })
    //.def("eval_matrix", &Function::eval_matrix)
    .def("eval_vector", (IntervalVector (Function::*)(const IntervalVector& x) const) &Function::eval_vector)
    
    .def("backward", (bool (Function::*)(const Interval&, IntervalVector&) const) &Function::backward)
    .def("backward", (bool (Function::*)(const IntervalVector&, IntervalVector&) const) &Function::backward)
    .def("backward", (bool (Function::*)(const IntervalMatrix&, IntervalVector&) const) &Function::backward)
  ;
}