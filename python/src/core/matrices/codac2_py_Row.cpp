/** 
 *  Row binding, from Vector bindings
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Row.h>
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
#include "codac2_py_Row_docs.h"

#include "codac2_py_VectorBase.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

py::class_<Row> export_Row(py::module& m)
{
  py::class_<Row> exported_row_class(m, "Row", DOC_TO_BE_DEFINED);
  export_MatrixBase<Row,double,true>(m, exported_row_class);
  export_VectorBase<Row,Matrix,double>(m, exported_row_class);

  exported_row_class

    .def(py::init(
        [](Index_type n)
        {
          matlab::test_integer(n);
          return std::make_unique<Row>((int)n);
        }),
      DOC_TO_BE_DEFINED,
      "n"_a)

    .def(py::init<const Row&>(),
      "x"_a)

    .def(py::init( // this constructor must be the last one to be declared
        [](const std::vector<double>& v_)
        {
          auto v = std::make_unique<Row>(v_.size());
          for(size_t i = 0 ; i < v_.size() ; i++)
            (*v)[i] = v_[i];
          return v;
        }),
      MATRIX_ADDONS_VECTOR_MATRIX_INITIALIZER_LIST_DOUBLE,
      "v"_a)

    .def("min_coeff_index", [](const Row& x)
        {
          return matlab::output_index(x.min_coeff_index());
        },
      MATRIXBASE_ADDONS_VECTOR_INDEX_MIN_COEFF_INDEX_CONST)

    .def("max_coeff_index", [](const Row& x)
        {
          return matlab::output_index(x.max_coeff_index());
        },
      MATRIXBASE_ADDONS_VECTOR_INDEX_MAX_COEFF_INDEX_CONST)

    .def("__repr__", [](const Row& x)
        {
          std::ostringstream s;
          s << x;
          return string(s.str()); 
        },
      DOC_TO_BE_DEFINED)
    
  ;

  py::implicitly_convertible<py::list,Row>();
  
  return exported_row_class;
}
