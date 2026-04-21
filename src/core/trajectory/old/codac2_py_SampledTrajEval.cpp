/** 
 *  SampledTrajEval binding
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
#include <pybind11/numpy.h>
#include <codac2_SampledTraj.h>
#include <codac2_SampledTrajEval.h>
#include "codac2_py_SampledTrajEval_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

/*template<typename T>
void _export_SampledTrajEval(py::module& m, const string& class_name)
{
  py::class_<SampledTrajEval<T>> exported_class(m, class_name.c_str(), SAMPLEDTRAJEVAL_MAIN);
  exported_class

    .def("__repr__", [](const SampledTrajEval<T>& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_SAMPLEDTRAJEVAL_TREADONLY_REF)
  ;
}

void export_SampledTrajEval(py::module& m)
{
  //_export_SampledTrajEval<double>(m, "SampledTraj_ScalarEval");
  //_export_SampledTrajEval<Vector>(m, "SampledTraj_VectorEval");
  //_export_SampledTrajEval<Matrix>(m, "SampledTraj_MatrixEval");
}*/