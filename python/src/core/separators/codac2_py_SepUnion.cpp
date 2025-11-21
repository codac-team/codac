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
#include <codac2_SepUnion.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepUnion_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepUnion(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepUnion> exported(m, "SepUnion", pysep, SEPUNION_MAIN);
  exported

    .def(py::init(
        [](const SepBase& s)
        {
          return std::make_unique<SepUnion>(s.copy());
        }),
      SEPUNION_SEPUNION_CONST_S_REF,
      "s"_a)

    .def(py::init(
        [](const SepBase& s1, const SepBase& s2)
        {
          return std::make_unique<SepUnion>(s1.copy(),s2.copy());
        }),
      SEPUNION_SEPUNION_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a)

    .def("separate", &SepUnion::separate,
      BOXPAIR_SEPUNION_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

    .def("__ior__", [](SepUnion& s1, const SepBase& s2)
        {
          s1 |= s2.copy();
          return s1;
        },
      SEPUNION_REF_SEPUNION_OPERATORUNIONEQ_CONST_S_REF)
  ;
}