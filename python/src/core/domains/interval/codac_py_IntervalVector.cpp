/** 
 *  \file
 *  IntervalVector Python binding
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
#include "codac_IntervalVector.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_IntervalVector_docs.h" // todo: generate this file from Doxygen doc

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;

IntervalVector* create_from_pylist(const vector<py::list>& lst)
{
  if(lst.size() < 1)
    throw invalid_argument("size of the input list is 0");
  
  double (*tmp)[2] = new double[lst.size()][2];
  for(size_t i = 0; i < lst.size(); i++)
  {
    if(lst[i].size() != 2)
    {
      delete tmp;
      throw invalid_argument("sub list must contain only two elements");
    }

    tmp[i][0] = lst[i][0].cast<double>();
    tmp[i][1] = lst[i][1].cast<double>();
  }

  IntervalVector *instance = new IntervalVector(lst.size(), tmp);
  delete[] tmp;
  return instance;
  // todo: manage delete
}

IntervalVector* create_from_vector_of_intervals(const vector<Interval>& lst)
{
  IntervalVector *instance = new IntervalVector(lst.size());

  for(size_t i = 0; i < lst.size(); i++)
    (*instance)[i] = lst[i];
  
  return instance;
  // todo: manage delete
}

IntervalVector* create_from_int_list(int ndim, vector<double>& v)
{
  if(v.size() != 2)
    throw invalid_argument("syntax is IntervalVector(2, [1,2])");
  return new IntervalVector(ndim, Interval(v[0], v[1]));
  // todo: manage delete
}

// void create_from_tuple(IntervalVector &instance, vector<double>& tub)
//{
//  new(&instance) IntervalVector(lst.size(), Interval(v[0], v[1]));
//}

vector<IntervalVector> complementary_wrapper(IntervalVector& x)
{
  IntervalVector* result;
  int n = x.complementary(result);
  vector<IntervalVector> v;
  v.assign(result, result+n);
  return v;
}

vector<IntervalVector> diff_wrapper(IntervalVector& x, IntervalVector& y, bool compactness)
{
  IntervalVector* result;
  int n = x.diff(y, result, compactness);
  vector<IntervalVector> v;
  v.assign(result, result+n);
  return v;
}

vector<int> sort_indices_wrapper(IntervalVector& x, bool min)
{
  vector<int> v(x.size());
  x.sort_indices(min, &v[0]);
  return v;
}

void assign_IntervalVector(IntervalVector& x, const IntervalVector& y)
{
  x = y;
}

IntervalVector IntervalVector_copy(IntervalVector& x)
{
  return IntervalVector(x);
}

IntervalVector max_IntevalVector(const IntervalVector& a, const IntervalVector& b)
{
  assert(a.size() == b.size());
  IntervalVector res(a.size());
  for(int i = 0; i < res.size(); i++)
    res[i] = max(a[i], b[i]);
  return res;
}

vector<double> to_list(const IntervalVector& x)
{
  vector<double> res(2*x.size());
  for(int i = 0; i < x.size(); i++)
  {
    res[2*i] = x[i].lb();
    res[2*i+1] = x[i].ub();
  }
  return res;
}

void export_IntervalVector(py::module& m)
{
  py::class_<IntervalVector> interval_vector(m, "IntervalVector", DOCS_INTERVALVECTOR_TYPE);
  interval_vector

  // Definition

    .def(py::init<int>(), "dim"_a)
    .def(py::init<int,const Interval>(), "dim"_a, "itv"_a)
    .def(py::init<const IntervalVector&>(), "x"_a)
    .def(py::init<const Vector&>(), "x"_a)
    .def(py::init<const Interval&>(), "x"_a)
    .def(py::init(&create_from_pylist), "list"_a)
    .def(py::init(&create_from_int_list), "dim"_a, "list"_a)
    .def(py::init(&create_from_vector_of_intervals), "list"_a)
    //.def("__init__", &create_from_tuple, "list"_a)

    .def(py::init([](py::array_t<double> b) // create for instance an IntervalVector from a NumPy array
    {
      // Request a buffer descriptor from Python
      py::buffer_info info = b.request();

      // Some sanity checks...
      if(info.format != py::format_descriptor<double>::format())
        throw std::runtime_error("Incompatible format: expected a double array");

      if(info.ndim == 1 ||                            // e.g.: a=np.array([1,0,0])
         (info.ndim == 2 && (int)info.shape[1] == 1)) // e.g.: a=np.array([[1],[0],[0]]) 
      {
        ibex::Vector m((int)info.shape[0], static_cast<double*>(info.ptr));
        return IntervalVector(m);
      }

      else
        throw std::runtime_error("Incompatible buffer dimension");

      return IntervalVector(0);
    }))

  // Python vector methods

    .def("__len__", &IntervalVector::size)

    .def("__getitem__", [](IntervalVector& s, size_t index) -> Interval&
      {
        if(index < 0 || index >= static_cast<size_t>(s.size()))
          throw py::index_error();
        return s[static_cast<int>(index)];
      },
      py::return_value_policy::reference_internal)

    .def("__setitem__", [](IntervalVector& s, size_t index, Interval& t)
      {
        if(index < 0 || index >= static_cast<size_t>(s.size()))
          throw py::index_error();
        s[static_cast<int>(index)] = t;
      })
    
    .def("__iter__", 
      [](const IntervalVector& s)
      {
        return py::make_iterator(&s[0], &s[0]+s.size());
      },
      py::keep_alive<0,1>()) // keep object alive while iterator exists
    
    //.def("__contains__", [](const IntervalVector &s, float v) { return s.contains(v); })
    //.def("__reversed__", [](const IntervalVector &s) -> IntervalVector { return s.reversed(); })

    .def(py::pickle(
      [](const IntervalVector &p) {
        py::list l;
        for(int i = 0; i < p.size(); i++)
          l.append(p[i]);
        return l;
      },
      [](py::list t) {
        if(t.size() == 0) {
          cerr << t.size();
          throw runtime_error("Invalid state");
        }
        auto p = create_from_vector_of_intervals(t.cast<vector<Interval>>());
        return p;
      }
    ))

    .def("to_list", &to_list, DOCS_INTERVALVECTOR_TOLIST)
    .def("assign", &assign_IntervalVector)

  // Arithmetic

    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self + py::self)
    .def(py::self - py::self)
    .def(py::self * py::self)
    .def(py::self & py::self)
    .def(py::self | py::self)
    .def(-py::self)

    .def(py::self += py::self)
    .def(py::self += double())
    .def(py::self -= py::self)
    .def(py::self -= double())
    .def(py::self *= double())
    .def("__imul__", [](IntervalVector& a, const Interval& x) { return a*=x; })
    .def("__mul__",  [](IntervalVector& a, const Vector& x) { return a*x; })
    .def("__rmul__", [](IntervalVector& a, const Vector& x) { return x*a; })
    .def("__rmul__", [](IntervalVector& a, const Interval& x) { return x*a; })

    .def(py::self &= py::self)
    .def(py::self |= py::self)

    .def("__add__",  [](IntervalVector& a, const Vector& x) { return a+x; })
    .def("__iadd__", [](IntervalVector& a, const Vector& x) { return a+x; })
    .def("__radd__", [](IntervalVector& a, const Vector& x) { return a+x; })

    .def("__sub__",  [](IntervalVector& a, const Vector& x) { return a-x; })
    .def("__isub__", [](IntervalVector& a, const Vector& x) { return a-=x; })
    .def("__rsub__", [](IntervalVector& a, const Vector& x) { return x-a; })
    .def(double() * py::self)

    .def("__repr__", [](const IntervalVector& x) { ostringstream str; str << x; return str.str(); })
    .def("copy", &IntervalVector_copy)
    .def("size", &IntervalVector::size)
    .def_static("empty", &IntervalVector::empty, DOCS_INTERVALVECTOR_EMPTY, py::arg("n"))
    .def("set_empty", &IntervalVector::set_empty, DOCS_INTERVALVECTOR_SET_EMPTY)
    .def("clear", &IntervalVector::clear, DOCS_INTERVALVECTOR_CLEAR)
    .def("init", &IntervalVector::init, DOCS_INTERVALVECTOR_INIT, py::arg("x"))
    .def("inflate", (IntervalVector& (IntervalVector::*) (double)) &IntervalVector::inflate, DOCS_INTERVALVECTOR_INFLATE, "rad"_a)
    .def("inflate", (IntervalVector& (IntervalVector::*) (double, double)) &IntervalVector::inflate, DOCS_INTERVALVECTOR_INFLATE, "rad"_a, "chi"_a)
    //.def("inflate", &IntervalVector::inflate, DOCS_INTERVALVECTOR_INFLATE, py::return_value_policy::reference_internal, py::arg("rad"))
    .def("resize", &IntervalVector::resize, DOCS_INTERVALVECTOR_RESIZE, py::arg("n"))
    .def("subvector", &IntervalVector::subvector, DOCS_INTERVALVECTOR_SUBVECTOR, "start_index"_a, "end_index"_a) //, return_value_policy<return_by_value>())
    .def("put", &IntervalVector::put, DOCS_INTERVALVECTOR_PUT)
    .def("size", &IntervalVector::size, DOCS_INTERVALVECTOR_SIZE)
    .def("lb", &IntervalVector::lb, DOCS_INTERVALVECTOR_LB)
    .def("ub", &IntervalVector::ub, DOCS_INTERVALVECTOR_UB)
    .def("mid", &IntervalVector::mid, DOCS_INTERVALVECTOR_MID)
    .def("mig", &IntervalVector::mig, DOCS_INTERVALVECTOR_MIG)
    .def("mag", &IntervalVector::mag, DOCS_INTERVALVECTOR_MAG)

  // Tests

    .def("is_empty", &IntervalVector::is_empty, DOCS_INTERVALVECTOR_IS_EMPTY)
    .def("is_flat", &IntervalVector::is_flat, DOCS_INTERVALVECTOR_IS_FLAT)
    .def("is_unbounded", &IntervalVector::is_unbounded, DOCS_INTERVALVECTOR_IS_UNBOUNDED)
    .def("is_subset", &IntervalVector::is_subset, DOCS_INTERVALVECTOR_IS_SUBSET, "x"_a)
    .def("is_strict_subset", &IntervalVector::is_strict_subset, DOCS_INTERVALVECTOR_IS_STRICT_SUBSET, "x"_a)
    .def("is_interior_subset", &IntervalVector::is_interior_subset, DOCS_INTERVALVECTOR_IS_INTERIOR_SUBSET, "x"_a)
    .def("is_strict_interior_subset", &IntervalVector::is_strict_interior_subset, DOCS_INTERVALVECTOR_IS_STRICT_INTERIOR_SUBSET, "x"_a)
    .def("is_superset", &IntervalVector::is_superset, DOCS_INTERVALVECTOR_IS_SUPERSET, "x"_a)
    .def("is_strict_superset", &IntervalVector::is_strict_superset, DOCS_INTERVALVECTOR_IS_STRICT_SUPERSET, "x"_a)
    .def("contains", &IntervalVector::contains, DOCS_INTERVALVECTOR_CONTAINS, "x"_a)
    .def("__contains__", &IntervalVector::contains, DOCS_INTERVALVECTOR_CONTAINS, "x"_a)
    .def("interior_contains", &IntervalVector::interior_contains, DOCS_INTERVALVECTOR_INTERIOR_CONTAINS, "x"_a)
    .def("intersects", &IntervalVector::intersects, DOCS_INTERVALVECTOR_INTERSECTS, "x"_a)
    .def("overlaps", &IntervalVector::overlaps, DOCS_INTERVALVECTOR_OVERLAPS, "x"_a)
    .def("is_disjoint", &IntervalVector::is_disjoint, DOCS_INTERVALVECTOR_IS_DISJOINT,"x"_a)
    .def("is_zero", &IntervalVector::is_zero, DOCS_INTERVALVECTOR_IS_ZERO)
    .def("is_bisectable", &IntervalVector::is_bisectable, DOCS_INTERVALVECTOR_IS_BISECTABLE)

    .def("rad", &IntervalVector::rad, DOCS_INTERVALVECTOR_RAD)
    .def("diam", &IntervalVector::diam, DOCS_INTERVALVECTOR_DIAM)
    .def("extr_diam_index", &IntervalVector::extr_diam_index, DOCS_INTERVALVECTOR_EXTR_DIAM_INDEX, "min"_a)
    .def("sort_indices", sort_indices_wrapper, DOCS_INTERVALVECTOR_SORT_INDICES, "min"_a)
    .def("max_diam", &IntervalVector::max_diam, DOCS_INTERVALVECTOR_MAX_DIAM)
    .def("min_diam", &IntervalVector::min_diam, DOCS_INTERVALVECTOR_MIN_DIAM)
    .def("volume", &IntervalVector::volume, DOCS_INTERVALVECTOR_VOLUME)
    .def("perimeter", &IntervalVector::perimeter, DOCS_INTERVALVECTOR_PERIMETER)
    .def("rel_distance", &IntervalVector::rel_distance, DOCS_INTERVALVECTOR_REL_DISTANCE, "x"_a)
    .def("width", [](IntervalVector& x) {
      Vector diam = x.diam();
      double width=0;
      for(int i = 0; i < diam.size(); i++)
        width += pow(diam[i],2);
      return sqrt(width);
    })
    .def("diff", &diff_wrapper, DOCS_INTERVALVECTOR_DIFF, "y"_a, "compactness"_a=true)
    .def("complementary", &complementary_wrapper, DOCS_INTERVALVECTOR_COMPLEMENTARY)
    .def("bisect", &IntervalVector::bisect, DOCS_INTERVALVECTOR_BISECT, py::arg("i"), py::arg("ratio")=0.5)
    ;

  // External functions

  m.def("abs", (IntervalVector (*) (const IntervalVector&)) &ibex::abs);
  m.def("bwd_add", (bool (*) (const IntervalVector&, IntervalVector&, IntervalVector&)) &ibex::bwd_add);
  m.def("bwd_sub", (bool (*) (const IntervalVector&, IntervalVector&, IntervalVector&)) &ibex::bwd_sub);
  m.def("cart_prod", (IntervalVector (*) (const IntervalVector&, const IntervalVector&)) &ibex::cart_prod);

  // Hadamard_product overloading
  //IntervalVector (*hadamard_product_1) (const IntervalVector&, const Vector&) = &ibex::hadamard_product;
  //IntervalVector (*hadamard_product_2) (const Vector&, const IntervalVector&) = &ibex::hadamard_product;
  //IntervalVector (*hadamard_product_3) (const IntervalVector&, const IntervalVector&) = &ibex::hadamard_product;
  m.def("hadamard_product", (IntervalVector (*) (const IntervalVector&, const Vector&)) &ibex::hadamard_product);
  m.def("hadamard_product", (IntervalVector (*) (const Vector&, const IntervalVector&)) &ibex::hadamard_product);
  m.def("hadamard_product", (IntervalVector (*) (const IntervalVector&, const IntervalVector&)) &ibex::hadamard_product);

  // bwd_mul overloading
  //bool (*bwd_mul_1) (const IntervalVector&, Interval&, IntervalVector&) = &ibex::bwd_mul;
  //bool (*bwd_mul_2) (const Interval&, IntervalVector&, IntervalVector&) = &ibex::bwd_mul;
  m.def("bwd_mul", (bool (*) (const IntervalVector&, Interval&, IntervalVector&)) &ibex::bwd_mul);
  m.def("bwd_mul", (bool (*) (const Interval&, IntervalVector&, IntervalVector&)) &ibex::bwd_mul);

  m.def("max", (IntervalVector(*) (const IntervalVector&, const IntervalVector&)) &max_IntevalVector);
};