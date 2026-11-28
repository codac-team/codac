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
#include "codac2_py_DiffInclusion_docs.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

/* we limit ourselves to VectorVar */

void export_DiffInclusion(py::module& m, const std::string& name)
{
  py::class_<DiffInclusion<VectorVar,VectorVar>> 
			exported_diffinclusion(m, name.c_str(),
			 DIFFINCLUSION_MAIN);

  exported_diffinclusion

    .def(py::init<const AnalyticFunction<VectorType>&, bool>(),
      DIFFINCLUSION_XVARUVAR_DIFFINCLUSION_CONST_ANALYTICFUNCTION_XTYPE_REF_BOOL,
      "f"_a, "time_dependant"_a = false)
  ;
}
