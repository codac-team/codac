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
#include <codac2_template_tools.h>
#include <codac2_SlicedTube.h>
#include <codac2_CtcEval.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcEval_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_contract(py::class_<CtcEval>& exported)
{
  exported.def("contract",
    [](const CtcEval& ctc, Interval& t, T& y, SlicedTube<T>& x, const SlicedTube<T>& v)
    -> py::tuple
    {
      ctc.contract(t,y,x,v);

      return py::make_tuple(
        py::cast(t, py::return_value_policy::reference),
        py::cast(y, py::return_value_policy::reference),
        py::cast(x, py::return_value_policy::reference),
        py::cast(v, py::return_value_policy::reference)
      );
    },
  VOID_CTCEVAL_CONTRACT_INTERVAL_REF_T_REF_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF_CONST,
  "t"_a, "y"_a, "x"_a, "v"_a);
}

void export_CtcEval(py::module& m)
{
  py::class_<CtcEval> exported(m, "CtcEval", CTCEVAL_MAIN);
  exported.def(py::init<>(), CTCEVAL_CTCEVAL);
  export_contract<Interval>(exported);
  export_contract<IntervalVector>(exported);
}