/** 
 *  \file
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Ctc.h"
#include <ibex_Sep.h>
#include "codac_DynCtc.h"
#include "codac_TFnc.h"
#include "codac_VIBesFig.h"
#include "codac_Trajectory.h"

#include "src/core/contractors/static/codac_py_Ctc.h"
#include "src/core/contractors/dyn/codac_py_DynCtc.h"
#include "src/core/separators/codac_py_Sep.h"
#include "src/core/functions/codac_py_TFnc.h"

#include <pybind11/pybind11.h>

using namespace codac;
namespace py = pybind11;

void export_arithmetic(py::module& m);
void export_ContractorNetwork(py::module& m);
void export_IntervalVar(py::module& m);
void export_IntervalVectorVar(py::module& m);

py::class_<Ctc,pyCtc> export_Ctc(py::module& m);
void export_CtcBox(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcCartProd(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcDist(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcFunction(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcConstell(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcNewton(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcPolar(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_CtcQInterProjF(py::module& m, py::class_<Ctc, pyCtc>& ctc);

py::class_<DynCtc,pyDynCtc> export_DynCtc(py::module& m);
void export_CtcDelay(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcDeriv(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcEval(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcLohner(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcPicard(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);
void export_CtcStatic(py::module& m, py::class_<DynCtc, pyDynCtc>& dyn_ctc);

py::class_<ibex::Sep,pySep> export_Sep(py::module& m);

void export_bisectors(py::module&m);
void export_Interval(py::module&m);
void export_IntervalVector(py::module&m);
void export_IntervalMatrix(py::module&m);
void export_BoolInterval(py::module&m);
void export_Tube(py::module&m);
void export_TubeVector(py::module& m);
void export_Trajectory(py::module& m);
void export_RandTrajectory(py::module& m);
void export_TrajectoryVector(py::module& m);
void export_Slice(py::module& m);

void export_Function(py::module& m);
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

void export_geometry(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<ibex::Sep, pySep>& sep);


void export_Paving(py::module& m);
void export_Set(py::module& m);

void export_DataLoader(py::module& m);
void export_TPlane(py::module& m);

void export_sivia(py::module& m, py::class_<Ctc,pyCtc>& ctc, py::class_<ibex::Sep,pySep>& sep);


PYBIND11_MODULE(core, m)
{
  m.doc() = "Python binding of Codac (core)";

  export_arithmetic(m);
  export_ContractorNetwork(m);
  export_IntervalVar(m);
  export_IntervalVectorVar(m);

  py::class_<Ctc, pyCtc> ctc = export_Ctc(m);
  export_CtcBox(m, ctc);
  export_CtcCartProd(m, ctc);
  export_CtcDist(m, ctc);
  export_CtcFunction(m, ctc);
  export_CtcConstell(m, ctc);
  export_CtcNewton(m, ctc);
  export_CtcPolar(m, ctc);

  py::class_<DynCtc, pyDynCtc> dyn_ctc = export_DynCtc(m);
  export_CtcDelay(m, dyn_ctc);
  export_CtcDeriv(m, dyn_ctc);
  export_CtcEval(m, dyn_ctc);
  export_CtcLohner(m, dyn_ctc);
  export_CtcPicard(m, dyn_ctc);
  export_CtcStatic(m, dyn_ctc);

  py::class_<ibex::Sep, pySep> sep = export_Sep(m);

  export_bisectors(m);
  export_BoolInterval(m);
  export_Interval(m);
  export_IntervalVector(m);
  export_IntervalMatrix(m);
  export_Tube(m);
  export_TubeVector(m);
  export_Trajectory(m);
  export_RandTrajectory(m);
  export_TrajectoryVector(m);
  export_Slice(m);

  export_Function(m);
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

  export_geometry(m, ctc, sep);

  export_Paving(m);
  export_Set(m);

  export_DataLoader(m);
  export_TPlane(m);

  export_sivia(m, ctc, sep);

  // m.attr("ibex_version") = _IBEX_VERSION_;
  // return m.ptr();
}