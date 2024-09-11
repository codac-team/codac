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
#include <codac2_SepWrapper.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepWrapper_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepWrapper(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepWrapper_<IntervalVector>> exported(m, "SepWrapper", pysep, SEPWRAPPER__INTERVALVECTOR_MAIN);
  exported

    .def(py::init<const IntervalVector&>(),
      SEPWRAPPER__INTERVALVECTOR_SEPWRAPPER__CONST_INTERVALVECTOR_REF,
      "vertices"_a)

    .def("separate", &SepWrapper_<IntervalVector>::separate,
      BOXPAIR_SEPWRAPPER__INTERVALVECTOR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}