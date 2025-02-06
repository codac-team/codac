/** 
 *  Inversion of matrices
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_inversion.h>

#include "codac2_py_doc.h"
#include "codac2_py_inversion_docs.h" // Generated file from Doxygen 


using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_Inversion(py::module& m)
{
  m

   .def("infinite_sum_enclosure", [](const IntervalMatrix& A) { double mrad; return infinite_sum_enclosure(A,mrad); },
    INTERVALMATRIX_INFINITE_SUM_ENCLOSURE_CONST_INTERVALMATRIX_REF_DOUBLE_REF,
    "A"_a)

   .def("inverse_correction", [](const IntervalMatrix& A, const IntervalMatrix& B, bool left_inv = true)
    {
      return left_inv ? inverse_correction<LEFT_INV>(A,B) : inverse_correction<RIGHT_INV>(A,B);
    },
    INTERVALMATRIX_INVERSE_CORRECTION_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF_CONST_EIGEN_MATRIXBASE_OTHERDERIVED__REF,
    "A"_a, "B"_a, "left_inv"_a)

   .def("inverse_enclosure", [](const Matrix& A) { return inverse_enclosure(A); },
  INTERVALMATRIX_INVERSE_ENCLOSURE_CONST_EIGEN_MATRIXBASE_OTHERDERIVED_REF,
  "A"_a)

   .def("inverse_enclosure", 
  (IntervalMatrix(*)(const IntervalMatrix&))
   &codac2::inverse_enclosure,
  INTERVALMATRIX_INVERSE_ENCLOSURE_CONST_INTERVALMATRIX_REF,
  "A"_a)

   ;
}
