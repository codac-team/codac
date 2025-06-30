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
#include <codac2_CtcConstell.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcConstell_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcConstell(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcConstell> exported(m, "CtcConstell", pyctc, CTCCONSTELL_MAIN);
  exported

    .def(py::init<const std::vector<IntervalVector>&>(),
      CTCCONSTELL_CTCCONSTELL_CONST_VECTOR_INTERVALVECTOR_REF,
      "M"_a)

    .def(CONTRACT_BOX_METHOD(CtcConstell,
      VOID_CTCCONSTELL_CONTRACT_INTERVALVECTOR_REF_CONST))
  ;
}