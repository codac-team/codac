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
#include <codac2_Vector.h>
#include <codac2_IntervalMatrix.h>

#include "codac2_py_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_VectorBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Vector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_VectorBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Vector> export_Vector(py::module& m)
{
  py::class_<Vector> exported_vector_class(m, "Vector", VECTOR_MAIN);
  export_MatrixBase<Vector,double,true>(m, exported_vector_class);
  export_VectorBase<Vector,Matrix,double>(m, exported_vector_class);

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
        [](size_t_type n, double x)
        {
          matlab::test_integer(n);
          return std::make_unique<Vector>(n,x);
        }),
      VECTOR_VECTOR_SIZET_DOUBLE,
      "n"_a, "x"_a)

    .def(py::init<const Vector&>(),
      "x"_a)

    .def(py::init<const std::vector<double>>(), // this constructor must be the last one to be declared
      VECTOR_VECTOR_CONST_VECTOR_DOUBLE_REF,
      "x"_a)

    .def("min_coeff_index", [](const Vector& x)
        {
          return matlab::output_index(x.min_coeff_index());
        },
      SIZET_VECTOR_MIN_COEFF_INDEX_CONST)

    .def("max_coeff_index", [](const Vector& x)
        {
          return matlab::output_index(x.max_coeff_index());
        },
      SIZET_VECTOR_MAX_COEFF_INDEX_CONST)

    .def("__repr__", [](const Vector& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_VECTOR_REF)
    
  ;

  py::implicitly_convertible<py::list,Vector>();
  
  return exported_vector_class;
}