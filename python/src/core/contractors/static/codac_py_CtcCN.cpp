/**
 *  \file
 *  CtcCN Python binding
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Julien Damers
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
#include "codac_CtcCN.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcCN_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_CtcCN(py::module& m, py::class_<Ctc, pyCtc>& ctc)
{
    py::class_<CtcCN> ctc_cn(m, "CtcCN", ctc, CTCCN_MAIN);
    ctc_cn

            .def(py::init<ContractorNetwork &, IntervalVectorVar &>(),
                 CTCCN_CTCCN_CONTRACTORNETWORK_INTERVALVECTORVAR,
                 "cn"_a.noconvert(),"box"_a.noconvert())


            .def("contract", &CtcCN::contract,
                 CTCCN_VOID_CONTRACT_INTERVALVECTOR,
                 "box"_a.noconvert())
            ;
}

