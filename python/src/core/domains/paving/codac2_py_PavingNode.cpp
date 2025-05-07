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
#include <codac2_Paving.h>
#include <codac2_PavingNode.h>
#include "codac2_py_PavingNode_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename PN,typename P>
void export_pavingnode_base(py::class_<PN, std::shared_ptr<PN>>& c)
{
  c

    .def("paving", &PN::paving,
      CONST_P_REF_PAVINGNODE_P_PAVING_CONST)

    .def("boxes", (typename P::NodeTuple_&(PN::*)()) &PN::boxes,
      P_NODETUPLE__REF_PAVINGNODE_P_BOXES)

    .def("set_boxes", &PN::set_boxes,
      VOID_PAVINGNODE_P_SET_BOXES_CONST_TYPENAME_P_NODETUPLE__REF,
      "x"_a)

    .def("hull", &PN::hull,
      INTERVALVECTOR_PAVINGNODE_P_HULL_CONST)

    .def("unknown", &PN::unknown,
      INTERVALVECTOR_PAVINGNODE_P_UNKNOWN_CONST)

    .def("top", (std::shared_ptr<PN>(PN::*)()) &PN::top,
      SHARED_PTR_PAVINGNODE_P_PAVINGNODE_P_TOP)

    .def("left", (std::shared_ptr<PN>(PN::*)()) &PN::left,
      SHARED_PTR_PAVINGNODE_P_PAVINGNODE_P_LEFT)

    .def("right", (std::shared_ptr<PN>(PN::*)()) &PN::right,
      SHARED_PTR_PAVINGNODE_P_PAVINGNODE_P_RIGHT)

    .def("is_leaf", &PN::is_leaf,
      BOOL_PAVINGNODE_P_IS_LEAF_CONST)

    .def("bisect", (void(PN::*)(void)) &PN::bisect,
      VOID_PAVINGNODE_P_BISECT)

    .def("bisect", (void(PN::*)(std::function<std::pair<IntervalVector,IntervalVector>(const IntervalVector&)>)) &PN::bisect,
      VOID_PAVINGNODE_P_BISECT_FUNCTION_PAIR_INTERVALVECTORINTERVALVECTOR_CONST_INTERVALVECTOR_REF_)

    .def("visit", (void(PN::*)(std::function<bool(std::shared_ptr<PN>)>)) &PN::visit,
      VOID_PAVINGNODE_P_VISIT_FUNCTION_BOOL_SHARED_PTR_PAVINGNODE_P_,
      "visitor"_a)

  ;
}

void export_PavingNode(py::module& m)
{
  py::class_<PavingOut_Node, std::shared_ptr<PavingOut_Node> /* due to enable_shared_from_this */>
    exported_pavingnode_out(m, "PavingOut_Node", "");
  export_pavingnode_base<PavingOut_Node,PavingOut>(exported_pavingnode_out);

  py::class_<PavingInOut_Node, std::shared_ptr<PavingInOut_Node> /* due to enable_shared_from_this */>
    exported_pavingnode_inout(m, "PavingInOut_Node", "");
  export_pavingnode_base<PavingInOut_Node,PavingInOut>(exported_pavingnode_inout);
}