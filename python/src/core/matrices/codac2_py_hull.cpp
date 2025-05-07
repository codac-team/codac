/** 
 *  hull binding
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

#include <codac2_hull.h>

#include "codac2_py_hull_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename T>
inline T list_to_hull(const std::list<py::object>& l)
{
  T c = cast<T>(*l.begin());
  for(auto it = std::next(l.cbegin()); it != l.cend(); it++)
    c = hull(c,cast<T>(*it));
  return c;
}

void export_hull(py::module& m)
{
  m.def("hull_intervalvector", &list_to_hull<IntervalVector>,
    WRAPPER_X1_DOMAIN_HULL_CONST_X1_REF_CONST_X_REF_VARIADIC);

  m.def("hull_intervalmatrix", &list_to_hull<IntervalMatrix>,
    WRAPPER_X1_DOMAIN_HULL_CONST_X1_REF_CONST_X_REF_VARIADIC);

  // The variadic version of this function is defined in __init__.py file
}