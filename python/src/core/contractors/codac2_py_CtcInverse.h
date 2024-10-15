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
#include <codac2_CtcInverse.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInverse_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_CtcInverse(py::module& m, const std::string& export_name, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcInverse_<T>> exported(m, export_name.c_str(), pyctc, CTCINVERSE_MAIN);

  exported
    .def(py::init<const AnalyticFunction<OpValue<T,IntervalMatrix>>&, const T&, bool>(),
      "f"_a, "y"_a, "with_centered_form"_a = true,
      CTCINVERSE_Y_CTCINVERSE_CONST_ANALYTICFUNCTION_TYPENAME_WRAPPER_Y_DOMAIN_REF_CONST_Y_REF_BOOL_BOOL);

  if constexpr(std::is_same_v<T,IntervalVector>) // separators only associated with interval vectors
  {
    exported
    .def(py::init<const AnalyticFunction<OpValue<T,IntervalMatrix>>&, const pyCtcIntervalVector&, bool>(),
      "f"_a, "c"_a, "with_centered_form"_a = true,
      CTCINVERSE_Y_CTCINVERSE_CONST_ANALYTICFUNCTION_TYPENAME_WRAPPER_Y_DOMAIN_REF_CONST_C_REF_BOOL_BOOL);
  }

  exported

    .def(CONTRACT_BOX_METHOD(CtcInverse_<T>,
      VOID_CTCINVERSE_Y_CONTRACT_X_REF_VARIADIC_CONST))

    .def("function", &CtcInverse_<T>::function,
      CONST_ANALYTICFUNCTION_TYPENAME_WRAPPER_Y_DOMAIN_REF_CTCINVERSE_Y_FUNCTION_CONST)
    
  ;
}