/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_SepTest.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepTest_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepTest(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepTest> exported(m, "SepTest", pysep, SEPTEST_MAIN);
  exported

    .def(py::init<const SepBase&>(),
      SEPTEST_SEPTEST_CONST_S_REF,
      "s"_a)

    .def("separate", &SepTest::separate,
      BOXPAIR_SEPTEST_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)

  ;
}