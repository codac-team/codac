/** 
 *  \file
 *  Codac binding (codac2)
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac_Ctc.h>
#include <ibex_Sep.h>
#include "src/core/contractors/static/codac_py_Ctc.h"

#include <pybind11/pybind11.h>

using namespace codac;
namespace py = pybind11;

void export_Action(py::module& m);
void export_CtcAction(py::module& m, py::class_<Ctc, pyCtc>& ctc);
void export_ContractorNetwork_codac2(py::module& m);
void export_Contractor_codac2(py::module& m);

PYBIND11_MODULE(codac2, m)
{
  m.doc() = "Python binding of Codac (codac2)";

  py::class_<Ctc, pyCtc> ctc = (py::class_<Ctc, pyCtc>)py::module::import("codac").attr("Ctc");

  export_Action(m);
  export_CtcAction(m, ctc);
  export_ContractorNetwork_codac2(m);
  export_Contractor_codac2(m);
}