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
#include <codac2_SepInter.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepInter(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepInter> exported(m, "SepInter", pysep, SEPINTER_MAIN);
  exported

    .def(py::init(
        [](const SepBase& s)
        {
          return std::make_unique<SepInter>(s.copy());
        }),
      SEPINTER_SEPINTER_CONST_S_REF,
      "s"_a)

    .def(py::init(
        [](const SepBase& s1, const SepBase& s2)
        {
          return std::make_unique<SepInter>(s1.copy(),s2.copy());
        }),
      SEPINTER_SEPINTER_CONST_S_REF_VARIADIC,
      "s1"_a, "s2"_a)

    .def("nb", &SepInter::nb,
      SIZET_SEPINTER_NB_CONST)

    .def("separate", &SepInter::separate,
      BOXPAIR_SEPINTER_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

    .def("__iand__", [](SepInter& s1, const SepBase& s2)
        {
          s1 &= s2.copy();
          return s1;
        },
      SEPINTER_REF_SEPINTER_OPERATORINTEREQ_CONST_S_REF)
  ;
}