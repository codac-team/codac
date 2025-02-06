/** 
 *  AnalyticTraj Python binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <limits>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <codac2_AnalyticTraj.h>
#include <codac2_SampledTraj.h>
#include "codac2_py_AnalyticTraj_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_TrajBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_TrajBase.h"
#include "codac2_Traj_operator.h"
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename O>
void _export_AnalyticTraj(py::module& m, const string& class_name)
{
  py::class_<AnalyticTraj<O>> exported_class(m, class_name.c_str(), ANALYTICTRAJ_MAIN);
  export_TrajBase<AnalyticTraj<O>>(exported_class);

  exported_class

    .def(py::init<const AnalyticFunction<O>&,const Interval&>(),
      ANALYTICTRAJ_OS_ANALYTICTRAJ_CONST_ANALYTICFUNCTION_O_REF_CONST_INTERVAL_REF,
      "f"_a, "tdomain"_a)

    .def("__call__", [](const AnalyticTraj<O>& x, double t)
        {
          return x(t);
        },
      VIRTUAL_S_ANALYTICTRAJ_OS_OPERATORCALL_DOUBLE_CONST,
      "t"_a)

    .def("__call__", [](const AnalyticTraj<O>& x, const Interval& t)
        {
          return x(t);
        },
      VIRTUAL_WRAPPER_S_DOMAIN_ANALYTICTRAJ_OS_OPERATORCALL_CONST_INTERVAL_REF_CONST,
      "t"_a)

  ;
}

void export_AnalyticTraj(py::module& m)
{
  _export_AnalyticTraj<ScalarType>(m, "AnalyticTraj_Scalar");
  _export_AnalyticTraj<VectorType>(m, "AnalyticTraj_Vector");
}