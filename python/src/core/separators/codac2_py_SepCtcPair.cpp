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
#include <codac2_SepCtcPair.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepCtcPair_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepCtcPair(py::module& m, py::class_<SepBase,pySep>& pysep)
{
  py::class_<SepCtcPair> exported(m, "SepCtcPair", pysep, SEPCTCPAIR_MAIN);
  exported

    .def(py::init(
        [](const CtcBase<IntervalVector>& ctc_in, const CtcBase<IntervalVector>& ctc_out)
        {
          return std::make_unique<SepCtcPair>(ctc_in.copy(),ctc_out.copy());
        }),
      SEPCTCPAIR_SEPCTCPAIR_CONST_C1_REF_CONST_C2_REF,
      "ctc_in"_a, "ctc_out"_a)

    .def("separate", &SepCtcPair::separate,
      BOXPAIR_SEPCTCPAIR_SEPARATE_CONST_INTERVALVECTOR_REF_CONST,
      "x"_a)
  ;
}