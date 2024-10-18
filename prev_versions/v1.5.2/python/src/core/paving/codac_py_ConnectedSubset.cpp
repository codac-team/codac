/** 
 *  \file
 *  Paving Python binding
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_Vector.h"
#include "codac_ConnectedSubset.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_ConnectedSubset_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


void export_ConnectedSubset(py::module& m)
{
  py::class_<ConnectedSubset> connectedsubset(m, "ConnectedSubset", CONNECTEDSUBSET_MAIN);
  connectedsubset

    .def("get_boxes", &ConnectedSubset::get_boxes,
      CONNECTEDSUBSET_VECTORINTERVALVECTOR_GET_BOXES)

    .def("is_strictly_included_in_paving", &ConnectedSubset::is_strictly_included_in_paving,
      CONNECTEDSUBSET_BOOL_IS_STRICTLY_INCLUDED_IN_PAVING)

    .def("contains", &ConnectedSubset::contains,
      CONNECTEDSUBSET_BOOL_CONTAINS_VECTOR,
      "p"_a.noconvert())

  ;
}