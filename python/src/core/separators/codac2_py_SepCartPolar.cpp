/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_template_tools.h>
#include <codac2_SepCartPolar.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepCartPolar_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepCartPolar(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepCartPolar> exported(m, "SepCartPolar", pysep, SEPCARTPOLAR_MAIN);
  exported

    .def(py::init([](const SepBase& s1) {
          return std::make_unique<SepCartPolar>(s1.copy());
        }),
      SEPCARTPOLAR_SEPCARTPOLAR_CONST_S_REF,
      "s1"_a)

    .def("separate", &SepCartPolar::separate,
      BOXPAIR_SEPCARTPOLAR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
    
  ;
}