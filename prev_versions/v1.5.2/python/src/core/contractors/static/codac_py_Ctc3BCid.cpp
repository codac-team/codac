/** 
 *  \file
 *  Ctc3BCid Python binding
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2023 Codac Team
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
#include "ibex_Ctc3BCid.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
//#include "codac_py_Ctc3BCid_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_Ctc3BCid(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
  py::class_<ibex::Ctc3BCid> ctc_3bcid(m, "Ctc3BCid", ctc, "todo");
  ctc_3bcid

    .def(py::init<Ctc&,int,int,int,double>(),
      py::keep_alive<1,2>(),
      "ctc"_a.noconvert(),
      "s3b"_a.noconvert()=ibex::Ctc3BCid::default_s3b,
      "scid"_a.noconvert()=ibex::Ctc3BCid::default_scid,
      "vhandled"_a.noconvert()=-1,
      "var_min_width"_a.noconvert()=ibex::Ctc3BCid::default_var_min_width)
    
    .def("contract", (void (Ctc::*) (IntervalVector&)) &ibex::Ctc3BCid::contract, "todo")
  ;
}