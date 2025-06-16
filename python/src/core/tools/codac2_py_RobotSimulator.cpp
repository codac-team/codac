/** 
 *  RobotSimulator Python binding
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
#include <pybind11/stl.h>
#include <codac2_RobotSimulator.h>
#include "codac2_py_RobotSimulator_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_RobotSimulator(py::module& m)
{
  py::class_<RobotSimulator> exported_rs(m, "RobotSimulator", ROBOTSIMULATOR_MAIN);
  exported_rs

    .def_readwrite("v_max", &RobotSimulator::v_max,
      DOUBLE_ROBOTSIMULATOR_V_MAX)
    .def_readwrite("w_max", &RobotSimulator::w_max,
      DOUBLE_ROBOTSIMULATOR_W_MAX)
    .def_readwrite("a_max", &RobotSimulator::a_max,
      DOUBLE_ROBOTSIMULATOR_A_MAX)
    .def_readwrite("dist_threshold", &RobotSimulator::dist_threshold,
      DOUBLE_ROBOTSIMULATOR_DIST_THRESHOLD)

    .def(py::init<>(),
      ROBOTSIMULATOR_ROBOTSIMULATOR)

    .def("controller", &RobotSimulator::controller,
      VECTOR_ROBOTSIMULATOR_CONTROLLER_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST,
      "x"_a, "wpt"_a)

    .def("simulate", &RobotSimulator::simulate,
      VECTOR_ROBOTSIMULATOR_CONTROLLER_CONST_VECTOR_REF_CONST_VECTOR_REF_CONST,
      "x0"_a, "dt"_a, "wpts"_a)

  ;
}