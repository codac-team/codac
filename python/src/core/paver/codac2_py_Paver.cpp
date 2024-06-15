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
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_Paver.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_Paver_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Paver(py::module& m, py::class_<Ctc_<IntervalVector>,pyCtcIntervalVector>& ctc)
{
  m.def("pave", (void (*)(const IntervalVector&,const Ctc_<IntervalVector>&,double))&pave,
    VOID_PAVE_CONST_INTERVALVECTOR_REF_CONST_O_REF_DOUBLE,
    "x0"_a, "c"_a, "eps"_a);

  m.def("pave", (void (*)(const IntervalVector&,const Sep&,double))&pave,
    VOID_PAVE_CONST_INTERVALVECTOR_REF_CONST_O_REF_DOUBLE,
    "x0"_a, "s"_a, "eps"_a);
}