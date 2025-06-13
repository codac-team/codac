/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Interval.h>
#include <codac2_TDomain.h>
#include "codac2_py_TDomain_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_TDomain(py::module& m)
{
  py::class_<TDomain, std::shared_ptr<TDomain>> exported_tdomain_class(m, "TDomain", TDOMAIN_MAIN);
  exported_tdomain_class

    .def("t0_tf", &TDomain::t0_tf,
      INTERVAL_TDOMAIN_T0_TF_CONST)

    .def("nb_tslices", &TDomain::nb_tslices,
      SIZET_TDOMAIN_NB_TSLICES_CONST)

    .def("nb_tubes", &TDomain::nb_tubes,
      SIZET_TDOMAIN_NB_TUBES_CONST)

    .def("all_gates_defined", &TDomain::all_gates_defined,
      BOOL_TDOMAIN_ALL_GATES_DEFINED_CONST)

    .def("tslices_vector", &TDomain::tslices_vector,
      VECTOR_TSLICE_TDOMAIN_TSLICES_VECTOR_CONST)

    .def("tslice", [](TDomain& tdomain, double t) -> TSlice& {
        return *tdomain.tslice(t);
      },
      LIST_TSLICE_ITERATOR_TDOMAIN_TSLICE_DOUBLE
      "t"_a)

    .def("sample", [](TDomain& tdomain, double t, bool with_gate) -> TSlice& {
        return *tdomain.sample(t, with_gate);
      },
      LIST_TSLICE_ITERATOR_TDOMAIN_SAMPLE_DOUBLE_BOOL,
      "t"_a, "with_gate"_a=false)

    .def("sample", (void (TDomain::*)(const Interval&,double,bool)) &TDomain::sample,
      VOID_TDOMAIN_SAMPLE_CONST_INTERVAL_REF_DOUBLE_BOOL,
      "t0_tf"_a, "dt"_a, "with_gate"_a=false)

    .def("delete_gates", &TDomain::delete_gates,
      VOID_TDOMAIN_DELETE_GATES)

    .def("__repr__", [](const TDomain& x) {
          std::ostringstream stream;
          stream << x;
          return string(stream.str()); 
        })

    .def_static("are_same", &TDomain::are_same,
      STATIC_BOOL_TDOMAIN_ARE_SAME_CONST_SHARED_PTR_CONST_TDOMAIN_REF_CONST_SHARED_PTR_CONST_TDOMAIN_REF)

  ;

  m

    .def("create_tdomain", (std::shared_ptr<TDomain> (*)(const Interval&)) &create_tdomain,
      SHARED_PTR_TDOMAIN_CREATE_TDOMAIN_CONST_INTERVAL_REF,
      "t0_tf"_a=Interval(-oo,oo))

    .def("create_tdomain", (std::shared_ptr<TDomain> (*)(const Interval&,double,bool)) &create_tdomain,
      SHARED_PTR_TDOMAIN_CREATE_TDOMAIN_CONST_INTERVAL_REF_DOUBLE_BOOL,
      "t0_tf"_a, "dt"_a, "with_gates"_a=false)

  ;
}
