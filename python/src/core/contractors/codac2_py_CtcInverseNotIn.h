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
  using D = typename T::Domain;
  py::class_<CtcInverseNotIn<D>> exported(m, export_name.c_str(), pyctc, CTCINVERSE_MAIN);

  exported
    .def(py::init<const AnalyticFunction<T>&, const D&, bool>(),
      "f"_a, "y"_a, "with_centered_form"_a = true,
      CTCINVERSENOTIN_YX_CTCINVERSENOTIN_CONST_ANALYTICFUNCTION_TYPENAME_VALUETYPE_Y_TYPE_REF_CONST_Y_REF_BOOL);

  // disabled: if constexpr(std::is_same_v<T,VectorType>) // contractors only associated with interval vectors
  // disabled: {
  // disabled:   exported
  // disabled:   .def(py::init(
  // disabled:       [](const py::object& f, const CtcBase<IntervalVector>& c, bool with_centered_form)
  // disabled:       {
  // disabled:         return std::make_unique<CtcInverseNotIn<D>>(
  // disabled:           cast<AnalyticFunction<T>>(f),
  // disabled:           c.copy(), with_centered_form);
  // disabled:       }
  // disabled:     ),
  // disabled:     CTCINVERSENOTIN_YX_CTCINVERSENOTIN_CONST_ANALYTICFUNCTION_TYPENAME_VALUETYPE_Y_TYPE_REF_CONST_C_REF_BOOL,
  // disabled:     "f"_a, "c"_a, "with_centered_form"_a = true);
  // disabled: }

  exported

    .def(CONTRACT_BOX_METHOD(CtcInverseNotIn<D>,
      VOID_CTCINVERSENOTIN_YX_CONTRACT_X_REF_CONST))
    
  ;
}