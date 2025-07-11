/** 
 *  \file
 *  Codac binding (unsupported)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <codac2_Interval.h>
#include <codac2_math.h>

#include <iostream>

using namespace codac2;
namespace py = pybind11;

// PEIBOS
void export_Parallelepiped(py::module& m);
void export_Zonotope(py::module& m);
void export_peibos(py::module& m);

#ifdef PEIBOS_CAPD
  void export_IMapWrapper(py::module& m);
  void export_peibos_capd(py::module& m);
#endif

PYBIND11_MODULE(_unsupported, m)
{
  m.doc() = "Python binding of Codac (unsupported)";

  // PEIBOS
  export_Parallelepiped(m);
  export_Zonotope(m);
  export_peibos(m);

  #ifdef PEIBOS_CAPD
    export_IMapWrapper(m);
    export_peibos_capd(m);
    // Forcing back the rounding managed by GAOL (while keeping the same mode), maybe one day I will understand...
    if (get_rounding_mode()=="FE_UPWARD")
      round_up();
    else if (get_rounding_mode()=="FE_DOWNWARD")
      round_down();
  #endif

}