/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Auguste Bourgois, Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_template_tools.h>
#include <codac2_SlicedTube.h>
#include <codac2_CtcLohner.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcLohner_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcLohner(py::module& m)
{
  py::class_<CtcLohner> exported(m, "CtcLohner", CTCLOHNER_MAIN);
  exported

    .def(py::init<const AnalyticFunction<VectorType>&,int,double>(),
      CTCLOHNER_CTCLOHNER_CONST_ANALYTICFUNCTION_VECTORTYPE_REF_INT_DOUBLE,
      "f"_a, "contractions"_a = 5, "eps"_a = 0.1)

    .def("contract",
        [](const CtcLohner& ctc, SlicedTube<IntervalVector>& x, const TimePropag& t_propa)
        -> SlicedTube<IntervalVector>&
        {
          ctc.contract(x,t_propa);
          return x;
        },
      VOID_CTCLOHNER_CONTRACT_SLICEDTUBE_INTERVALVECTOR_REF_TIMEPROPAG_CONST,
      "x"_a, "t_propa"_a = TimePropag::FWD_BWD);
  ;
}