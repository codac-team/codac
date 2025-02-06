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
      INDEX_PAVING_PX_SIZE_CONST)

    .def("tree", (std::shared_ptr<PavingNode<P>>(Paving<P,X...>::*)()) &Paving<P,X...>::tree,
      SHARED_PTR_PAVINGNODE_P_PAVING_PX_TREE)

    .def("intersecting_boxes", &Paving<P,X...>::intersecting_boxes,
      LIST_INTERVALVECTOR_PAVING_PX_INTERSECTING_BOXES_CONST_INTERVALVECTOR_REF_CONST_NODEVALUE__REF_CONST
      "x"_a, "node_value"_a)
    
  ;
}

void export_Paving(py::module& m)
{
  py::class_<PavingOut> exported_paving_out(m, "PavingOut", PAVINGOUT_MAIN);
  export_paving_base<PavingOut,IntervalVector>(exported_paving_out);
  exported_paving_out
  
    .def(py::init<Index>(),
      PAVINGOUT_PAVINGOUT_INDEX)

    .def(py::init<const IntervalVector&>(),
      PAVINGOUT_PAVINGOUT_CONST_INTERVALVECTOR_REF)

    .def("connected_subsets", (std::list<PavingOut::ConnectedSubset_>(PavingOut::*)(const PavingOut::NodeValue_&) const) &PavingOut::connected_subsets,
      LIST_PAVINGOUT_CONNECTEDSUBSET__PAVINGOUT_CONNECTED_SUBSETS_CONST_PAVINGOUT_NODEVALUE__REF_CONST,
      "node_value"_a = PavingOut::outer)

    .def("connected_subsets", (std::list<PavingOut::ConnectedSubset_>(PavingOut::*)(const IntervalVector&,const PavingOut::NodeValue_&) const) &PavingOut::connected_subsets,
      LIST_PAVINGOUT_CONNECTEDSUBSET__PAVINGOUT_CONNECTED_SUBSETS_CONST_INTERVALVECTOR_REF_CONST_PAVINGOUT_NODEVALUE__REF_CONST,
      "x0"_a, "node_value"_a = PavingOut::outer)

    .def_readonly_static("outer", &PavingOut::outer,
      STATIC_CONST_NODEVALUE__PAVINGOUT_OUTER)
    .def_readonly_static("outer_complem", &PavingOut::outer_complem,
      STATIC_CONST_NODEVALUE__PAVINGOUT_OUTER_COMPLEM)

  ;

  py::class_<PavingInOut> exported_paving_inout(m, "PavingInOut", PAVINGINOUT_MAIN);
  export_paving_base<PavingInOut,IntervalVector,IntervalVector>(exported_paving_inout);
  exported_paving_inout
  
    .def(py::init<Index>(),
      PAVINGINOUT_PAVINGINOUT_INDEX)

    .def(py::init<const IntervalVector&>(),
      PAVINGINOUT_PAVINGINOUT_CONST_INTERVALVECTOR_REF)

    .def("connected_subsets", (std::list<PavingInOut::ConnectedSubset_>(PavingInOut::*)(const PavingInOut::NodeValue_&) const) &PavingInOut::connected_subsets,
      LIST_PAVINGINOUT_CONNECTEDSUBSET__PAVINGINOUT_CONNECTED_SUBSETS_CONST_PAVINGINOUT_NODEVALUE__REF_CONST,
      "node_value"_a = PavingInOut::outer)

    .def("connected_subsets", (std::list<PavingInOut::ConnectedSubset_>(PavingInOut::*)(const IntervalVector&,const PavingInOut::NodeValue_&) const) &PavingInOut::connected_subsets,
      LIST_PAVINGINOUT_CONNECTEDSUBSET__PAVINGINOUT_CONNECTED_SUBSETS_CONST_INTERVALVECTOR_REF_CONST_PAVINGINOUT_NODEVALUE__REF_CONST,
      "x0"_a, "node_value"_a = PavingInOut::outer)

    .def_readonly_static("outer", &PavingInOut::outer,
      STATIC_CONST_NODEVALUE__PAVINGINOUT_OUTER)
    .def_readonly_static("outer_complem", &PavingInOut::outer_complem,
      STATIC_CONST_NODEVALUE__PAVINGINOUT_OUTER_COMPLEM)
    .def_readonly_static("inner", &PavingInOut::inner,
      STATIC_CONST_NODEVALUE__PAVINGINOUT_INNER)
    .def_readonly_static("bound", &PavingInOut::bound,
      STATIC_CONST_NODEVALUE__PAVINGINOUT_BOUND)
    .def_readonly_static("all", &PavingInOut::all,
      STATIC_CONST_NODEVALUE__PAVINGINOUT_ALL)

  ;
}