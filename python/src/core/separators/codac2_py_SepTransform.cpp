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

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepTransform(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  using FNC = AnalyticFunction<VectorOpValue>;

  py::class_<SepTransform> exported(m, "SepTransform", pysep, SEPTRANSFORM_MAIN);
  exported

    .def(py::init(
        [](const SepBase& s, const py::object& f, const py::object& f_inv)
        {
          py::object _f = f.attr("f"), _f_inv = f_inv.attr("f");
          assert_release(py::isinstance<FNC>(_f) && py::isinstance<FNC>(_f_inv)
            && "SepTransform: provided arguments are not vectorial AnalyticFunctions");

          return std::make_unique<SepTransform>(s.copy(), _f.cast<FNC>(), _f_inv.cast<FNC>());
        }),
      SEPTRANSFORM_SEPTRANSFORM_CONST_S_REF_CONST_ANALYTICFUNCTION_VECTOROPVALUE_REF_CONST_ANALYTICFUNCTION_VECTOROPVALUE_REF,
      "s"_a, "f"_a, "f_inv"_a)

    .def("separate", &SepTransform::separate,
      BOXPAIR_SEPTRANSFORM_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}