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

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "tubex_RandTrajectory.h"

// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_RandTrajectory_docs.h"

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


void export_RandTrajectory(py::module& m, py::class_<Trajectory>& trajectory){

    py::class_<RandTrajectory> randtrajectory(m, "RandTrajectory", trajectory, "todo");
    randtrajectory
      .def(py::init<const ibex::Interval &,double,const ibex::Interval &>(),
          RANDTRAJECTORY_RANDTRAJECTORY_INTERVAL_DOUBLE_INTERVAL, "tdomain"_a, "timestep"_a, "bounds"_a);

    //py::implicitly_convertible<Trajectory,RandTrajectory>();
}
