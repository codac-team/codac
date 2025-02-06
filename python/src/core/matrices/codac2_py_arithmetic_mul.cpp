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

void export_arithmetic_mul(
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<B>& py_B, py::class_<IB>& py_IB)
{
  // ====== First operand: double

    //inline Vector operator*(double x1, const Vector& x2)
    py_V.def("__rmul__", [](const Vector& x2, double x1) -> Vector { return x1*x2; }, py::is_operator());

    //Matrix operator*(double x1, const M& x2)
    py_M.def("__rmul__", [](const Matrix& x2, double x1) -> Matrix { return x1*x2; }, py::is_operator());
    py_B.def("__rmul__", [](const B& x2, double x1) -> Matrix { return x1*x2; }, py::is_operator());

    //inline IntervalVector operator*(double x1, const IntervalVector& x2)
    py_IV.def("__rmul__", [](const IntervalVector& x2, double x1) -> IntervalVector { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(double x1, const IM& x2)
    py_IM.def("__rmul__", [](const IntervalMatrix& x2, double x1) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_IB.def("__rmul__", [](const IB& x2, double x1) -> IntervalMatrix { return x1*x2; }, py::is_operator());

  // ====== First operand: interval

    //inline IntervalVector operator*(const Interval& x1, const Vector& x2)
    py_V.def("__rmul__", [](const Vector& x2, const Interval& x1) -> IntervalVector { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(const Interval& x1, const M& x2)
    py_M.def("__rmul__", [](const Matrix& x2, const Interval& x1) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_B.def("__rmul__", [](const B& x2, const Interval& x1) -> IntervalMatrix { return x1*x2; }, py::is_operator());

    //inline IntervalVector operator*(const Interval& x1, const IntervalVector& x2)
    py_IV.def("__rmul__", [](const IntervalVector& x2, const Interval& x1) -> IntervalVector { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(const Interval& x1, const IM& x2)
    py_IM.def("__rmul__", [](const IntervalMatrix& x2, const Interval& x1) -> IntervalMatrix{ return x1*x2; }, py::is_operator());
    py_IB.def("__rmul__", [](const IB& x2, const Interval& x1) -> IntervalMatrix { return x1*x2; }, py::is_operator());

  // ====== First operand: vector

    //inline Vector operator*(const Vector& x1, double x2)
    py_V.def("__mul__", [](const Vector& x1, double x2) -> Vector { return x1*x2; }, py::is_operator());

    //inline IntervalVector operator*(const Vector& x1, const Interval& x2)
    py_V.def("__mul__", [](const Vector& x1, const Interval& x2) -> IntervalVector { return x1*x2; }, py::is_operator());

    py_V.def("__imul__", [](Vector& x1, double x2) { return x1*=x2; }, py::is_operator());

  // ====== First operand: matrix

    //Interval operator*(const M& x1, double x2)
    py_M.def("__mul__", [](const Matrix& x1, double x2) -> Matrix { return x1*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, double x2) -> Matrix { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(const M& x1, const Interval& x2)
    py_M.def("__mul__", [](const Matrix& x1, const Interval& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const Interval& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());

    //Vector operator*(const M& x1, const Vector& x2)
    py_M.def("__mul__", [](const Matrix& x1, const Vector& x2) -> Vector { return x1*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const Vector& x2) -> Vector { return x1*x2; }, py::is_operator());

    //Matrix operator*(const M& x1, const M_& x2)
    py_M.def("__mul__", [](const Matrix& x1, const Matrix& x2) -> Matrix { return x1*x2; }, py::is_operator());
    py_M.def("__mul__", [](const Matrix& x1, const B& x2) -> Matrix { return x1*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const Matrix& x2) -> Matrix { return x1*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const B& x2) -> Matrix { return x1*x2; }, py::is_operator());

    //IntervalVector operator*(const M& x1, const IntervalVector& x2)
    py_M.def("__mul__", [](const Matrix& x1, const IntervalVector& x2) -> IntervalVector { return x1*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const IntervalVector& x2) -> IntervalVector { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(const M& x1, const IM& x2)
    py_M.def("__mul__", [](const Matrix& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1.template cast<Interval>()*x2; }, py::is_operator());
    py_M.def("__mul__", [](const Matrix& x1, const IB& x2) -> IntervalMatrix { return x1.template cast<Interval>()*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1.template cast<Interval>()*x2; }, py::is_operator());
    py_B.def("__mul__", [](const B& x1, const IB& x2) -> IntervalMatrix { return x1.template cast<Interval>()*x2; }, py::is_operator());

    py_M.def("__imul__", [](Matrix& x1, double x2) { return x1*=x2; }, py::is_operator());
    py_M.def("__imul__", [](Matrix& x1, const Matrix& x2) { return x1*=x2; }, py::is_operator());
    py_M.def("__imul__", [](Matrix& x1, const B& x2) { return x1*=x2; }, py::is_operator());

  // ====== First operand: interval vector

    //inline IntervalVector operator*(const IntervalVector& x1, double x2)
    py_IV.def("__mul__", [](const IntervalVector& x1, double x2) -> IntervalVector { return x1*x2; }, py::is_operator());

    //inline IntervalVector operator*(const IntervalVector& x1, const Interval& x2)
    py_IV.def("__mul__", [](const IntervalVector& x1, const Interval& x2) -> IntervalVector { return x1*x2; }, py::is_operator());

    py_IV.def("__imul__", [](IntervalVector& x1, double x2) { return x1*=x2; }, py::is_operator());
    py_IV.def("__imul__", [](IntervalVector& x1, const Interval& x2) { return x1*=x2; }, py::is_operator());

  // ====== First operand: interval matrix

    //IntervalMatrix operator*(const IM& x1, double x2)
    py_IM.def("__mul__", [](const IntervalMatrix& x1, double x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, double x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(const IM& x1, const Interval& x2)
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const Interval& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const Interval& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());

    //IntervalVector operator*(const IM& x1, const Vector& x2)
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const Vector& x2) -> IntervalVector { return x1*x2.template cast<Interval>(); }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const Vector& x2) -> IntervalVector { return x1*x2.template cast<Interval>(); }, py::is_operator());

    //IntervalMatrix operator*(const IM& x1, const M& x2)
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const Matrix& x2) -> IntervalMatrix { return x1*x2.template cast<Interval>(); }, py::is_operator());
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const B& x2) -> IntervalMatrix { return x1*x2.template cast<Interval>(); }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const Matrix& x2) -> IntervalMatrix { return x1*x2.template cast<Interval>(); }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const B& x2) -> IntervalMatrix { return x1*x2.template cast<Interval>(); }, py::is_operator());

    //IntervalVector operator*(const IM& x1, const IntervalVector& x2)
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const IntervalVector& x2) -> IntervalVector { return x1*x2; }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const IntervalVector& x2) -> IntervalVector { return x1*x2; }, py::is_operator());

    //IntervalMatrix operator*(const IM& x1, const IM_& x2)
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_IM.def("__mul__", [](const IntervalMatrix& x1, const IB& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const IntervalMatrix& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());
    py_IB.def("__mul__", [](const IB& x1, const IB& x2) -> IntervalMatrix { return x1*x2; }, py::is_operator());

    py_IM.def("__imul__", [](IntervalMatrix& x1, double x2) { return x1*=x2; }, py::is_operator());
    py_IM.def("__imul__", [](IntervalMatrix& x1, const Matrix& x2) { return x1*=x2.template cast<Interval>(); }, py::is_operator());
    py_IM.def("__imul__", [](IntervalMatrix& x1, const B& x2) { return x1*=x2.template cast<Interval>(); }, py::is_operator());
    py_IM.def("__imul__", [](IntervalMatrix& x1, const Interval& x2) { return x1*=x2; }, py::is_operator());
    py_IM.def("__imul__", [](IntervalMatrix& x1, const IntervalMatrix& x2) { return x1*=x2; }, py::is_operator());
    py_IM.def("__imul__", [](IntervalMatrix& x1, const IB& x2) { return x1*=x2; }, py::is_operator());

}