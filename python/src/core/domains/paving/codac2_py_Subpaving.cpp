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


template<typename T,typename... X>
void export_subpaving_base(py::class_<T>& c)
{

  //    Subpaving(const std::list<typename P::Node_>& l, const P::NodeValue_& node_value)
  //    IntervalVector box() const
  //    std::list<IntervalVector> boxes(bool non_flat = false) const
  //    std::list<IntervalVector> contour(bool sort = false) const
  //    std::list<IntervalVector> contour(const P::NodeValue_& node_complementary_value, bool sort = false) const
}

void export_Subpaving(py::module& m)
{
  py::class_<SubpavingOut> exported_subpaving_out(m, "SubpavingOut", SUBPAVING_MAIN);
  export_subpaving_base<SubpavingOut,IntervalVector>(exported_subpaving_out);
  exported_subpaving_out

  ;

  py::class_<SubpavingInOut> exported_subpaving_inout(m, "SubpavingInOut", SUBPAVING_MAIN);
  export_subpaving_base<SubpavingInOut,IntervalVector,IntervalVector>(exported_subpaving_inout);
  exported_subpaving_inout

  ;

}