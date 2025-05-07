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
  py::class_<SepWrapper<IntervalVector>> exported_intervalvector(m, "SepWrapper_IntervalVector", pysep, SEPWRAPPER_INTERVALVECTOR_MAIN);
  exported_intervalvector

    .def(py::init<const IntervalVector&>(),
      SEPWRAPPER_INTERVALVECTOR_SEPWRAPPER_CONST_INTERVALVECTOR_REF,
      "y"_a)

    .def("separate", &SepWrapper<IntervalVector>::separate,
      BOXPAIR_SEPWRAPPER_INTERVALVECTOR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;

  py::class_<SepWrapper<PavingInOut>> exported_pavinginout(m, "SepWrapper_PavingInOut", pysep, SEPWRAPPER_INTERVALVECTOR_MAIN);
  exported_pavinginout

    .def(py::init<const PavingInOut&>(),
      SEPWRAPPER_PAVINGINOUT_SEPWRAPPER_CONST_PAVINGINOUT_REF,
      "y"_a)

    .def("separate", &SepWrapper<PavingInOut>::separate,
      BOXPAIR_SEPWRAPPER_PAVINGINOUT_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}