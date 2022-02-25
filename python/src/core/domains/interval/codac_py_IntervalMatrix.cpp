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

#include "codac_Interval.h"
#include "codac_IntervalMatrix.h"
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

IntervalVector& get_item(IntervalMatrix& x, int i)
{
  return x[i];
}

void set_item(IntervalMatrix& x, int i, IntervalVector& xi)
{
  x[i] = xi;
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
  .def("__get_item__", get_item, py::return_value_policy::reference_internal)
  .def("__set_item__", set_item)
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
  ;
};