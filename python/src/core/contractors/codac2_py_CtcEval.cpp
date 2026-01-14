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
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcEval(py::module& m)
{
  py::class_<CtcEval> exported(m, "CtcEval", CTCEVAL_MAIN);
  exported

    .def(py::init<>(),
      CTCEVAL_CTCEVAL)

    .def("contract", [](const CtcEval& ctc, Interval& t, py::object& y, py::object& x, const py::object& v)
        {
          if(is_instance<SlicedTube<Interval>>(x)
            && is_instance<SlicedTube<Interval>>(v)
            && is_instance<Interval>(y))
            ctc.contract(t, cast<Interval&>(y), cast<SlicedTube<Interval>>(x), cast<SlicedTube<Interval>>(v));

          else if(is_instance<SlicedTube<IntervalVector>>(x)
            && is_instance<SlicedTube<IntervalVector>>(v)
            && is_instance<IntervalVector>(y))
            ctc.contract(t, cast<IntervalVector&>(y), cast<SlicedTube<IntervalVector>>(x), cast<SlicedTube<IntervalVector>>(v));

          else {
            assert_release("contract: invalid input types");
          }
        },
      VOID_CTCEVAL_CONTRACT_INTERVAL_REF_T_REF_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF_CONST,
      "t"_a, "y"_a, "x"_a, "v"_a)

  ;
}