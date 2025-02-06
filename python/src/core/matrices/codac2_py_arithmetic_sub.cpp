/** 
 *  Arithmetic binding
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
#include <codac2_Matrix.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

using B = Eigen::Block<Matrix>;
using IB = Eigen::Block<IntervalMatrix>;

void export_arithmetic_sub(
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<B>& py_B, py::class_<IB>& py_IB)
{
  // ====== First operand: vector

    py_V.def("__neg__", [](const Vector& x1) -> Vector { return -x1; }, py::is_operator());

    //inline Vector operator-(const Vector& x1, const Vector& x2)
    py_V.def("__sub__", [](const Vector& x1, const Vector& x2) -> Vector { return x1-x2; }, py::is_operator());

    //inline IntervalVector operator-(const Vector& x1, const IntervalVector& x2)
    py_V.def("__sub__", [](const Vector& x1, const IntervalVector& x2) -> IntervalVector { return x1.template cast<Interval>()-x2; }, py::is_operator());

    py_V.def("__isub__", [](Vector& x1, const Vector& x2) { return x1-=x2; }, py::is_operator());

  // ====== First operand: matrix

    py_M.def("__neg__", [](const Matrix& x1) -> Matrix { return -x1; }, py::is_operator());

    //Matrix operator-(const M& x1, const M_& x2)
    py_M.def("__sub__", [](const Matrix& x1, const Matrix& x2) -> Matrix { return x1-x2; }, py::is_operator());
    py_M.def("__sub__", [](const Matrix& x1, const B& x2) -> Matrix { return x1-x2; }, py::is_operator());
    py_B.def("__sub__", [](const B& x1, const Matrix& x2) -> Matrix { return x1-x2; }, py::is_operator());
    py_B.def("__sub__", [](const B& x1, const B& x2) -> Matrix { return x1-x2; }, py::is_operator());

    //IntervalMatrix operator-(const M& x1, const IM& x2)
    py_M.def("__sub__", [](const Matrix& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1.template cast<Interval>()-x2; }, py::is_operator());
    py_M.def("__sub__", [](const Matrix& x1, const IB& x2) -> IntervalMatrix { return x1.template cast<Interval>()-x2; }, py::is_operator());
    py_B.def("__sub__", [](const B& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1.template cast<Interval>()-x2; }, py::is_operator());
    py_B.def("__sub__", [](const B& x1, const IB& x2) -> IntervalMatrix { return x1.template cast<Interval>()-x2; }, py::is_operator());

    py_M.def("__isub__", [](Matrix& x1, const Matrix& x2) { return x1-=x2; }, py::is_operator());
    py_M.def("__isub__", [](Matrix& x1, const B& x2) { return x1-=x2; }, py::is_operator());

  // ====== First operand: interval vector

    py_IV.def("__neg__", [](const IntervalVector& x1) -> IntervalVector { return -x1; }, py::is_operator());

    //inline IntervalVector operator-(const IntervalVector& x1, const Vector& x2)
    py_IV.def("__sub__", [](const IntervalVector& x1, const Vector& x2) -> IntervalVector { return x1-x2.template cast<Interval>(); }, py::is_operator());

    //inline IntervalVector operator-(const IntervalVector& x1, const IntervalVector& x2)
    py_IV.def("__sub__", [](const IntervalVector& x1, const IntervalVector& x2) -> IntervalVector { return x1-x2; }, py::is_operator());

    py_IV.def("__isub__", [](IntervalVector& x1, const Vector& x2) { return x1-=x2.template cast<Interval>(); }, py::is_operator());
    py_IV.def("__isub__", [](IntervalVector& x1, const IntervalVector& x2) { return x1-=x2; }, py::is_operator());

  // ====== First operand: interval matrix

    py_IM.def("__neg__", [](const IntervalMatrix& x1) -> IntervalMatrix { return -x1; }, py::is_operator());

    //IntervalMatrix operator-(const IM& x1, const M& x2)
    py_IM.def("__sub__", [](const IntervalMatrix& x1, const Matrix& x2) -> IntervalMatrix { return x1-x2.template cast<Interval>(); }, py::is_operator());
    py_IM.def("__sub__", [](const IntervalMatrix& x1, const B& x2) -> IntervalMatrix { return x1-x2.template cast<Interval>(); }, py::is_operator());
    py_IB.def("__sub__", [](const IB& x1, const Matrix& x2) -> IntervalMatrix { return x1-x2.template cast<Interval>(); }, py::is_operator());
    py_IB.def("__sub__", [](const IB& x1, const B& x2) -> IntervalMatrix { return x1-x2.template cast<Interval>(); }, py::is_operator());

    //IntervalMatrix operator-(const IM& x1, const IM_& x2)
    py_IM.def("__sub__", [](const IntervalMatrix& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1-x2; }, py::is_operator());
    py_IM.def("__sub__", [](const IntervalMatrix& x1, const IB& x2) -> IntervalMatrix { return x1-x2; }, py::is_operator());
    py_IB.def("__sub__", [](const IB& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1-x2; }, py::is_operator());
    py_IB.def("__sub__", [](const IB& x1, const IB& x2) -> IntervalMatrix { return x1-x2; }, py::is_operator());

    py_IM.def("__isub__", [](IntervalMatrix& x1, const Matrix& x2) { return x1-=x2.template cast<Interval>(); }, py::is_operator());
    py_IM.def("__isub__", [](IntervalMatrix& x1, const B& x2) { return x1-=x2.template cast<Interval>(); }, py::is_operator());
    py_IM.def("__isub__", [](IntervalMatrix& x1, const IntervalMatrix& x2) { return x1-=x2; }, py::is_operator());
    py_IM.def("__isub__", [](IntervalMatrix& x1, const IB& x2) { return x1-=x2; }, py::is_operator());
    
}