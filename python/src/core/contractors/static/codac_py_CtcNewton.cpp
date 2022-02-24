/** 
 *  \file
 *  CtcNewton Python binding
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "ibex_Domain.h"
#include "ibex_Newton.h" // for default values of the Ctc
#include "ibex_CtcNewton.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
// todo: #include "codac_py_CtcNewton_docs.h"

using namespace std;
using namespace ibex;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcNewton(py::module& m)
{
  py::class_<CtcNewton,Ctc> ctc_newton(m, "CtcNewton", "todo");
  ctc_newton

    .def(py::init<const Function&,double,double,double>(),
      "todo",
      "f"_a, "ceil"_a=CtcNewton::default_ceil, "prec"_a=default_newton_prec, "ratio"_a=default_gauss_seidel_ratio)

    .def("contract", (void (CtcNewton::*)(IntervalVector&))&CtcNewton::contract,
      "todo",
      "x"_a.noconvert());
  ;
}