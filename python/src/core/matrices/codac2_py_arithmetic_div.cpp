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
#include <codac2_arithmetic_div.h>

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

using B = MatrixBaseBlock<EigenMatrix<double>&,double>;
using IB = MatrixBaseBlock<EigenMatrix<Interval>&,Interval>;

void export_arithmetic_div(py::module& m,
  py::class_<Vector>& py_V, py::class_<IntervalVector>& py_IV,
  py::class_<Matrix>& py_M, py::class_<IntervalMatrix>& py_IM,
  py::class_<B>& py_B, py::class_<IB>& py_IB)
{
  // ====== First operand: vector

    //inline Vector operator/(const Vector& x1, double x2)
    py_V.def("__truediv__", [](const Vector& x1, double x2) { return x1/x2; }, py::is_operator());

    //inline IntervalVector operator/(const Vector& x1, const Interval& x2)
    py_V.def("__truediv__", [](const Vector& x1, const Interval& x2) { return x1/x2; }, py::is_operator());

    py_V.def("__itruediv__", [](Vector& x1, double x2) { return x1/=x2; }, py::is_operator());

  // ====== First operand: matrix

    //Matrix operator/(const M& x1, double x2)
    py_M.def("__truediv__", [](const Matrix& x1, double x2) { return x1/x2; }, py::is_operator());
    py_B.def("__truediv__", [](const B& x1, double x2) { return x1/x2; }, py::is_operator());

    //IntervalMatrix operator/(const M& x1, const Interval& x2)
    py_M.def("__truediv__", [](const Matrix& x1, const Interval& x2) { return x1/x2; }, py::is_operator());
    py_B.def("__truediv__", [](const B& x1, const Interval& x2) { return x1/x2; }, py::is_operator());

    py_M.def("__itruediv__", [](Matrix& x1, double x2) { return x1/=x2; }, py::is_operator());

  // ====== First operand: interval vector

    //inline IntervalVector operator/(const IntervalVector& x1, double x2)
    py_IV.def("__truediv__", [](const IntervalVector& x1, double x2) { return x1/x2; }, py::is_operator());

    //inline IntervalVector operator/(const IntervalVector& x1, const Interval& x2)
    py_IV.def("__truediv__", [](const IntervalVector& x1, const Interval& x2) { return x1/x2; }, py::is_operator());

    py_IV.def("__itruediv__", [](IntervalVector& x1, double x2) { return x1/=x2; }, py::is_operator());
    py_IV.def("__itruediv__", [](IntervalVector& x1, const Interval& x2) { return x1/=x2; }, py::is_operator());

  // ====== First operand: interval matrix

    //IntervalMatrix operator/(const IM& x1, double x2)
    py_IM.def("__truediv__", [](const IntervalMatrix& x1, double x2) { return x1/x2; }, py::is_operator());
    py_IB.def("__truediv__", [](const IB& x1, double x2) { return x1/x2; }, py::is_operator());

    //IntervalMatrix operator/(const IM& x1, const Interval& x2)
    py_IM.def("__truediv__", [](const IntervalMatrix& x1, const Interval& x2) { return x1/x2; }, py::is_operator());
    py_IB.def("__truediv__", [](const IB& x1, const Interval& x2) { return x1/x2; }, py::is_operator());

    py_IM.def("__itruediv__", [](IntervalMatrix& x1, double x2) { return x1/=x2; }, py::is_operator());
    py_IM.def("__itruediv__", [](IntervalMatrix& x1, const Interval& x2) { return x1/=x2; }, py::is_operator());

}