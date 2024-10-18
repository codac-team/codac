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
#include <codac2_CtcNot.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcNot_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcNot(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcNot> exported(m, "CtcNot", pyctc, CTCNOT_MAIN);
  exported

    .def(py::init(
        [](const pyCtcIntervalVector& c)
        {
          return std::make_unique<CtcNot>(c.copy());
        }),
      CTCNOT_CTCNOT_CONST_C_REF,
      "c"_a)

    .def(CONTRACT_BOX_METHOD(CtcNot,
      VOID_CTCNOT_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}