/** 
 *  \file
 *  Codac binding (unsupported)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac_Ctc.h>
#include <ibex_Sep.h>
#include "src/core/contractors/static/codac_py_Ctc.h"
#include "src/core/separators/codac_py_Sep.h"

#include <pybind11/pybind11.h>

using namespace codac;
namespace py = pybind11;

void export_GeoImage(py::module& m);
void export_CtcRaster(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_GeoMapper(py::module& m);
void export_unsupported_sep(py::module& m, py::class_<ibex::Sep, pySep>& sep);
void export_unsupported_ctc(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_unsupported_geometry(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<ibex::Sep, pySep>& sep);
void export_unsupported_paving(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<ibex::Sep, pySep>& sep);
void export_unsupported_thickset(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<ibex::Sep, pySep>& sep);

PYBIND11_MODULE(unsupported, m)
{
  m.doc() = "Python binding of Codac (unsupported)";

  py::class_<Ctc, pyCtc> ctc = (py::class_<Ctc, pyCtc>)py::module::import("codac").attr("Ctc");
  py::class_<ibex::Sep, pySep> sep = (py::class_<ibex::Sep, pySep>)py::module::import("codac").attr("Sep");

  export_GeoImage(m);
  export_CtcRaster(m, ctc);
  export_GeoMapper(m);

  export_unsupported_sep(m, sep);
  export_unsupported_ctc(m, ctc);
  export_unsupported_geometry(m, ctc, sep);
  export_unsupported_paving(m, ctc, sep);
  export_unsupported_thickset(m, ctc, sep);

  // m.attr("ibex_version") = _IBEX_VERSION_;
  // return m.ptr();
}