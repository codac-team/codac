/** 
 *  Codac binding (graphics)
 * ----------------------------------------------------------------------------
 *  \date       202
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_template_tools.h>
 #include <codac2_py_cast.h>
#include <codac2_Vector.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_Parallelepiped.h>
#include <codac2_peibos.h>
#include "codac2_py_peibos_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_peibos(py::module& m)
{
  m.def("PEIBOS", 
    [](const py::object& f, const py::object& psi_0, const vector<vector<int>>& generators, double epsilon)
    {
      return PEIBOS(cast<AnalyticFunction<VectorType>>(f), cast<AnalyticFunction<VectorType>>(psi_0), generators, epsilon);
    },
    VECTOR_PARALLELEPIPED_PEIBOS_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_VECTOR_INT_REF_DOUBLE,
    "f"_a, "psi_0"_a, "generators"_a, "epsilon"_a);

  m.def("PEIBOS", 
    [](const py::object& f, const py::object& psi_0, const vector<vector<int>>& generators, double epsilon, Vector offset)
    {
      return PEIBOS(cast<AnalyticFunction<VectorType>>(f), cast<AnalyticFunction<VectorType>>(psi_0), generators, epsilon, offset);
    },
    VECTOR_PARALLELEPIPED_PEIBOS_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_CONST_VECTOR_VECTOR_INT_REF_DOUBLE_CONST_VECTOR_REF,
    "f"_a, "psi_0"_a, "generators"_a, "epsilon"_a, "offset"_a);
}