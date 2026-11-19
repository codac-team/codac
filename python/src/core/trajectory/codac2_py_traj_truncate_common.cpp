/** 
 *  traj_truncate_common binding
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
#include <codac2_TrajBase.h>
#include "codac2_py_TrajBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_cast.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_traj_truncate_common(py::module& m)
{
  m.def("truncate_to_common_tdomain", [](py::args trajs)
      {
        if(trajs.empty())
          throw py::value_error("At least one trajectory is required");

        Interval t;

        for(const auto& traj : trajs)
          t &= traj.attr("tdomain")().cast<Interval>();

        if(t.is_empty())
          throw py::value_error("Trajectories have disjoint time domains");

        for(const auto& traj : trajs)
          traj.attr("truncate_tdomain")(t);
      },
    VOID_TRUNCATE_TO_COMMON_TDOMAIN_T_REF_VARIADIC);
}