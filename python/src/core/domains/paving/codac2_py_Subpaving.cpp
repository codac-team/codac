/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <codac2_Subpaving.h>
#include <codac2_Paving.h>
#include "codac2_py_Subpaving_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename T,typename P>
void export_subpaving_base(py::class_<T>& c)
{
  c

    .def(py::init<const std::list<IntervalVector>&>(),
      SUBPAVING_P_SUBPAVING_CONST_LIST_INTERVALVECTOR_REF,
      "l"_a)

    .def("box", &T::box,
      INTERVALVECTOR_SUBPAVING_P_BOX_CONST)

    .def("boxes", &T::boxes,
      LIST_INTERVALVECTOR_SUBPAVING_P_BOXES_CONST)

    /*.def("contour", (std::list<IntervalVector>(T::*)(bool) const) &T::contour,
      LIST_INTERVALVECTOR_SUBPAVING_P_CONTOUR_BOOL_CONST,
      "sort"_a = false)

    .def("contour", (std::list<IntervalVector>(T::*)(const typename P::NodeValue_&, bool) const) &T::contour,
      LIST_INTERVALVECTOR_SUBPAVING_P_CONTOUR_CONST_P_NODEVALUE__REF_BOOL_CONST,
      "node_complementary_value"_a, "sort"_a = false)*/

  ;
}

void export_Subpaving(py::module& m)
{
  py::class_<SubpavingOut> exported_subpaving_out(m, "SubpavingOut", SUBPAVING_MAIN);
  export_subpaving_base<SubpavingOut,PavingOut>(exported_subpaving_out);

  py::class_<SubpavingInOut> exported_subpaving_inout(m, "SubpavingInOut", SUBPAVING_MAIN);
  export_subpaving_base<SubpavingInOut,PavingInOut>(exported_subpaving_inout);
}