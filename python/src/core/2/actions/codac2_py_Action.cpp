/** 
 *  \file
 *  Action Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "codac_type_caster.h"

#include "codac2_Action.h"
#include "codac2_CtcAction.h"

using namespace std;
using namespace codac;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Action(py::module& m)
{
  py::class_<OctaSym> octasym(m, "OctaSym", "todo");
  octasym

    .def(py::init([](py::array_t<int> s)
    {
      // Request a buffer descriptor from Python
      py::buffer_info info = s.request();

      // Some sanity checks...
      // TEMPORARILY REMOVED (not supported on Windows): if(info.format != py::format_descriptor<int>::format())
      // TEMPORARILY REMOVED (not supported on Windows):   throw std::runtime_error("Incompatible format: expected a int array");

      //if(info.ndim == 1 ||                            // e.g.: a=np.array([1,0,0])
      //   (info.ndim == 2 && (int)info.shape[1] == 1)) // e.g.: a=np.array([[1],[0],[0]]) 
      {
        //ibex::Vector m(, static_cast<int*>(info.ptr));

        int len = (int)info.shape[0];
        int* w = static_cast<int*>(info.ptr);

        vector<int> s;
        s.assign(w, w + len);
        return OctaSym(s);
      }
    }))
    
    .def(py::self == py::self)

    .def("__call__", [](OctaSym& s,const codac::IntervalVector& x) { return s(x); }, 
      "todo",
      py::return_value_policy::reference_internal)

    .def("__call__", [](OctaSym& s,codac::Ctc& ctc) { return s(ctc); }, 
      "todo",
      py::return_value_policy::reference_internal)

    .def("invert", &OctaSym::invert,
      "todo")

    .def(py::self * py::self)

    .def("__repr__", [](const OctaSym& s) { ostringstream str; str << s; return str.str(); })
  ;

  // Automatic cast from lists to OctaSym
  py::implicitly_convertible<py::list, OctaSym>();
}