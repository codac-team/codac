/** 
 *  Matrix binding
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
#include <codac2_Matrix.h>
#include <codac2_IntervalMatrix.h>

#include "codac2_py_doc.h"
#include "codac2_py_Matrix_addons_Base_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
//#include "codac2_py_Matrix_addons_IntervalMatrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_addons_IntervalMatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_addons_IntervalVector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
//#include "codac2_py_Matrix_addons_Matrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_addons_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_addons_Vector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_addons_VectorBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_MatrixBase_addons_Base_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
//#include "codac2_py_MatrixBase_addons_IntervalMatrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_MatrixBase_addons_IntervalMatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_MatrixBase_addons_IntervalVector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
//#include "codac2_py_MatrixBase_addons_Matrix_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
//#include "codac2_py_MatrixBase_addons_MatrixBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_MatrixBase_addons_Vector_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_MatrixBase_addons_VectorBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_matrices_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_Matrix_docs.h"

#include "codac2_py_MatrixBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Matrix> export_Matrix(py::module& m)
{
  py::class_<Matrix> exported_matrix_class(m, "Matrix", DOC_TO_BE_DEFINED);
  export_MatrixBase<Matrix,double,false>(m, exported_matrix_class);

  exported_matrix_class

    .def(py::init(
        [](Index_type r, Index_type c)
        {
          matlab::test_integer(r,c);
          return std::make_unique<Matrix>(r,c);
        }),
      DOC_TO_BE_DEFINED,
      "r"_a, "c"_a)

    .def(py::init<const Matrix&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init<const Vector&>(),
      DOC_TO_BE_DEFINED,
      "x"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<Vector>& v)
        {
          assert_release(!std::empty(v));
          auto m = std::make_unique<Matrix>(v.size(),v[0].size());
          for(size_t i = 0 ; i < v.size() ; i++)
          {
            assert_release(v[i].size() == m->cols() && "Vector objects of different size");
            m->row(i) = v[i].transpose();
          }
          return m;
        }),
      DOC_TO_BE_DEFINED,
      "v"_a)

    .def("inverse", [](const Matrix& x)
        {
          return x.inverse().eval();
        },
      DOC_TO_BE_DEFINED)

  ;

  return exported_matrix_class;
}