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
#include <codac2_SepChi.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepChi_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepChi(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepChi> exported(m, "SepChi", pysep, SEPCHI_MAIN);
  exported

    .def(py::init(
        [](const SepBase& Sa, const SepBase& Sb, const SepBase& Sc)
        {
          return make_unique<SepChi>(Sa.copy(), Sb.copy(), Sc.copy());
        }),
      SEPCHI_SEPCHI_CONST_S1_REF_CONST_S2_REF_CONST_S3_REF,
      "Sa"_a, "Sb"_a, "Sc"_a)

    .def("separate", &SepChi::separate,
      BOXPAIR_SEPCHI_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}