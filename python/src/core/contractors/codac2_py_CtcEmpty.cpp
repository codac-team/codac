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
#include <codac2_CtcEmpty.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcEmpty_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcEmpty(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcEmpty> exported(m, "CtcEmpty", pyctc, CTCEMPTY_MAIN);
  exported

    .def(py::init<size_t>(),
      CTCEMPTY_CTCEMPTY_SIZET
      "n"_a)

    .def(CONTRACT_BOX_METHOD(CtcEmpty,
      VOID_CTCEMPTY_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}