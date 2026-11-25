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
  py::class_<TSlice,std::shared_ptr<TSlice>> exported_tslice_class(m, "TSlice", TSLICE_MAIN);
  exported_tslice_class

    // Return a copy: Python does not enforce the constness of C++ references.
    .def("t0_tf", [](const TSlice& x) { return x.t0_tf(); })

    .def("lb", [](const TSlice& x) { return x.lb(); })
    .def("ub", [](const TSlice& x) { return x.ub(); })
    .def("mid", [](const TSlice& x) { return x.mid(); })
    .def("rad", [](const TSlice& x) { return x.rad(); })
    .def("diam", [](const TSlice& x) { return x.diam(); })
    .def("is_empty", [](const TSlice& x) { return x.is_empty(); })
    .def("is_unbounded", [](const TSlice& x) { return x.is_unbounded(); })
    .def("is_degenerated", [](const TSlice& x) { return x.is_degenerated(); })
    .def("contains", [](const TSlice& x, double t) { return x.contains(t); }, "t"_a)
    .def("intersects", [](const TSlice& x, const Interval& t) { return x.intersects(t); }, "t"_a)
    .def("is_subset", [](const TSlice& x, const Interval& t) { return x.is_subset(t); }, "t"_a)
    .def("is_superset", [](const TSlice& x, const Interval& t) { return x.is_superset(t); }, "t"_a)

    .def("is_gate", &TSlice::is_gate,
      BOOL_TSLICE_IS_GATE_CONST)

    .def("slices", &TSlice::slices,
      CONST_MAP_CONST_SLICEDTUBEBASE_PTRSHARED_PTR_SLICEBASE_REF_TSLICE_SLICES_CONST)

    .def(py::self == py::self, "x"_a)

    .def(py::self != py::self, "x"_a)

    .def(py::self == Interval(), "x"_a)

    .def(py::self != Interval(), "x"_a)

    .def("__repr__", [](const TSlice& x) {
          std::ostringstream stream;
          stream << x;
          return stream.str();
        })

  ;
}
