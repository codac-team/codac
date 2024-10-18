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
#include <codac2_CtcInverseNotIn.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcInverseNotIn_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_CtcInverseNotIn(py::module& m, const std::string& export_name, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcInverseNotIn<T>> exported(m, export_name.c_str(), pyctc, CTCINVERSE_MAIN);

  exported
    .def(py::init<const AnalyticFunction<OpValue<T,IntervalMatrix>>&, const T&, bool>(),
      "f"_a, "y"_a, "with_centered_form"_a = true,
      CTCINVERSENOTIN_YX_CTCINVERSENOTIN_CONST_ANALYTICFUNCTION_TYPENAME_WRAPPER_Y_DOMAIN_REF_CONST_Y_REF_BOOL);

  if constexpr(std::is_same_v<T,IntervalVector>) // separators only associated with interval vectors
  {
    exported
    .def(py::init<const AnalyticFunction<OpValue<T,IntervalMatrix>>&, const pyCtcIntervalVector&, bool>(),
      "f"_a, "c"_a, "with_centered_form"_a = true,
      CTCINVERSENOTIN_YX_CTCINVERSENOTIN_CONST_ANALYTICFUNCTION_TYPENAME_WRAPPER_Y_DOMAIN_REF_CONST_C_REF_BOOL);
  }

  exported

    .def(CONTRACT_BOX_METHOD(CtcInverseNotIn<T>,
      VOID_CTCINVERSENOTIN_YX_CONTRACT_X_REF_CONST))
    
  ;
}