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
#include <codac2_TSlice.h>
#include <codac2_SlicedTubeBase.h>
#include "codac2_py_TSlice_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_TSlice(py::module& m)
{
  py::class_<TSlice,Interval,std::shared_ptr<TSlice>> exported_tslice_class(m, "TSlice", TSLICE_MAIN);
  exported_tslice_class

    .def("is_gate", &TSlice::is_gate,
      BOOL_TSLICE_IS_GATE_CONST)

    .def("slices", &TSlice::slices,
      CONST_MAP_CONST_SLICEDTUBEBASE_PTRSHARED_PTR_SLICEBASE_REF_TSLICE_SLICES_CONST)

    .def(py::self == py::self,
      BOOL_TSLICE_OPERATOREQ_CONST_TSLICE_REF_CONST,
      "x"_a)

    .def(py::self != py::self,
      "x"_a)

  ;
}
