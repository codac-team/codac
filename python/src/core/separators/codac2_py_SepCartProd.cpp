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
#include <codac2_SepCartProd.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepCartProd_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepCartProd(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepCartProd> exported(m, "SepCartProd", pysep, SEPCARTPROD_MAIN);
  exported

    .def(py::init([](const SepBase& s1) {
          return std::make_unique<SepCartProd>(s1.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF,
      "s1"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4, const SepBase& s5) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4, const SepBase& s5, const SepBase& s6) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4, const SepBase& s5, const SepBase& s6, const SepBase& s7) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4, const SepBase& s5, const SepBase& s6, const SepBase& s7, const SepBase& s8) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy(),s8.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a, "s8"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4, const SepBase& s5, const SepBase& s6, const SepBase& s7, const SepBase& s8, const SepBase& s9) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy(),s8.copy(),s9.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a, "s8"_a, "s9"_a)

    .def(py::init([](const SepBase& s1, const SepBase& s2, const SepBase& s3, const SepBase& s4, const SepBase& s5, const SepBase& s6, const SepBase& s7, const SepBase& s8, const SepBase& s9, const SepBase& s10) {
          return std::make_unique<SepCartProd>(s1.copy(),s2.copy(),s3.copy(),s4.copy(),s5.copy(),s6.copy(),s7.copy(),s8.copy(),s9.copy(),s10.copy());
        }),
      SEPCARTPROD_SEPCARTPROD_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a, "s3"_a, "s4"_a, "s5"_a, "s6"_a, "s7"_a, "s8"_a, "s9"_a, "s10"_a)

    .def("separate", &SepCartProd::separate,
      BOXPAIR_SEPCARTPROD_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
    
  ;

  m.def("cart_prod_sep", [](const std::list<std::reference_wrapper<SepBase>>& l)
      {
        Collection<SepBase> c;
        for(const auto& si : l)
          c.add_shared_ptr(si.get().copy());
        return SepCartProd(c);
      },
    SEPCARTPROD_CART_PROD_CONST_S_REF_VARIADIC);
  // The variadic version of this function is defined in __init__.py file
}