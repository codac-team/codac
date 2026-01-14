/** 
 *  tube_cart_prod binding
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
#include <codac2_tube_cart_prod.h>
#include "codac2_py_tube_cart_prod_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_tube_cart_prod(py::module& m)
{
  m.def("tube_cart_prod_list", [](const std::list<py::object>& l) -> SlicedTube<IntervalVector>
      {
        Index n = 0;
        for(const auto& li : l)
          n += cast<SlicedTube<IntervalVector>>(li).size();

        const SlicedTube<IntervalVector>& x0 = cast<SlicedTube<IntervalVector>>(*l.begin());
        SlicedTube<IntervalVector> v(x0.tdomain(), IntervalVector(n));

        for(auto it = x0.tdomain()->begin(); it != x0.tdomain()->end(); it++)
        {
          Index i = 0;
          for(const auto& li : l)
          {
            auto si = cast<SlicedTube<IntervalVector>>(li).slice(it)->codomain();
            v.slice(it)->codomain().put(i,si);
            i += si.size();
          }
          assert(i == n);
        }

        return v;
      },
    SLICEDTUBE_INTERVALVECTOR_TUBE_CART_PROD_CONST_X_REF_VARIADIC);

  // The variadic version of this function is defined in __init__.py file
}