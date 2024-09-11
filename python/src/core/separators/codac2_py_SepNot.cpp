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
#include <codac2_SepNot.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepNot_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepNot(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepNot> exported(m, "SepNot", pysep, SEPNOT_MAIN);
  exported

    .def(py::init<const IntervalVector&>(),
      SEPNOT_SEPNOT_CONST_INTERVALVECTOR_REF,
      "x"_a)

    .def(py::init(
        [](const SepBase& s)
        {
          return std::make_unique<SepNot>(s.copy());
        }),
      SEPNOT_SEPNOT_CONST_S_REF,
      "s"_a)

    .def("separate", &SepNot::separate,
      BOXPAIR_SEPNOT_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}