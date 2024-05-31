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
#include <codac2_CtcInverse.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInverse_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_CtcInverse(py::module& m, const std::string& export_name, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcInverse_<T>> exported(m, export_name.c_str(), pyctc, CTCINVERSE_MAIN);
  exported

    .def(py::init<const AnalyticFunction<OpValue<T>>&, const T&>())

    .def("contract", &CtcInverse_<T>::contract,
      "todo",
      "x"_a)
  ;
}