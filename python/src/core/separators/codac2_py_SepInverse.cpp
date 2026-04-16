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
#include <codac2_SepInverse.h>
#include <codac2_AnalyticFunction.h>
#include "codac2_Sep.h"
#include "codac2_py_Sep.h"
#include "codac2_py_SepInverse_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_SepCtcPair_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_AnalyticFunction.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepInverse(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepInverse> exported(m, "SepInverse", pysep, SEPINVERSE_MAIN);
  exported

    .def(py::init(
        [](const AnalyticFunction<VectorType>& f, const SepBase& s, bool with_centered_form)
        {
          return std::make_unique<SepInverse>(f, s.copy(), with_centered_form);
          // todo: s.copy necessary?
        }
      ),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_S_REF_BOOL,
      "f"_a, "s"_a, "with_centered_form"_a = true)

    .def(py::init<const AnalyticFunction<ScalarType>&,const Interval&,bool>(),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_TYPENAME_T_DOMAIN_REF_BOOL,
      "f"_a, "y"_a, "with_centered_form"_a = true)

    .def(py::init<const AnalyticFunction<VectorType>&,const IntervalVector&,bool>(),
      SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_TYPENAME_T_DOMAIN_REF_BOOL,
      "f"_a, "y"_a, "with_centered_form"_a = true)

    // not yet available .def(py::init<const AnalyticFunction<MatrixType>&,const IntervalMatrix&,bool>(),
    // not yet available   SEPINVERSE_SEPINVERSE_CONST_ANALYTICFUNCTION_T_REF_CONST_TYPENAME_T_DOMAIN_REF_BOOL,
    // not yet available   "f"_a, "y"_a, "with_centered_form"_a = true)

    .def("separate", &SepInverse::separate,
      BOXPAIR_SEPCTCPAIR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}