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

#include "ibex_Ctc.h"
#include "tubex_DynCtc.h"
#include "tubex_TFnc.h"
#include "tubex_VIBesFig.h"
#include "tubex_Trajectory.h"

#include "src/core/contractors/dyn/tubex_py_DynCtc.h"

#include <pybind11/pybind11.h>

using namespace ibex;
using namespace tubex;
namespace py = pybind11;

void export_arithmetic(py::module& m);
void export_ContractorNetwork(py::module& m);

void export_CtcDist(py::module& m);
void export_CtcFunction(py::module& m);
void export_CtcConstell(py::module& m);

py::class_<DynCtc,pyDynCtc> export_DynCtc(py::module& m);
void export_CtcDeriv(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcEval(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcPicard(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);

void export_Tube(py::module&m);
void export_TubeVector(py::module& m);
void export_Trajectory(py::module& m);
void export_RandTrajectory(py::module& m);
void export_TrajectoryVector(py::module& m);
void export_Slice(py::module& m);

py::class_<TFnc> export_TFnc(py::module& m);
void export_TFunction(py::module& m, py::class_<TFnc>& fnc);

void export_graphics(py::module& m);
void export_ColorMap(py::module& m);
void export_Figure(py::module& m);
void export_VIBesFig(py::module& m);
void export_VIBesFigTube(py::module& m);
void export_VIBesFigTubeVector(py::module& m);
void export_VIBesFigMap(py::module& m);

void export_DataLoader(py::module& m);

PYBIND11_MODULE(tube, m)
{
  m.doc() = "Python binding of tubex-lib";

  export_arithmetic(m);
  export_ContractorNetwork(m);

  export_CtcDist(m);
  export_CtcFunction(m);
  export_CtcConstell(m);

  py::class_<DynCtc, pyDynCtc> dyn_ctc = export_DynCtc(m);
  export_CtcDeriv(m, dyn_ctc);
  export_CtcEval(m, dyn_ctc);
  export_CtcPicard(m, dyn_ctc);

  export_Tube(m);
  export_TubeVector(m);
  export_Trajectory(m);
  export_RandTrajectory(m);
  export_TrajectoryVector(m);
  export_Slice(m);

  py::class_<TFnc> tfnc = export_TFnc(m);
  export_TFunction(m, tfnc);

  export_graphics(m);
  export_ColorMap(m);
  export_Figure(m);
  export_VIBesFig(m);
  export_VIBesFigTube(m);
  export_VIBesFigTubeVector(m);
  export_VIBesFigMap(m);

  export_DataLoader(m);

  // m.attr("ibex_version") = _IBEX_VERSION_;
  // return m.ptr();
}