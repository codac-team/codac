/** 
 *  cart_prod binding
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

#include <codac2_cart_prod.h>

#include "codac2_py_cart_prod_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_cart_prod(py::module& m)
{
  m.def("cart_prod_vector", [](const std::list<Vector>& l) -> Vector
      {
        Vector c = *l.begin();
        for(auto it = std::next(l.cbegin()); it != l.cend(); it++)
          c = cart_prod(c,*it);
        return c;
      },
    VECTOR_CART_PROD_CONST_X_REF_VARIADIC);

  m.def("cart_prod_intervalvector", [](const std::list<IntervalVector>& l) -> IntervalVector
      {
        IntervalVector c = *l.begin();
        for(auto it = std::next(l.cbegin()); it != l.cend(); it++)
          c = cart_prod(c,*it);
        return c;
      },
    INTERVALVECTOR_CART_PROD_CONST_X_REF_VARIADIC);

  // The variadic version of this function is defined in __init__.py file
}