/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_template_tools.h>
#include <codac2_CtcPolytope.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcPolytope_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcPolytope(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcPolytope> exported(m, "CtcPolytope", pyctc, CTCPOLYTOPE_MAIN);
  exported

    .def(py::init<const Polytope&>(),
      CTCPOLYTOPE_CTCPOLYTOPE_CONST_POLYTOPE_REF,
      "p"_a)

    .def(CONTRACT_BOX_METHOD(CtcPolytope,
      VOID_CTCPOLYTOPE_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;

  py::implicitly_convertible<Polytope,CtcPolytope>();
}
