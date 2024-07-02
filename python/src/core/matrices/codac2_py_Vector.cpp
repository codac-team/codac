/** 
 *  Vector binding
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
#include "codac2_py_core.h"
#include <codac2_Vector.h>
#include "codac2_py_Vector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Vector> export_Vector(py::module& m)
{
  py::class_<Vector> exported_vector_class(m, "Vector", VECTOR_MAIN);
  exported_vector_class

    .def(py::init(
        [](size_t_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<Vector>(n);
        }),
      VECTOR_VECTOR_SIZET,
      "n"_a)

    .def(py::init(
        [](const std::vector<double>& v)
        {
          auto iv = std::make_unique<Vector>(v.size());
          for(size_t i = 0 ; i < v.size() ; i++)
            (*iv)[i] = v[i];
          return iv;
        }),
      VECTOR_VECTOR_INITIALIZER_LIST_DOUBLE,
      "x"_a)

    .def(py::init<const Vector&>(),
      "x"_a)
  ;

  py::implicitly_convertible<py::list,Vector>();
  return exported_vector_class;
}