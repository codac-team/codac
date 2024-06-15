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
#include "codac2_py_core.h"
#include <codac2_SepInverse.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepInverse_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_SepInverse(py::module& m, const std::string& export_name, py::class_<Sep,pySep>& pysep)
{
  py::class_<SepInverse<T>> exported(m, export_name.c_str(), pysep, SEPINVERSE_MAIN);
  exported

    .def(py::init<const AnalyticFunction<OpValue<T>>&,const T&,bool>(),
      "f"_a, "y"_a, "with_centered_form"_a = true)

    .def("separate", &SepInverse<T>::separate,
      "todo",
      "x"_a)
  ;
}