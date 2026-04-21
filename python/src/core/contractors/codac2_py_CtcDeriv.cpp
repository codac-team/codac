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
#include <codac2_CtcDeriv.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcDeriv_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
void export_contract(py::class_<CtcDeriv>& exported)
{
  exported

    .def("contract",
        [](const CtcDeriv& ctc, Slice<T>& x, const Slice<T>& v, const std::vector<Index_type>& ctc_indices)
        -> py::tuple
        {
          if constexpr(std::is_same_v<T,IntervalVector>)
            ctc.contract(x, v, matlab::convert_indices(ctc_indices));
          else
            ctc.contract(x, v);

          return py::make_tuple(
            py::cast(x, py::return_value_policy::reference),
            py::cast(v, py::return_value_policy::reference)
          );
        },
      VOID_CTCDERIV_CONTRACT_SLICE_T_REF_CONST_SLICE_T_REF_CONST_VECTOR_INDEX_REF_CONST,
      "x"_a, "v"_a, "ctc_indices"_a = std::vector<Index>())

    .def("contract",
        [](const CtcDeriv& ctc, SlicedTube<T>& x, const SlicedTube<T>& v, const std::vector<Index_type>& ctc_indices)
        -> py::tuple
        {
          if constexpr(std::is_same_v<T,IntervalVector>)
            ctc.contract(x, v, matlab::convert_indices(ctc_indices));
          else
            ctc.contract(x, v);

          return py::make_tuple(
            py::cast(x, py::return_value_policy::reference),
            py::cast(v, py::return_value_policy::reference)
          );
        },
      VOID_CTCDERIV_CONTRACT_SLICEDTUBE_T_REF_CONST_SLICEDTUBE_T_REF_CONST_VECTOR_INDEX_REF_CONST,
      "x"_a, "v"_a, "ctc_indices"_a = std::vector<Index>())

  ;
}

void export_CtcDeriv(py::module& m)
{
  py::class_<CtcDeriv> exported(m, "CtcDeriv", CTCDERIV_MAIN);
  exported

    .def(py::init<const TimePropag&,bool>(),
      CTCDERIV_CTCDERIV_CONST_TIMEPROPAG_REF_BOOL,
      "time_propag"_a=TimePropag::FWD_BWD, "fast_mode"_a=true)

    .def("restrict_tdomain", &CtcDeriv::restrict_tdomain,
      VOID_CTCDERIV_RESTRICT_TDOMAIN_CONST_INTERVAL_REF,
      "tdomain"_a)
  ;

  export_contract<Interval>(exported);
  export_contract<IntervalVector>(exported);
}