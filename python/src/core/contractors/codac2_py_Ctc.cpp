/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "codac2_py_core.h"
#include "codac2_py_Ctc.h"
#include "codac2_py_Ctc_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector> export_CtcIntervalVector(py::module& m)
{
  py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector> py_ctc(m, "CtcIntervalVector");
  py_ctc

    .def(py::init<size_t>(),
      CTC__X__CTC__SIZET);

  return py_ctc;
}