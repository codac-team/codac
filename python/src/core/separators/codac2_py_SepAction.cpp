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
#include <codac2_template_tools.h>
#include <codac2_SepAction.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepAction_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepAction(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepAction> exported(m, "SepAction", pysep, SEPACTION_MAIN);
  exported

    .def(py::init(
        [](const SepBase& s, const OctaSym& a)
        {
          return std::make_unique<SepAction>(s.copy(),a);
        }),
      SEPACTION_SEPACTION_CONST_S_REF_CONST_OCTASYM_REF,
      "s"_a, "a"_a)

    .def("separate", &SepAction::separate,
      BOXPAIR_SEPACTION_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}