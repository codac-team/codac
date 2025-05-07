/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_IntvFullPivLU.h>
#include "codac2_py_IntvFullPivLU_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py):

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

void export_IntvFullPivLU(py::module& m)
{
  py::class_<IntvFullPivLU> exported(m, "IntvFullPivLU", INTVFULLPIVLU_MAIN);
  exported

    .def(py::init<const Matrix &>(),INTVFULLPIVLU_INTVFULLPIVLU_CONST_MATRIX_REF,
      "M"_a)

    .def(py::init<const IntervalMatrix &>(),INTVFULLPIVLU_INTVFULLPIVLU_CONST_INTERVALMATRIX_REF,
      "M"_a)

    .def("is_injective",&IntvFullPivLU::is_injective,BOOLINTERVAL_INTVFULLPIVLU_IS_INJECTIVE_CONST)

    .def("is_invertible",&IntvFullPivLU::is_invertible,BOOLINTERVAL_INTVFULLPIVLU_IS_INVERTIBLE_CONST)

    .def("is_surjective",&IntvFullPivLU::is_surjective,BOOLINTERVAL_INTVFULLPIVLU_IS_SURJECTIVE_CONST)

    .def("determinant",&IntvFullPivLU::determinant,INTERVAL_INTVFULLPIVLU_DETERMINANT_CONST)

    .def("rank",&IntvFullPivLU::rank,INTERVAL_INTVFULLPIVLU_RANK_CONST)

    .def("dimension_of_kernel",&IntvFullPivLU::dimension_of_kernel,INTERVAL_INTVFULLPIVLU_DIMENSION_OF_KERNEL_CONST)

    .def("kernel",&IntvFullPivLU::kernel,INTERVALMATRIX_INTVFULLPIVLU_KERNEL_CONST)

    .def("image",&IntvFullPivLU::image<IntervalMatrix>,DERIVED_INTVFULLPIVLU_IMAGE_CONST_EIGEN_MATRIXBASE_DERIVED_REF_CONST,"M"_a)

    .def("image",[](const IntvFullPivLU &S, const Matrix& M) { return S.image(M); },DERIVED_INTVFULLPIVLU_IMAGE_CONST_EIGEN_MATRIXBASE_DERIVED_REF_CONST,"M"_a)

    .def("solve",&IntvFullPivLU::solve,INTERVALMATRIX_INTVFULLPIVLU_SOLVE_CONST_INTERVALMATRIX_REF_CONST,"rhs"_a)

    .def("reconstructed_matrix",&IntvFullPivLU::reconstructed_matrix,INTERVALMATRIX_INTVFULLPIVLU_RECONSTRUCTED_MATRIX_CONST)

    .def("max_pivot",&IntvFullPivLU::max_pivot,DOUBLE_INTVFULLPIVLU_MAX_PIVOT_CONST)

    .def("permutation_P",&IntvFullPivLU::permutation_P,CONST_EIGEN_FULLPIVLU_MATRIX_PERMUTATIONPTYPE_REF_INTVFULLPIVLU_PERMUTATION_P_CONST)

    .def("permutation_Q",&IntvFullPivLU::permutation_Q,CONST_EIGEN_FULLPIVLU_MATRIX_PERMUTATIONQTYPE_REF_INTVFULLPIVLU_PERMUTATION_Q_CONST)

    .def("matrix_LU",&IntvFullPivLU::matrix_LU,CONST_INTERVALMATRIX_REF_INTVFULLPIVLU_MATRIX_LU_CONST)
     
  ;
}
