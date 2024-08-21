/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include <codac2_template_tools.h>
#include <codac2_CtcAction.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcAction_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcAction(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcAction> exported(m, "CtcAction", pyctc, CTCACTION_MAIN);
  exported

    .def(py::init(
        [](const Ctc_<IntervalVector>& c, const OctaSym& a)
        {
          return std::make_unique<CtcAction>(c.copy(),a);
        }),
      CTCACTION_CTCACTION_CONST_C_REF_CONST_OCTASYM_REF,
      "c"_a, "a"_a)

    .def("contract", &CtcAction::contract,
      VOID_CTCACTION_CONTRACT_INTERVALVECTOR_REF_CONST,
      "x"_a)
    
  ;
}