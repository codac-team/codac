/** 
 *  Codac binding (diffinclusion)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_DiffInclusion.h>
#include <codac2_ReachabilityStep.h>
#include <codac2_RS_box.h>
#include <codac2_RS_zonotope.h>
#include <codac2_DI_Integration.h>
#include <codac2_SlicedTube.h>
#include "codac2_py_DI_Integration_docs.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

/* we limit ourselves to VectorVar */

template <typename DI, typename IS>
void export_DI_Integration(py::module& m, const std::string& name)
{

  using X = DI::X;
  using U = DI::U;
  using DomA = IS::DomainA;
  using DomB = IS::DomainB;


  py::class_<DI_Integration<DI,IS>> 
			exported_di_integration(m, name.c_str(),
			 DI_INTEGRATION_MAIN);

  exported_di_integration
    .def(py::init<const DI&, int, int>(),
      DI_INTEGRATION_DIIS_DI_INTEGRATION_CONST_DI_REF_INT_INT,
      "di"_a, "widen_attempts"_a=3, "narrow_attempts"_a=3)

    .def("contract_one_step",
	[](DI_Integration<DI,IS>& s, const Interval &time_step,
                const DomA& init, DomB& in_domain, DomA& out_gate,
                const U& u, bool forward) {
	   return s.contract_one_step(time_step,init,in_domain,out_gate,u,
		forward);
        },
	BOOL_DI_INTEGRATION_DIIS_CONTRACT_ONE_STEP_CONST_INTERVAL_REF_CONST_DOMA_REF_DOMB_REF_DOMA_REF_CONST_U_REF_BOOL,
	"time_step"_a, "init"_a, "in_domain"_a, "out_gate"_a, "u"_a,
	"forward"_a=true)

    .def("integrate_to_time_indom",
	[](DI_Integration<DI,IS>& s, const Interval &time_step,
		  const DomA& init, const DomB& in_domain,
                  const U& u, double t_steps) {
	   return s.integrate_to_time_indom(time_step,init,in_domain,u,t_steps);
        },
	PAIR_BOOLDOMA_DI_INTEGRATION_DIIS_INTEGRATE_TO_TIME_INDOM_CONST_INTERVAL_REF_CONST_DOMA_REF_CONST_DOMB_REF_CONST_U_REF_DOUBLE,
	"time_step"_a, "init"_a, "in_domain"_a, "u"_a, "t_steps"_a)

    .def("integrate_to_time",
	[](DI_Integration<DI,IS>& s, const Interval &time_step,
                  const X& init, const X& in_domain,
                  const U& u, double t_steps) {
	   return s.integrate_to_time(time_step,init,in_domain,u,t_steps);
	},
        PAIR_BOOLX_DI_INTEGRATION_DIIS_INTEGRATE_TO_TIME_CONST_INTERVAL_REF_CONST_X_REF_CONST_X_REF_CONST_U_REF_DOUBLE,
	"time_step"_a, "init"_a, "in_domain"_a, "u"_a, "t_steps"_a)

    .def("create_sliced_tube",
	[](DI_Integration<DI,IS>& s, const std::shared_ptr<TDomain>& tdomain,
                        const X&init, const X& in_domain,
                        const U& u, bool forward) {
	   return s.create_sliced_tube(tdomain,init,in_domain,u,forward);
	},
	SLICEDTUBE_X_DI_INTEGRATION_DIIS_CREATE_SLICED_TUBE_CONST_SHARED_PTR_TDOMAIN_REF_CONST_X_REF_CONST_X_REF_CONST_U_REF_BOOL,
	"tdomain"_a, "init"_a, "in_domain"_a, "u"_a, "forward"_a)

    .def("contract_sliced_tube",
	[](DI_Integration<DI,IS>& s, SlicedTube<X>& tube, const U& u, TimePropag dir) {
	   s.contract_sliced_tube(tube,u,dir);
        },
	"tube"_a, "u"_a, "dir"_a)
  ;
}
