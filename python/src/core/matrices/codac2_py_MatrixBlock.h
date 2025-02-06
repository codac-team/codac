/** 
 *  MatrixBase binding
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "codac2_py_doc.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

template<typename S>
py::class_<Eigen::Block<S>> export_EigenBlock(py::module& m, const std::string& name)
{
  py::class_<Eigen::Block<S>> exported_mbb_class(m, name.c_str(), DOC_TO_BE_DEFINED);

  exported_mbb_class

    .def("rows", &Eigen::Block<S>::rows,
      DOC_TO_BE_DEFINED)

    .def("cols", &Eigen::Block<S>::cols,
      DOC_TO_BE_DEFINED)

    .def("eval", &Eigen::Block<S>::eval,
      DOC_TO_BE_DEFINED)
    
    .def(py::self == py::self)
    .def(py::self != py::self)

    .def("__repr__", [](const Eigen::Block<S>& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      DOC_TO_BE_DEFINED)
    
  ;
  
  py::implicitly_convertible<Eigen::Block<S>,S>();

  return exported_mbb_class;
}