/** 
 *  \file
 *  Tube binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Ctc.h"
#include "codac_DynCtc.h"
#include "codac_TFnc.h"
#include "codac_VIBesFig.h"
#include "codac_Trajectory.h"

#include "src/core/contractors/dyn/codac_py_DynCtc.h" // virtual items
#include "src/core/functions/codac_py_TFnc.h" // virtual items

#include <pybind11/pybind11.h>

using namespace ibex;
using namespace codac;
namespace py = pybind11;

void export_arithmetic(py::module& m);
void export_ContractorNetwork(py::module& m);
void export_IntervalVar(py::module& m);
void export_IntervalVectorVar(py::module& m);

void export_CtcDist(py::module& m);
void export_CtcFunction(py::module& m);
void export_CtcConstell(py::module& m);
void export_CtcNewton(py::module& m);

py::class_<DynCtc,pyDynCtc> export_DynCtc(py::module& m);
void export_CtcDelay(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcDeriv(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcEval(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcLohner(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcPicard(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcStatic(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);

void export_Tube(py::module&m);
void export_TubeVector(py::module& m);
void export_Trajectory(py::module& m);
void export_RandTrajectory(py::module& m);
void export_TrajectoryVector(py::module& m);
void export_Slice(py::module& m);

py::class_<TFnc,pyTFnc> export_TFnc(py::module& m);
void export_TFunction(py::module& m, py::class_<TFnc>& fnc);

void export_graphics(py::module& m);
void export_ColorMap(py::module& m);
void export_Figure(py::module& m);
void export_VIBesFig(py::module& m);
void export_VIBesFigTube(py::module& m);
void export_VIBesFigTubeVector(py::module& m);
void export_VIBesFigMap(py::module& m);
void export_VIBesFigPaving(py::module& m);

void export_Paving(py::module& m);

void export_DataLoader(py::module& m);
void export_TPlane(py::module& m);

PYBIND11_MODULE(tube, m)
{
  m.doc() = "Python binding of Codac";

  export_arithmetic(m);
  export_ContractorNetwork(m);
  export_IntervalVar(m);
  export_IntervalVectorVar(m);

  export_CtcDist(m);
  export_CtcFunction(m);
  export_CtcConstell(m);
  export_CtcNewton(m);

  py::class_<DynCtc, pyDynCtc> dyn_ctc = export_DynCtc(m);
  export_CtcDelay(m, dyn_ctc);
  export_CtcDeriv(m, dyn_ctc);
  export_CtcEval(m, dyn_ctc);
  export_CtcLohner(m, dyn_ctc);
  export_CtcPicard(m, dyn_ctc);
  export_CtcStatic(m, dyn_ctc);

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
  export_VIBesFigPaving(m);

  export_Paving(m);

  export_DataLoader(m);
  export_TPlane(m);

  // m.attr("ibex_version") = _IBEX_VERSION_;
  // return m.ptr();
}