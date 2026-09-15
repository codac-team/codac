/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_template_tools.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_OctaSym.h>
#include <codac2_SepImage.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepImage_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepImage(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepImage> exported(m, "SepImage", pysep, SEPIMAGE_MAIN);
  exported

    .def(py::init(
        [](const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const CtcBase<IntervalVector>& ctc_init, double epsilon_pave)
        {
          return std::make_unique<SepImage>(f,psi_0, Sigma, epsilon, ctc_init.copy(), epsilon_pave);
        }),
      SEPIMAGE_SEPIMAGE_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_OCTASYM_REF_DOUBLE_CONST_C_REF_DOUBLE,
      "f"_a, "psi_0"_a, "Sigma"_a, "epsilon"_a, "ctc_init"_a, "epsilon_pave"_a = -1)

    .def("separate", &SepImage::separate,
      BOXPAIR_SEPIMAGE_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
    
  ;
}