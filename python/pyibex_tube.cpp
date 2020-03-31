/** 
 *  \file
 *  Tube binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Tube.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


PYBIND11_MODULE(tube, m)
{
  py::class_<Tube>(m, "Tube")
    .def(init<const Interval&, double, const Interval&>())
    .def("codomain", &Tube::codomain)
    .def("volume", &Tube::volume)
  ;

  //m.def("cos", [] (const Tube& x){ return cos(x); });
}