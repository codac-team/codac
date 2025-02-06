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
#include <codac2_SepTransform.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepTransform_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepTransform(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepTransform> exported(m, "SepTransform", pysep, SEPTRANSFORM_MAIN);
  exported

    .def(py::init(
        [](const SepBase& s, const py::object& f, const py::object& f_inv)
        {
          return std::make_unique<SepTransform>(
            s.copy(),
            cast<AnalyticFunction<VectorType>>(f),
            cast<AnalyticFunction<VectorType>>(f_inv));
        }),
      SEPTRANSFORM_SEPTRANSFORM_CONST_S_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF,
      "s"_a, "f"_a, "f_inv"_a)

    .def("separate", &SepTransform::separate,
      BOXPAIR_SEPTRANSFORM_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}