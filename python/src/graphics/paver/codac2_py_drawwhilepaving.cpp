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
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_drawwhilepaving.h>
#include "codac2_py_drawwhilepaving_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_drawwhilepaving(py::module& m)
{
  m.def("draw_while_paving", (void (*)(const IntervalVector&,const CtcBase<IntervalVector>&,double,std::shared_ptr<Figure2D>))&draw_while_paving,
    VOID_DRAW_WHILE_PAVING_CONST_INTERVALVECTOR_REF_CONST_CTCBASE_INTERVALVECTOR_REF_DOUBLE_SHARED_PTR_FIGURE2D,
    "x0"_a, "c"_a, "eps"_a, "fig"_a=nullptr);

  m.def("draw_while_paving", (void (*)(const IntervalVector&,const SepBase&,double,std::shared_ptr<Figure2D>))&draw_while_paving,
    VOID_DRAW_WHILE_PAVING_CONST_INTERVALVECTOR_REF_CONST_SEPBASE_REF_DOUBLE_SHARED_PTR_FIGURE2D,
    "x0"_a, "s"_a, "eps"_a, "fig"_a=nullptr);
}