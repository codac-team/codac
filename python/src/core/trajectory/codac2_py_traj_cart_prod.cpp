/** 
 *  traj_cart_prod binding
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
#include <codac2_traj_cart_prod.h>
#include "codac2_py_traj_cart_prod_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_traj_cart_prod(py::module& m)
{
  m.def("traj_cart_prod_list", [](const std::list<py::object>& l) -> SampledTraj<Vector>
      {
        SampledTraj<Vector> x = is_instance<SampledTraj<Vector>>(*l.begin()) ?
          cast<SampledTraj<Vector>>(*l.begin()) : traj_cart_prod(cast<SampledTraj<double>>(*l.begin()));
        for(auto it = std::next(l.begin()); it != l.end(); it++)
          x = traj_cart_prod(x, is_instance<SampledTraj<Vector>>(*it) ?
          cast<SampledTraj<Vector>>(*it) : traj_cart_prod(cast<SampledTraj<double>>(*it)));
        return x;
      },
    SAMPLEDTRAJ_VECTOR_TRAJ_CART_PROD_CONST_X_REF_VARIADIC);

  // The variadic version of this function is defined in __init__.py file
}