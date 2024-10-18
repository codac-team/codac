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
#include <codac2_CtcLazy.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcLazy_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcLazy(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcLazy> exported(m, "CtcLazy", pyctc, CTCLAZY_MAIN);
  exported

    .def(py::init(
        [](const pyCtcIntervalVector& c)
        {
          return std::make_unique<CtcLazy>(c.copy());
        }),
      CTCLAZY_CTCLAZY_CONST_C_REF,
      "c"_a)

    .def(CONTRACT_BOX_METHOD(CtcLazy,
      VOID_CTCLAZY_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}