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
#include <codac2_template_tools.h>
#include <codac2_SepCartProd.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepCartProd_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepCartProd(py::module& m, py::class_<Sep,pySep>& pysep)
{
  py::class_<SepCartProd> exported(m, "SepCartProd", pysep, SEPCARTPROD_MAIN);
  exported

    .def(py::init([](const Sep& s1) {
          return std::make_unique<SepCartProd>(s1.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF,
      "s1"_a)

    .def(py::init([](const Sep& s1, const Sep& s2) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4, const Sep& s5) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4, const Sep& s5, const Sep& s6) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4, const Sep& s5, const Sep& s6, const Sep& s7) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4, const Sep& s5, const Sep& s6, const Sep& s7, const Sep& s8) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy(),s8.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a, "s8"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4, const Sep& s5, const Sep& s6, const Sep& s7, const Sep& s8, const Sep& s9) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy(),s8.copy(),s9.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a, "s8"_a, "s9"_a)

    .def(py::init([](const Sep& s1, const Sep& s2, const Sep& s3, const Sep& s4, const Sep& s5, const Sep& s6, const Sep& s7, const Sep& s8, const Sep& s9, const Sep& s10) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy(),s8.copy(),s9.copy(),s10.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a, "s8"_a, "s9"_a, "s10"_a)

    .def("separate", &SepCartProd::separate,
      BOXPAIR_SEPCARTPROD_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
    
  ;

  m.def("cart_prod_sep", [](const std::list<std::reference_wrapper<Sep>>& l)
      {
        Collection<Sep> c;
        for(const auto& si : l)
          c.add_shared_ptr(si.get().copy());
        return SepCartProd(c);
      },
    SEPCARTPROD_CART_PROD_CONST_S_REF_VARIADIC);
  // The variadic version of this function is defined in __init__.py file
}