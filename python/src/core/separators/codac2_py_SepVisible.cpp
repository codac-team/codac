/** * Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <codac2_SepVisible.h>
#include "codac2_py_Sep.h"
#include "codac2_py_SepVisible_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_SepVisible(py::module& m, py::class_<SepBase, pySep>& pysep)
{
  py::class_<SepVisible> exported(m, "SepVisible", pysep, SEPVISIBLE_MAIN);
  exported
    .def(py::init<const IntervalVector&, const Segment&>(),
      "a"_a, "s"_a, 
      SEPVISIBLE_SEPVISIBLE_CONST_INTERVALVECTOR_REF_CONST_SEGMENT_REF)

    .def(py::init<const IntervalVector&, const std::vector<Segment>&>(),
      "a"_a, "s"_a, 
      SEPVISIBLE_SEPVISIBLE_CONST_INTERVALVECTOR_REF_CONST_VECTOR_SEGMENT_REF)

    .def(py::init<const IntervalVector&, const Polygon&>(),
      "a"_a, "p"_a, 
      SEPVISIBLE_SEPVISIBLE_CONST_INTERVALVECTOR_REF_CONST_POLYGON_REF)
    
    .def("separate", &SepVisible::separate, 
      "x"_a, 
      BOXPAIR_SEPVISIBLE_SEPARATE_CONST_INTERVALVECTOR_REF_CONST)
  ;
}
