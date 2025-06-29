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
#include "codac2_py_Ctc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_CtcInverse(py::module& m, const std::string& export_name, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  using D = typename T::Domain;
  using C = CtcInverse<D,IntervalVector>; // comma cannot be interpreted by the following macro

  py::class_<C> exported(m, export_name.c_str(), pyctc, CTCINVERSE_MAIN);

  exported
    .def(py::init<const AnalyticFunction<T>&, const D&, bool>(),
      "f"_a, "y"_a, "with_centered_form"_a = true,
      CTCINVERSE_YX_CTCINVERSE_CONST_ANALYTICFUNCTION_TYPENAME_EXPRTYPE_Y_TYPE_REF_CONST_Y_REF_BOOL_BOOL);

  if constexpr(std::is_same_v<T,VectorType>) // contractors only associated with interval vectors
  {
    exported
    .def(py::init(
        [](const py::object& f, const CtcBase<IntervalVector>& c, bool with_centered_form)
        {
          return std::make_unique<C>(
            cast<AnalyticFunction<T>>(f),
            c.copy(), with_centered_form);
        }
      ),
      CTCINVERSE_YX_CTCINVERSE_CONST_ANALYTICFUNCTION_TYPENAME_EXPRTYPE_Y_TYPE_REF_CONST_C_REF_BOOL_BOOL,
      "f"_a, "c"_a, "with_centered_form"_a = true);
  }

  exported

    .def(CONTRACT_BOX_METHOD(C,
      VOID_CTCINVERSE_YX_CONTRACT_X_REF_VARIADIC_CONST))
    
    .def("contract_tube", [](const C& c, py::object& x1) -> py::object&
        {
          if(!is_instance<SlicedTube<IntervalVector>>(x1)) {
            assert_release("contract_tube: invalid tube type");
          }

          c.contract(cast<SlicedTube<IntervalVector>>(x1));
          return x1;
        },
      VIRTUAL_VOID_CTCBASE_X_CONTRACT_SLICEDTUBE_X_REF_VARIADIC_CONST,
      "x"_a)

    .def("function", &C::function,
      CONST_ANALYTICFUNCTION_TYPENAME_EXPRTYPE_Y_TYPE_REF_CTCINVERSE_YX_FUNCTION_CONST)
    
  ;
}