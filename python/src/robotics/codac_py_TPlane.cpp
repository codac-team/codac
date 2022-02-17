/** 
 *  \file
 *  TPlane Python binding
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

#include "codac_Paving.h"
#include "codac_TPlane.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_TPlane_docs.h"

using namespace std;
using namespace ibex;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_TPlane(py::module& m)
{
  py::class_<TPlane, Paving> tplane(m, "TPlane", TPLANE_MAIN);
  tplane

    .def(py::init<const Interval&>(),
      TPLANE_TPLANE_INTERVAL)

    .def("compute_detections", (void (TPlane::*)(float,const TubeVector&))&TPlane::compute_detections,
      TPLANE_VOID_COMPUTE_DETECTIONS_FLOAT_TUBEVECTOR,
      "precision"_a, "p"_a)

    .def("compute_loops", [](TPlane& tplane, float precision, const TubeVector& p, const TubeVector& v)
      {
        TubeVector p_(p), v_(v);
        p_.enable_synthesis(SynthesisMode::BINARY_TREE);
        v_.enable_synthesis(SynthesisMode::BINARY_TREE);
        tplane.compute_detections(precision, p_, v_);
        //p_.enable_synthesis(SynthesisMode::POLYNOMIAL, 5.e-7);
        tplane.compute_proofs(p_, v_);
      },
      TPLANE_VOID_COMPUTE_DETECTIONS_FLOAT_TUBEVECTOR_TUBEVECTOR,
      "precision"_a, "p"_a, "v"_a)

    .def("compute_detections", (void (TPlane::*)(float,const TubeVector&,const TubeVector&))&TPlane::compute_detections,
      TPLANE_VOID_COMPUTE_DETECTIONS_FLOAT_TUBEVECTOR_TUBEVECTOR,
      "precision"_a, "p"_a, "v"_a)

    .def("compute_proofs", (void (TPlane::*)(const TubeVector&))&TPlane::compute_proofs,
      TPLANE_VOID_COMPUTE_PROOFS_TUBEVECTOR,
      "p"_a)

    .def("compute_proofs", (void (TPlane::*)(const TubeVector&,const TubeVector&))&TPlane::compute_proofs,
      TPLANE_VOID_COMPUTE_PROOFS_TUBEVECTOR_TUBEVECTOR,
      "p"_a, "v"_a)

    .def("nb_loops_detections", &TPlane::nb_loops_detections,
      TPLANE_INT_NB_LOOPS_DETECTIONS)

    .def("nb_loops_proofs", &TPlane::nb_loops_proofs,
      TPLANE_INT_NB_LOOPS_PROOFS)

    .def("detected_loops", &TPlane::detected_loops,
      TPLANE_CONSTVECTORINTERVALVECTOR_DETECTED_LOOPS)

    .def("proven_loops", &TPlane::proven_loops,
      TPLANE_CONSTVECTORINTERVALVECTOR_PROVEN_LOOPS)

    .def("traj_loops_summary", &TPlane::traj_loops_summary,
      TPLANE_TRAJECTORY_TRAJ_LOOPS_SUMMARY)

    .def_static("verbose", &TPlane::verbose,
      TPLANE_VOID_VERBOSE_BOOL,
      "verbose"_a = true)
  ;
}