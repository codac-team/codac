/** 
 *  \file
 *  IntervalMatrix Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <string>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "codac_type_caster.h"

#include "ibex_Matrix.h"
#include "codac_Interval.h"
#include "codac_IntervalMatrix.h"
#include "codac_matrix_arithmetic.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
// todo: #include "codac_py_IntervalMatrix_docs.h"

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


IntervalMatrix* create_from_list(int nb_rows, int nb_cols, vector<Interval>& lst)
{
  size_t size = lst.size();
  if((int)size != nb_cols*nb_rows)
    throw invalid_argument("Invalid size of input list");
  
  double (*tmp)[2] = new double[size][2];
  size_t index = 0;
  for(auto & v : lst){
    tmp[index][0] = v.lb();
    tmp[index][1] = v.ub();
    index++;
  }

  IntervalMatrix* instance = new IntervalMatrix(nb_rows, nb_cols, tmp);
  delete[] tmp;
  return instance;
  // todo: manage delete
}

void assign_IntervalMatrix(IntervalMatrix& x, const IntervalMatrix& y)
{
  x = y;
}

IntervalMatrix copy_IntervalMatrix(IntervalMatrix& x)
{
  return IntervalMatrix(x);
}

string to_string(const IntervalMatrix& x)
{
  stringstream ss;
  ss << x;
  return ss.str();
}

void export_IntervalMatrix(py::module& m)
{
  py::class_<IntervalMatrix> interval_matrix(m, "IntervalMatrix");
  interval_matrix

  .def(py::init<int,int>())
  .def(py::init<int,int,const Interval>())
  .def(py::init<const IntervalMatrix&>())
  .def(py::init(&create_from_list))

  .def(py::init([](py::buffer b) // create for instance an IntervalMatrix from a NumPy matrix
  {
    // Request a buffer descriptor from Python
    py::buffer_info info = b.request();

    // Some sanity checks...
    if(info.format != py::format_descriptor<double>::format())
      throw std::runtime_error("Incompatible format: expected a double array");

    if(info.ndim != 2)
      throw std::runtime_error("Incompatible buffer dimension");

    ibex::Matrix m((int)info.shape[0], (int)info.shape[1], static_cast<double*>(info.ptr));
    return IntervalMatrix(m);
  }))

  .def("__getitem__", [](IntervalMatrix& s, size_t index) -> IntervalVector&
    {
      if(index < 0 || index >= static_cast<size_t>(s.nb_rows()))
        throw py::index_error();
      return s[static_cast<int>(index)];
    },
    py::return_value_policy::reference_internal)

  .def("__setitem__", [](IntervalMatrix& s, size_t index, IntervalVector& t)
    {
      if(index < 0 || index >= static_cast<size_t>(s.nb_rows()))
        throw py::index_error();
      s[static_cast<int>(index)] = t;
    })

  .def("assign", &assign_IntervalMatrix)
  .def(py::self == py::self)
  .def(py::self != py::self)
  .def(py::self + py::self)
  .def(py::self - py::self)
  .def(py::self * py::self)
  .def(py::self &= py::self)
  .def(-py::self)
  .def(py::self += py::self)
  // todo: .def(py::self += other<Matrix>())
  .def(py::self -= py::self)
  // todo: .def(py::self -= other<Matrix>())
  .def(py::self *= py::self)
  // .def(self *= double())
  .def("__mul__", [](IntervalMatrix& m, const IntervalVector& x) { return m*x; })
  // .def(py::self *= Interval())
  .def("__imult__", [](IntervalMatrix &o, const Interval& itv) { o *= itv; })
  // todo: .def(py::self * other<Matrix>())
  // todo: .def(other<Matrix>() * self)
  .def("__mul__", [](IntervalMatrix& m, const IntervalVector& x) { return m*x; })
  .def(double() * py::self)
  // .def(Interval() * self)
  .def( "__mul__", [](IntervalMatrix& m, const Interval& x) { return x*m; })
  .def("shape", [] (IntervalMatrix& o) { return make_tuple(o.nb_rows(), o.nb_cols()); })
  .def("__repr__", [](const IntervalMatrix& x) { ostringstream str; str << x; return str.str(); })

  .def("__or__", [](const IntervalMatrix& x, const IntervalMatrix& y) { return x|y; })
  .def("__and__", [](const IntervalMatrix& x, const IntervalMatrix& y) { return x&y; })

  .def("inflate", &IntervalMatrix::inflate, "r"_a)
  .def("is_unbounded", &IntervalMatrix::is_unbounded)
  .def("is_subset", &IntervalMatrix::is_subset, "x"_a)
  .def("is_strict_subset", &IntervalMatrix::is_strict_subset, "x"_a)
  .def("is_interior_subset", &IntervalMatrix::is_interior_subset, "x"_a)
  .def("is_strict_interior_subset", &IntervalMatrix::is_strict_interior_subset, "x"_a)
  .def("is_superset", &IntervalMatrix::is_superset, "x"_a)
  .def("is_strict_superset", &IntervalMatrix::is_strict_superset, "x"_a)
  .def("contains", &IntervalMatrix::contains, "x"_a)
  .def("__contains__", &IntervalMatrix::contains, "x"_a)
  .def("interior_contains", &IntervalMatrix::interior_contains, "x"_a)
  .def("intersects", &IntervalMatrix::intersects, "x"_a)
  .def("overlaps", &IntervalMatrix::overlaps, "x"_a)
  .def("is_disjoint", &IntervalMatrix::is_disjoint,"x"_a)
  .def("is_zero", &IntervalMatrix::is_zero)
  ;
};