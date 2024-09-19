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
#include <pybind11/functional.h>
#include <codac2_Subpaving.h>
#include <codac2_Paving.h>
#include "codac2_py_Paving_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename P,typename... X>
void export_paving_base(py::class_<P>& c)
{
  c

    .def("size", &Paving<P,X...>::size,
      SIZET_PAVING_PX_SIZE_CONST)

    .def("tree", (std::shared_ptr<PavingNode<P>>(Paving<P,X...>::*)()) &Paving<P,X...>::tree,
      SHARED_PTR_PAVINGNODE_P_PAVING_PX_TREE)

    .def("intersecting_nodes", &Paving<P,X...>::intersecting_nodes,
      LIST_NODE__PAVING_PX_INTERSECTING_NODES_CONST_INTERVALVECTOR_REF_CONST_NODEVALUE__REF_BOOL_CONST
      "x"_a, "node_value"_a, "flat_intersection"_a = false)

    .def("neighbours", &Paving<P,X...>::neighbours,
      LIST_NODE__PAVING_PX_NEIGHBOURS_NODE__CONST_NODEVALUE__REF_CONST_NODEVALUE__REF_CONST
      "x"_a, "node_value"_a, "node_value_neighb"_a)
  ;
}

void export_Paving(py::module& m)
{
  py::class_<PavingOut> exported_paving_out(m, "PavingOut", PAVINGOUT_MAIN);
  export_paving_base<PavingOut,IntervalVector>(exported_paving_out);
  exported_paving_out
  
    .def(py::init<size_t>(),
      PAVINGOUT_PAVINGOUT_SIZET)

    .def(py::init<const IntervalVector&>(),
      PAVINGOUT_PAVINGOUT_CONST_INTERVALVECTOR_REF)

    .def("connected_subsets", (std::list<PavingOut::ConnectedSubset_>(PavingOut::*)(const PavingOut::NodeValue_&) const) &PavingOut::connected_subsets,
      LIST_PAVINGOUT_CONNECTEDSUBSET__PAVINGOUT_CONNECTED_SUBSETS_CONST_PAVINGOUT_NODEVALUE__REF_CONST,
      "node_value"_a = PavingOut::outer_approx)

    .def("connected_subsets", (std::list<PavingOut::ConnectedSubset_>(PavingOut::*)(const IntervalVector&,const PavingOut::NodeValue_&) const) &PavingOut::connected_subsets,
      LIST_PAVINGOUT_CONNECTEDSUBSET__PAVINGOUT_CONNECTED_SUBSETS_CONST_INTERVALVECTOR_REF_CONST_PAVINGOUT_NODEVALUE__REF_CONST,
      "x0"_a, "node_value"_a = PavingOut::outer_approx)

    .def_readonly_static("outer_approx", &PavingOut::outer_approx,
      CONST_NODEVALUE__PAVINGOUT_OUTER_APPROX)
    .def_readonly_static("outer_complem_approx", &PavingOut::outer_complem_approx,
      CONST_NODEVALUE__PAVINGOUT_OUTER_COMPLEM_APPROX)

  ;

  py::class_<PavingInOut> exported_paving_inout(m, "PavingInOut", PAVINGINOUT_MAIN);
  export_paving_base<PavingInOut,IntervalVector,IntervalVector>(exported_paving_inout);
  exported_paving_inout
  
    .def(py::init<size_t>(),
      PAVINGINOUT_PAVINGINOUT_SIZET)

    .def(py::init<const IntervalVector&>(),
      PAVINGINOUT_PAVINGINOUT_CONST_INTERVALVECTOR_REF)

    .def("connected_subsets", (std::list<PavingInOut::ConnectedSubset_>(PavingInOut::*)(const PavingInOut::NodeValue_&) const) &PavingInOut::connected_subsets,
      LIST_PAVINGINOUT_CONNECTEDSUBSET__PAVINGINOUT_CONNECTED_SUBSETS_CONST_PAVINGINOUT_NODEVALUE__REF_CONST,
      "node_value"_a = PavingInOut::outer_approx)

    .def("connected_subsets", (std::list<PavingInOut::ConnectedSubset_>(PavingInOut::*)(const IntervalVector&,const PavingInOut::NodeValue_&) const) &PavingInOut::connected_subsets,
      LIST_PAVINGINOUT_CONNECTEDSUBSET__PAVINGINOUT_CONNECTED_SUBSETS_CONST_INTERVALVECTOR_REF_CONST_PAVINGINOUT_NODEVALUE__REF_CONST,
      "x0"_a, "node_value"_a = PavingInOut::outer_approx)

    .def_readonly_static("outer_approx", &PavingInOut::outer_approx,
      CONST_NODEVALUE__PAVINGINOUT_OUTER_APPROX)
    .def_readonly_static("outer_complem_approx", &PavingInOut::outer_complem_approx,
      CONST_NODEVALUE__PAVINGINOUT_OUTER_COMPLEM_APPROX)
    .def_readonly_static("inner_approx", &PavingInOut::inner_approx,
      CONST_NODEVALUE__PAVINGINOUT_INNER_APPROX)
    .def_readonly_static("bound_approx", &PavingInOut::bound_approx,
      CONST_NODEVALUE__PAVINGINOUT_BOUND_APPROX)
    .def_readonly_static("all_approx", &PavingInOut::all_approx,
      CONST_NODEVALUE__PAVINGINOUT_ALL_APPROX)

  ;
}