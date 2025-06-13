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
#include <codac2_Interval.h>
#include <codac2_TubeBase.h>
#include "codac2_py_TubeBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_TubeBase(py::module& m)
{
  py::class_<TubeBase> exported_tubebase_class(m, "TubeBase", TUBEBASE_MAIN);
  exported_tubebase_class

    .def("tdomain", &TubeBase::tdomain,
      CONST_SHARED_PTR_TDOMAIN_REF_TUBEBASE_TDOMAIN_CONST)

    .def("t0_tf", &TubeBase::t0_tf,
      INTERVAL_TUBEBASE_T0_TF_CONST)

  ;
}
