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
#include <codac2_pave.h>
#include "codac2_py_pave_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_pave(py::module& m)
{
  m.def("pave", (PavingOut (*)(const IntervalVector&,const CtcBase<IntervalVector>&,double))&codac2::pave,
    PAVINGOUT_PAVE_CONST_INTERVALVECTOR_REF_CONST_CTCBASE_INTERVALVECTOR_REF_DOUBLE,
    "x"_a, "c"_a, "eps"_a);

  m.def("pave", (PavingInOut (*)(const IntervalVector&,const SepBase&,double))&codac2::pave,
    PAVINGINOUT_PAVE_CONST_INTERVALVECTOR_REF_CONST_SEPBASE_REF_DOUBLE,
    "x"_a, "s"_a, "eps"_a);

  m.def("sivia_", (PavingInOut (*)(const IntervalVector&,const AnalyticFunction<ScalarOpValue>&,const Interval&,double))&codac2::sivia,
    PAVINGINOUT_SIVIA_CONST_INTERVALVECTOR_REF_CONST_ANALYTICFUNCTION_Y_REF_CONST_TYPENAME_Y_DOMAIN_REF_DOUBLE,
    "x"_a, "f"_a, "y"_a, "eps"_a);

  m.def("sivia_", (PavingInOut (*)(const IntervalVector&,const AnalyticFunction<VectorOpValue>&,const IntervalVector&,double))&codac2::sivia,
    PAVINGINOUT_SIVIA_CONST_INTERVALVECTOR_REF_CONST_ANALYTICFUNCTION_Y_REF_CONST_TYPENAME_Y_DOMAIN_REF_DOUBLE,
    "x"_a, "f"_a, "y"_a, "eps"_a);
}