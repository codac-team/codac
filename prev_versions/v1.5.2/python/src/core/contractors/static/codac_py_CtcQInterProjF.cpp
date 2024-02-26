/** 
 *  \file
 *  CtcPolar Python binding
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
#include "codac_QInterProjF.h"

// Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;

const char* DOC_CTCQINTERPROJF_TYPE=
R"_doc(Exact relaxed intersection of a list of contractors using projection algorithm
Args:
    list (list<Ctc>) : list of contractors
    q (int) : The number of contractors which have to intersect the result
)_doc";


void export_CtcQInterProjF(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{

  py::class_<CtcQInterProjF> ctc_qinter_projf(m, "CtcQInterProjF", ctc, DOC_CTCQINTERPROJF_TYPE);
  ctc_qinter_projf
    .def(py::init<Array<Ctc>, int>(), py::keep_alive<1,2>(), "list"_a.noconvert(), "q"_a.noconvert())
    .def("contract", (void (Ctc::*) (IntervalVector&)) &CtcQInterProjF::contract, QINTERPROJF_CTCQINTERPROJF_ARRAYCTC_INT)
    ;

}