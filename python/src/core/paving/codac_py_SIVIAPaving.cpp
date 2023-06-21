/** 
 *  \file
 *  Paving Python binding
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

#include "codac_SIVIAPaving.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_SIVIAPaving_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_SIVIAPaving(py::module& m)
{
  py::class_<SIVIAPaving,Paving> paving(m, "SIVIAPaving", SIVIAPAVING_MAIN);
  paving

    .def(py::init<const IntervalVector&>(),
      SIVIAPAVING_SIVIAPAVING_INTERVALVECTOR)

    .def("compute", (void (SIVIAPaving::*)(const Function&,const IntervalVector&,float))&SIVIAPaving::compute,
      SIVIAPAVING_VOID_COMPUTE_FUNCTION_INTERVALVECTOR_FLOAT,
      "f"_a, "y"_a, "precision"_a)

    .def("compute", (void (SIVIAPaving::*)(Ctc&,float))&SIVIAPaving::compute,
      SIVIAPAVING_VOID_COMPUTE_CTC_FLOAT,
      "ctc"_a, "precision"_a)

    .def("compute", (void (SIVIAPaving::*)(ibex::Sep&,float))&SIVIAPaving::compute,
      SIVIAPAVING_VOID_COMPUTE_SEP_FLOAT,
      "sep"_a, "precision"_a)
  ;
}