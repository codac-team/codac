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
#include <codac2_CtcPointCloud.h>
#include "codac2_py_Ctc.h"
#include "codac2_py_CtcPointCloud_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_CtcPointCloud(py::module& m, py::class_<CtcBase<IntervalVector>,pyCtcIntervalVector>& pyctc)
{
  py::class_<CtcPointCloud> exported(m, "CtcPointCloud", pyctc, CTCPOINTCLOUD_MAIN);
  exported

    .def(py::init<const std::vector<IntervalVector>&>(),
      CTCPOINTCLOUD_CTCPOINTCLOUD_CONST_VECTOR_INTERVALVECTOR_REF,
      "p"_a)

    .def(CONTRACT_BOX_METHOD(CtcPointCloud,
      VOID_CTCPOINTCLOUD_CONTRACT_INTERVALVECTOR_REF_CONST))

  ;
}