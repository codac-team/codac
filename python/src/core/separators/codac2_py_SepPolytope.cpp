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
#include <codac2_SepPolytope.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepPolytope_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepPolytope(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepPolytope> exported(m, "SepPolytope", pysep, SEPPOLYTOPE_MAIN);
  exported

    .def(py::init<const Polytope&>(),
      SEPPOLYTOPE_SEPPOLYTOPE_CONST_POLYTOPE_REF,
      "p"_a)

    .def("separate", &SepPolytope::separate,
      BOXPAIR_SEPPOLYTOPE_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
  
  py::implicitly_convertible<Polytope,SepPolytope>();
}
