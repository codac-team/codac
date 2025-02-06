/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Ellipsoid_utils.h>
#include "codac2_py_Ellipsoid_utils_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Ellipsoid_utils(py::module& m)
{
  m

    .def("stability_analysis", &codac2::stability_analysis,
      BOOLINTERVAL_STABILITY_ANALYSIS_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_UNSIGNED_INT_ELLIPSOID_REF_ELLIPSOID_REF_BOOL,
      "f"_a, "alpha_max"_a, "e"_a, "e_out"_a, "verbose"_a=false)

    .def("solve_discrete_lyapunov", &codac2::solve_discrete_lyapunov,
      MATRIX_SOLVE_DISCRETE_LYAPUNOV_CONST_MATRIX_REF_CONST_MATRIX_REF,
      "A"_a, "Q"_a)

  ;
}