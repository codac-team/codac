/** 
 *  \file
 *  RandTrajectory Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_RandTrajectory.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_RandTrajectory_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_RandTrajectory(py::module& m)
{
  py::class_<RandTrajectory,Trajectory> randtrajectory(m, "RandTrajectory", RANDTRAJECTORY_MAIN);
  randtrajectory

  // Definition

    .def(py::init<const Interval&,double,const Interval&>(),
      RANDTRAJECTORY_RANDTRAJECTORY_INTERVAL_DOUBLE_INTERVAL,
      "tdomain"_a, "timestep"_a, "bounds"_a);
}
