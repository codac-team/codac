/** 
 *  \file
 *  CtcCartProd Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_py_Ctc.h"
#include "codac_Ctc.h"
#include "codac_CtcCartProd.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcCartProd_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcCartProd(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<CtcCartProd> ctc_cartprod(m, "CtcCartProd", ctc, CTCCARTPROD_MAIN);
  ctc_cartprod

    .def(py::init<ibex::Array<Ctc>&>(),
      CTCCARTPROD_ARRAYCTC_M_V,
      py::keep_alive<1,2>(),
      "array"_a.noconvert())

    .def("contract", &CtcCartProd::contract,
      CTCCARTPROD_VOID_CONTRACT_INTERVALVECTOR,
      "x"_a.noconvert())
  ;

  m.def("cart_prod", [](ibex::Array<Ctc> a)
    { return cart_prod(a); },
    "array"_a.noconvert());
}