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
#include <codac2_CtcPolar.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcPolar_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcPolar(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcPolar> exported(m, "CtcPolar", pyctc, CTCPOLAR_MAIN);
  exported

    .def(py::init<>(),
      CTCPOLAR_CTCPOLAR)

    .def(CONTRACT_BOX_METHOD(CtcPolar,
      VOID_CTCPOLAR_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}