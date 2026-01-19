/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_SepQInter.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepQInter_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepQInter(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepQInter> exported(m, "SepQInter", pysep, SEPQINTER_MAIN);
  exported

    .def(py::init(
        [](unsigned int q, const SepBase& s)
        {
          return std::make_unique<SepQInter>(q,s.copy());
        }),
      SEPQINTER_SEPQINTER_UNSIGNED_INT_CONST_S_REF,
      "q"_a, "s"_a)

    .def(py::init(
        [](unsigned int q, const SepBase& s1, const SepBase& s2)
        {
          return std::make_unique<SepQInter>(q,s1.copy(),s2.copy());
        }),
      SEPQINTER_SEPQINTER_UNSIGNED_INT_CONST_S_REF_VARIADIC,
      "q"_a, "s1"_a, "s2"_a)

    .def(py::init(
        [](unsigned int q, const SepBase& s1, const SepBase& s2, const SepBase& s3)
        {
          return std::make_unique<SepQInter>(q,s1.copy(),s2.copy(),s3.copy());
        }),
      SEPQINTER_SEPQINTER_UNSIGNED_INT_CONST_S_REF_VARIADIC,
      "q"_a, "s1"_a, "s2"_a, "s3"_a)

    .def("nb", &SepQInter::nb,
      SIZET_SEPQINTER_NB_CONST)

    .def("separate", &SepQInter::separate,
      BOXPAIR_SEPQINTER_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

  ;
}