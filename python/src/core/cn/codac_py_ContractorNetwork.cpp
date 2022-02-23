/** 
 *  \file
 *  ContractorNetwork Python binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <unordered_map>
#include "codac_type_caster.h"

#include "codac_ContractorNetwork.h"
#include "codac_Variable.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_ContractorNetwork_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


codac::Domain pyobject_to_domain(py::object object)
{
  if(py::isinstance<py::int_>(object)) // todo: get a reference instead of const (possible?)
  {
    const double itv = object.cast<int>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<py::float_>(object)) // todo: get a reference instead of const (possible?)
  {
    const double itv = object.cast<double>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<Interval>(object))
  {
    Interval &itv = object.cast<Interval&>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<IntervalVar>(object))
  {
    IntervalVar &itv = object.cast<IntervalVar&>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<py::list>(object))
  {
    // todo: remove dynamic allocation here
    IntervalVector *v = new IntervalVector(object.cast<const Vector>());
    return codac::Domain(*v);
  }
  else if(py::isinstance<Vector>(object)) // todo: this seems to be never used
  {
    Vector itv = object.cast<Vector>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<IntervalVector>(object))
  {
    IntervalVector &itv = object.cast<IntervalVector&>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<IntervalVectorVar>(object))
  {
    IntervalVectorVar &itv = object.cast<IntervalVectorVar&>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<Tube>(object))
  {
    Tube &itv = object.cast<Tube&>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<TubeVector>(object))
  {
    TubeVector &itv = object.cast<TubeVector&>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<float>(object))
  {
    const double itv = object.cast<double>();
    return codac::Domain(itv);
  }
  else if(py::isinstance<double>(object))
  {
    const double itv = object.cast<double>();
    return codac::Domain(itv);
  }
  else
  {
    try {
      const double itv = object.cast<double>();
      return codac::Domain(itv);
    }
    catch(py::cast_error const&) {
      throw invalid_argument("unable to convert the py::object into a valid codac::Domain");
    }
  }
}

vector<codac::Domain> pylist_to_vectordomains(py::list lst)
{
  vector<codac::Domain> domains;

  if(lst.size() < 1)
    throw invalid_argument("Size of the input list is 0");

  for(size_t i = 0 ; i < lst.size() ; i++)
    domains.push_back(pyobject_to_domain(lst[i]));

  return domains;
}

unordered_map<codac::Domain,codac::Domain> pydict_to_unorderedmapdomains(py::dict dct)
{
  unordered_map<codac::Domain,codac::Domain> domains;

  if(dct.size() < 1)
    throw invalid_argument("Size of the input dict is 0");

  for(auto e : dct)
    domains[pyobject_to_domain(e.first.cast<py::object>())] = pyobject_to_domain(e.second.cast<py::object>());

  return domains;
}

void export_ContractorNetwork(py::module& m)
{
  py::class_<ContractorNetwork> cn(m, "ContractorNetwork", CONTRACTORNETWORK_MAIN);
  cn

  // Definition

    .def(py::init<>(),
      CONTRACTORNETWORK_CONTRACTORNETWORK)

    .def("nb_ctc", &ContractorNetwork::nb_ctc,
      CONTRACTORNETWORK_INT_NB_CTC)

    .def("nb_dom", &ContractorNetwork::nb_dom,
      CONTRACTORNETWORK_INT_NB_DOM)

    .def("emptiness", &ContractorNetwork::emptiness,
      CONTRACTORNETWORK_BOOL_EMPTINESS)

  // Building the network (domains, contractors)

    .def("create_dom", (Interval& (ContractorNetwork::*)(const Interval&))&ContractorNetwork::create_dom,
      CONTRACTORNETWORK_INTERVAL_CREATE_DOM_INTERVAL,
      "i"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>()) // deprecated!

    .def("create_dom", (IntervalVector& (ContractorNetwork::*)(const IntervalVector&))&ContractorNetwork::create_dom,
      CONTRACTORNETWORK_INTERVALVECTOR_CREATE_DOM_INTERVALVECTOR,
      "iv"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>()) // deprecated!

    .def("create_dom", (Tube& (ContractorNetwork::*)(const Tube&))&ContractorNetwork::create_dom,
      CONTRACTORNETWORK_TUBE_CREATE_DOM_TUBE,
      "t"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>()) // deprecated!

    .def("create_dom", (TubeVector& (ContractorNetwork::*)(const TubeVector&))&ContractorNetwork::create_dom,
      CONTRACTORNETWORK_TUBEVECTOR_CREATE_DOM_TUBEVECTOR,
      "tv"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>()) // deprecated!

    .def("create_interm_var", (Interval& (ContractorNetwork::*)(const Interval&))&ContractorNetwork::create_interm_var,
      CONTRACTORNETWORK_INTERVAL_CREATE_INTERM_VAR_INTERVAL,
      "i"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    .def("create_interm_var", (IntervalVector& (ContractorNetwork::*)(const IntervalVector&))&ContractorNetwork::create_interm_var,
      CONTRACTORNETWORK_INTERVALVECTOR_CREATE_INTERM_VAR_INTERVALVECTOR,
      "iv"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    .def("create_interm_var", (Tube& (ContractorNetwork::*)(const Tube&))&ContractorNetwork::create_interm_var,
      CONTRACTORNETWORK_TUBE_CREATE_INTERM_VAR_TUBE,
      "t"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    .def("create_interm_var", (TubeVector& (ContractorNetwork::*)(const TubeVector&))&ContractorNetwork::create_interm_var,
      CONTRACTORNETWORK_TUBEVECTOR_CREATE_INTERM_VAR_TUBEVECTOR,
      "tv"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    .def("subvector", (IntervalVector& (ContractorNetwork::*)(Vector&,int,int))&ContractorNetwork::subvector,
      CONTRACTORNETWORK_INTERVALVECTOR_SUBVECTOR_VECTOR_INT_INT,
      "v"_a, "start_index"_a, "end_index"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    .def("subvector", (IntervalVector& (ContractorNetwork::*)(IntervalVector&,int,int))&ContractorNetwork::subvector,
      CONTRACTORNETWORK_INTERVALVECTOR_SUBVECTOR_INTERVALVECTOR_INT_INT,
      "iv"_a, "start_index"_a, "end_index"_a,
      py::return_value_policy::reference_internal,
      py::keep_alive<1,0>())

    .def("add", [](ContractorNetwork& cn, Ctc& ctc, py::list lst)
      {
        cn.add(ctc, pylist_to_vectordomains(lst));
      },
      CONTRACTORNETWORK_VOID_ADD_CTC_VECTORDOMAIN,
      "stastic_ctc"_a, "v_domains"_a,
      py::keep_alive<1,3>(), py::keep_alive<1,2>())

    .def("add", [](ContractorNetwork& cn, DynCtc& ctc, py::list lst)
      {
        cn.add(ctc, pylist_to_vectordomains(lst));
      },
      CONTRACTORNETWORK_VOID_ADD_DYNCTC_VECTORDOMAIN,
      "dyn_ctc"_a, "v_domains"_a,
      py::keep_alive<1,3>(), py::keep_alive<1,2>())

    .def("add_data", (void (ContractorNetwork::*)(Tube &,double,const Interval&))&ContractorNetwork::add_data,
      CONTRACTORNETWORK_VOID_ADD_DATA_TUBE_DOUBLE_INTERVAL,
      "x"_a, "t"_a, "y"_a)
    
    .def("add_data", (void (ContractorNetwork::*)(TubeVector &,double,const IntervalVector&))&ContractorNetwork::add_data,
      CONTRACTORNETWORK_VOID_ADD_DATA_TUBEVECTOR_DOUBLE_INTERVALVECTOR,
      "x"_a, "t"_a, "y"_a)

  // Contraction process

    .def("contract", (double (ContractorNetwork::*)(bool))&ContractorNetwork::contract,
      CONTRACTORNETWORK_DOUBLE_CONTRACT_BOOL,
      "verbose"_a=false)

    .def("contract", [](ContractorNetwork& cn, py::dict var_dom, bool verbose)
      {
        cn.contract(pydict_to_unorderedmapdomains(var_dom), verbose);
      },
      CONTRACTORNETWORK_DOUBLE_CONTRACT_UNORDEREDMAPDOMAINDOMAIN_BOOL,
      "var_dom"_a, "verbose"_a=false)

    .def("contract_during", &ContractorNetwork::contract_during,
      CONTRACTORNETWORK_DOUBLE_CONTRACT_DURING_DOUBLE_BOOL,
      "dt"_a, "verbose"_a=false)

    .def("set_fixedpoint_ratio", &ContractorNetwork::set_fixedpoint_ratio,
      CONTRACTORNETWORK_VOID_SET_FIXEDPOINT_RATIO_FLOAT,
      "r"_a)

    .def("trigger_all_contractors", &ContractorNetwork::trigger_all_contractors,
      CONTRACTORNETWORK_VOID_TRIGGER_ALL_CONTRACTORS)

    .def("reset_interm_vars", &ContractorNetwork::reset_interm_vars,
      CONTRACTORNETWORK_VOID_RESET_INTERM_VARS)

    .def("nb_ctc_in_stack", &ContractorNetwork::nb_ctc_in_stack,
      CONTRACTORNETWORK_INT_NB_CTC_IN_STACK)

  // Visualization

    .def("set_name", (void (ContractorNetwork::*)(Ctc &,const string&))&ContractorNetwork::set_name,
      CONTRACTORNETWORK_VOID_SET_NAME_CTC_STRING,
      "ctc"_a, "name"_a)

    .def("set_name", (void (ContractorNetwork::*)(DynCtc &,const string&))&ContractorNetwork::set_name,
      CONTRACTORNETWORK_VOID_SET_NAME_DYNCTC_STRING,
      "ctc"_a, "name"_a)

    // Place this function after the other set_name()
    .def("set_name", [](ContractorNetwork& cn, py::object obj, const string& name)
      {
        codac::Domain dom = pyobject_to_domain(obj);
        cn.set_name(dom, name);
      },
      CONTRACTORNETWORK_VOID_SET_NAME_DOMAIN_STRING,
      "dom"_a, "name"_a,
      py::keep_alive<1,2>())

    .def("print_dot_graph", &ContractorNetwork::print_dot_graph,
      CONTRACTORNETWORK_INT_PRINT_DOT_GRAPH_STRING_STRING,
      "cn_name"_a="cn", "layer_model"_a="fdp")

    .def("__repr__", [](const ContractorNetwork& x) { ostringstream str; str << x; return str.str(); })
  
  ;
}