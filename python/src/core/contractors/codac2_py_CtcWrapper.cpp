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
#include <codac2_CtcWrapper.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcWrapper_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcWrapper(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcWrapper<IntervalVector>> exported(m, "CtcWrapper", pyctc, CTCWRAPPER_MAIN);
  exported

    .def(py::init<const IntervalVector&>(),
      CTCWRAPPER_X_CTCWRAPPER_CONST_X_REF,
      "y"_a)

    .def(CONTRACT_BOX_METHOD(CtcWrapper<IntervalVector>,
      VOID_CTCWRAPPER_X_CONTRACT_X_REF_CONST))
    
  ;
}