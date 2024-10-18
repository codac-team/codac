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
#include <codac2_template_tools.h>
#include <codac2_CtcCartProd.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcCartProd_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcCartProd(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcCartProd> exported(m, "CtcCartProd", pyctc, CTCCARTPROD_MAIN);
  exported

    .def(py::init([](const CtcBase<IntervalVector>& c1) {
          return std::make_unique<CtcCartProd>(c1.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF,
      "c1"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4, const CtcBase<IntervalVector>& c5) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy(),c5.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a, "c5"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4, const CtcBase<IntervalVector>& c5, const CtcBase<IntervalVector>& c6) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy(),c5.copy(),c6.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a, "c5"_a, "c6"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4, const CtcBase<IntervalVector>& c5, const CtcBase<IntervalVector>& c6, const CtcBase<IntervalVector>& c7) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy(),c5.copy(),c6.copy(),c7.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a, "c5"_a, "c6"_a, "c7"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4, const CtcBase<IntervalVector>& c5, const CtcBase<IntervalVector>& c6, const CtcBase<IntervalVector>& c7, const CtcBase<IntervalVector>& c8) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy(),c5.copy(),c6.copy(),c7.copy(),c8.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a, "c5"_a, "c6"_a, "c7"_a, "c8"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4, const CtcBase<IntervalVector>& c5, const CtcBase<IntervalVector>& c6, const CtcBase<IntervalVector>& c7, const CtcBase<IntervalVector>& c8, const CtcBase<IntervalVector>& c9) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy(),c5.copy(),c6.copy(),c7.copy(),c8.copy(),c9.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a, "c5"_a, "c6"_a, "c7"_a, "c8"_a, "c9"_a)

    .def(py::init([](const CtcBase<IntervalVector>& c1, const CtcBase<IntervalVector>& c2, const CtcBase<IntervalVector>& c3, const CtcBase<IntervalVector>& c4, const CtcBase<IntervalVector>& c5, const CtcBase<IntervalVector>& c6, const CtcBase<IntervalVector>& c7, const CtcBase<IntervalVector>& c8, const CtcBase<IntervalVector>& c9, const CtcBase<IntervalVector>& c10) {
          return std::make_unique<CtcCartProd>(c1.copy(),c2.copy(),c3.copy(),c4.copy(),c5.copy(),c6.copy(),c7.copy(),c8.copy(),c9.copy(),c10.copy());
        }),
      CTCCARTPROD_CTCCARTPROD_CONST_C_REF_VARIADIC,
      "c1"_a, "c2"_a, "c3"_a, "c4"_a, "c5"_a, "c6"_a, "c7"_a, "c8"_a, "c9"_a, "c10"_a)

    .def(CONTRACT_BOX_METHOD(CtcCartProd,
      VOID_CTCCARTPROD_CONTRACT_INTERVALVECTOR_REF_CONST))
    
  ;

  m.def("cart_prod_ctc", [](const std::list<std::reference_wrapper<CtcBase<IntervalVector>>>& l)
      {
        Collection<CtcBase<IntervalVector>> c;
        for(const auto& ci : l)
          c.add_shared_ptr(ci.get().copy());
        return CtcCartProd(c);
      },
    CTCCARTPROD_CART_PROD_CONST_C_REF_VARIADIC);
  // The variadic version of this function is defined in __init__.py file
}