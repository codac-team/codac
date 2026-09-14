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
#include <pybind11/functional.h>
#include <codac2_pave.h>
#include "codac2_py_pave_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_pave(py::module& m)
{
  m.def("pave", (PavingOut (*)(const IntervalVector&,const CtcBase<IntervalVector>&,double,bool))&codac2::pave,
    PAVINGOUT_PAVE_CONST_INTERVALVECTOR_REF_CONST_CTCBASE_INTERVALVECTOR_REF_DOUBLE_BOOL,
    "x0"_a, "c"_a, "eps"_a, "verbose"_a=false);

  m.def("pave", (PavingOut (*)(const IntervalVector&,const CtcBase<IntervalVector>&,double,double&,bool))&codac2::pave,
    PAVINGOUT_PAVE_CONST_INTERVALVECTOR_REF_CONST_CTCBASE_INTERVALVECTOR_REF_DOUBLE_DOUBLE_REF_BOOL,
    "x0"_a, "c"_a, "eps"_a, "time"_a, "verbose"_a=false);

  m.def("pave", (PavingInOut (*)(const IntervalVector&,const SepBase&,double,bool))&codac2::pave,
    PAVINGINOUT_PAVE_CONST_INTERVALVECTOR_REF_CONST_SEPBASE_REF_DOUBLE_BOOL,
    "x0"_a, "s"_a, "eps"_a, "verbose"_a=false);

  m.def("regular_pave", &codac2::regular_pave,
    PAVINGINOUT_REGULAR_PAVE_CONST_INTERVALVECTOR_REF_CONST_FUNCTION_BOOLINTERVAL_CONST_INTERVALVECTOR_REF__REF_DOUBLE_BOOL,
    "x0"_a, "test"_a, "eps"_a, "verbose"_a=false);

  m.def("sivia", (PavingInOut (*)(const IntervalVector&,const AnalyticFunction<ScalarType>&,const Interval&,double,bool))&codac2::sivia,
    PAVINGINOUT_SIVIA_CONST_INTERVALVECTOR_REF_CONST_ANALYTICFUNCTION_Y_REF_CONST_TYPENAME_Y_DOMAIN_REF_DOUBLE_BOOL,
    "x0"_a, "f"_a, "y"_a, "eps"_a, "verbose"_a=false);

  m.def("sivia", (PavingInOut (*)(const IntervalVector&,const AnalyticFunction<VectorType>&,const IntervalVector&,double,bool))&codac2::sivia,
    PAVINGINOUT_SIVIA_CONST_INTERVALVECTOR_REF_CONST_ANALYTICFUNCTION_Y_REF_CONST_TYPENAME_Y_DOMAIN_REF_DOUBLE_BOOL,
    "x0"_a, "f"_a, "y"_a, "eps"_a, "verbose"_a=false);

  m.def("sivia", (PavingInOut (*)(const IntervalVector&,const AnalyticFunction<MatrixType>&,const IntervalMatrix&,double,bool))&codac2::sivia,
    PAVINGINOUT_SIVIA_CONST_INTERVALVECTOR_REF_CONST_ANALYTICFUNCTION_Y_REF_CONST_TYPENAME_Y_DOMAIN_REF_DOUBLE_BOOL,
    "x0"_a, "f"_a, "y"_a, "eps"_a, "verbose"_a=false);

  m.def("pave_tube", (PavingInOut (*)(const IntervalVector&,const SlicedTube<IntervalVector>&,double,bool))&codac2::pave_tube,
    PAVINGINOUT_PAVE_TUBE_CONST_INTERVALVECTOR_REF_CONST_SLICEDTUBE_INTERVALVECTOR_REF_DOUBLE_BOOL,
    "x0"_a, "f"_a, "eps"_a, "verbose"_a=false);
}