/** 
 *  \file
 *  CtcFunction Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "pyIbex_type_caster.h"

#include "ibex_Domain.h"
#include "tubex_CtcFunction.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "tubex_py_CtcFunction_docs.h"

using namespace std;
using namespace ibex;
using namespace tubex;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcFunction(py::module& m)
{
  py::class_<CtcFunction,Ctc> ctc_function(m, "CtcFunction", CTCFUNCTION_MAIN);
  ctc_function

    .def(py::init<const Function&>(),
      CTCFUNCTION_CTCFUNCTION_FUNCTION,
      "f"_a)

    .def(py::init<const Function&,const ibex::Domain&>(),
      CTCFUNCTION_CTCFUNCTION_FUNCTION_DOMAIN,
      "f"_a, "y"_a)

    .def(py::init<const Function&,const Interval&>(),
      CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVAL,
      "f"_a, "y"_a)

    .def(py::init<const Function&,const IntervalVector&>(),
      CTCFUNCTION_CTCFUNCTION_FUNCTION_INTERVALVECTOR,
      "f"_a, "y"_a)

    .def("contract", (void (CtcFunction::*)(IntervalVector&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_INTERVALVECTOR,
      "x"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(TubeVector&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBEVECTOR,
      "x"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(Tube&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBE,
      "x1"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(Tube&,Tube&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE,
      "x1"_a.noconvert(), "x2"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(Tube&,Tube&,Tube&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE,
      "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(Tube&,Tube&,Tube&,Tube&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE,
      "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(Tube&,Tube&,Tube&,Tube&,Tube&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE,
      "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert(), "x5"_a.noconvert())

    .def("contract", (void (CtcFunction::*)(Tube&,Tube&,Tube&,Tube&,Tube&,Tube&))&CtcFunction::contract,
      CTCFUNCTION_VOID_CONTRACT_TUBE_TUBE_TUBE_TUBE_TUBE_TUBE,
      "x1"_a.noconvert(), "x2"_a.noconvert(), "x3"_a.noconvert(), "x4"_a.noconvert(), "x5"_a.noconvert(), "x6"_a.noconvert())

    //.def("contract", (void (CtcFunction::*)(Slice **))&CtcFunction::contract,
    //    CTCFUNCTION_VOID_CONTRACT_SLICE, "v_x_slices"_a)
  ;
}