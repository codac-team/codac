//============================================================================
//                                P Y I B E X
// File        : pyIbex_IntervalVector.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================


#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pyIbex_type_caster.h>

#include "ibex_IntervalVector.h"
#include "ibex_IntervalMatrix.h"
#include <stdexcept>
#include <sstream>

using namespace pybind11::literals;
#include "pyibex_doc_IntervalVector.h"


namespace py = pybind11;
using py::self;

using ibex::Interval;
using ibex::IntervalVector;
using ibex::IntervalMatrix;
using ibex::Vector;



IntervalVector* CreateWithList(const std::vector< py::list >& lst){
  if (lst.size() < 1){
    throw std::invalid_argument("Size of the input list is 0");
  }
  double (*tmp)[2] = new double[lst.size()][2];
  for (size_t i = 0; i < lst.size(); i++){
    if (lst[i].size() != 2){
      delete tmp;
      throw std::invalid_argument("sub list must contain only two elements");
    }
    tmp[i][0] = lst[i][0].cast<double>();
    tmp[i][1] = lst[i][1].cast<double>();
  }
  IntervalVector * instance = new IntervalVector(lst.size(), tmp);
  delete[] tmp;
  return instance;
}

IntervalVector* CreateWithListOfInterval(const std::vector<Interval>& lst){
  IntervalVector * instance = new IntervalVector(lst.size());

  for (size_t i = 0; i < lst.size(); i++){
    (*instance)[i] = lst[i];
  }
  return instance;
}


IntervalVector* CreateWithIntAndList(int ndim, std::vector<double>& v){
  if (v.size() != 2){
    throw std::invalid_argument("syntax is IntervalVector(2, [1,2])");
  }
  return new IntervalVector(ndim, Interval(v[0], v[1]));
}



// void CreateWithTuple(IntervalVector &instance, vector<double>& tub){
//   new(&instance) IntervalVector(lst.size(), Interval(v[0], v[1]));
// }

Interval& getitem(IntervalVector& X, size_t i){
  if (i >= static_cast<size_t>(X.size()))
      throw py::index_error();
  return X[i];

}

void setitem(IntervalVector& X, size_t i, Interval& itv){
  if (i >= static_cast<size_t>(X.size()))
      throw py::index_error();
  X[i] = itv;
}

std::vector<IntervalVector> complementary_wrapper(IntervalVector& X){
      IntervalVector* result;
      int n = X.complementary(result);
      std::vector<IntervalVector> v;
      v.assign(result, result+n);
      return v;
}

std::vector<IntervalVector> diff_wrapper(IntervalVector& X, IntervalVector& Y, bool compactness){
    IntervalVector* result;
    int n = X.diff(Y, result, compactness);
    std::vector<IntervalVector> v;
    v.assign(result, result+n);
    return v;
}

std::vector<int> sort_indices_wrapper(IntervalVector& X, bool min){
  std::vector<int> v(X.size());
  X.sort_indices(min, &v[0]);
  return v;
}


void assignItv(IntervalVector& self, const IntervalVector& other){
    self = other;
}

IntervalVector my_copy(IntervalVector& itv){
    return IntervalVector(itv);
}

std::string to_string(const IntervalVector& a){
  std::stringstream ss;
  ss << a;
  return ss.str();
}


IntervalVector max_intevalVector (const IntervalVector& a, const IntervalVector& b){
  assert(a.size() == b.size());
  IntervalVector res(a.size());
  for (int i=0; i < res.size(); i++){
    res[i] = max(a[i], b[i]);
  }
  return res;
}

std::vector<double> tolist(const IntervalVector& obj){
  std::vector<double> res(2*obj.size());
  for (int i=0; i < obj.size(); i++){
    res[2*i] = obj[i].lb();
    res[2*i+1] = obj[i].ub();
  }
  return res;
}

void export_IntervalVector(py::module& m){

    py::class_<IntervalVector >(m, "IntervalVector", DOCS_INTERVALVECTOR_TYPE)
            .def(py::init<int>(), "dim"_a)
            .def(py::init<int,const Interval>(), "dim"_a, "itv"_a)
            .def(py::init<const IntervalVector&>(), "x"_a )
            .def(py::init<const Vector&>(), "x"_a)
            // .def(py::init([](const Interval& a) { IntervalVector *instance = new IntervalVector(1); (*instance)[0] = a; return instance;}), "itv"_a)
            .def(py::init<const Interval&>(), "x"_a)
            .def(py::init(&CreateWithList), "list"_a)
            .def(py::init(&CreateWithIntAndList), "dim"_a, "list"_a)
            .def(py::init(&CreateWithListOfInterval), "list"_a)
            // .def("__init__", &CreateWithTuple, "list"_a)

            // Bare bon interface
            .def("__len__", &IntervalVector::size )
            .def("__getitem__", getitem, py::return_value_policy::reference_internal)
            .def("__setitem__", setitem)

             // sequence protocol operations
             .def("__iter__", [](const IntervalVector &s) { return py::make_iterator(&s[0], &s[0]+s.size()); },
                              py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
            //  .def("__contains__", [](const IntervalVector &s, float v) { return s.contains(v); })
            //  .def("__reversed__", [](const IntervalVector &s) -> IntervalVector { return s.reversed(); })

            .def(py::pickle(
                [](const IntervalVector &p) {
                    py::list l;
                    for(int i = 0; i < p.size(); i++){
                      l.append(p[i]);
                    }
                    return l;
                },
                [](py::list t) {
                    if (t.size() == 0){
                      std::cerr << t.size();
                      throw std::runtime_error("Invalid state!");
                    }
                    auto p = CreateWithListOfInterval(t.cast<std::vector<Interval> >());
                    return p;
                }
            ))

            .def("tolist", &tolist, DOCS_INTERVALVECTOR_TOLIST)
            .def("assign", &assignItv)
            .def( self == self )
            .def( self != self )
            .def( self + self )
            .def( self - self )
            .def( self * self)
            .def( self & self )
            .def( self | self )
            .def( -self )

            .def( self += self)
            .def( self += double())

            .def( self -= self)
            .def( self -= double())

            .def( self *= double())
            .def( "__imul__", [](IntervalVector& a, const Interval& x){return a*=x;})
            .def( "__mul__", [](IntervalVector& a, const Vector& x){return a*x;})
            .def( "__rmul__", [](IntervalVector& a, const Vector& x){return x*a;})
            .def( "__rmul__", [](IntervalVector& a, const Interval& x){return x*a;})

            .def( self &= self)
            .def( self |= self)

            .def("__add__", [](IntervalVector& a, const Vector& x){return a+x;})
            .def("__iadd__", [](IntervalVector& a, const Vector& x){return a+x;})
            .def("__radd__", [](IntervalVector& a, const Vector& x){return a+x;})

            .def("__sub__", [](IntervalVector& a, const Vector& x){return a-x;})
            .def("__isub__", [](IntervalVector& a, const Vector& x){return a-=x;})
            .def("__rsub__", [](IntervalVector& a, const Vector& x){return x-a;})
            .def (double() * self)

            .def("__repr__", &to_string)
            .def("copy", &my_copy)
            .def("size", &IntervalVector::size )
            .def_static( "empty", &IntervalVector::empty, DOCS_INTERVALVECTOR_EMPTY, py::arg("n") )
            .def( "set_empty", &IntervalVector::set_empty, DOCS_INTERVALVECTOR_SET_EMPTY)
            .def( "clear", &IntervalVector::clear, DOCS_INTERVALVECTOR_CLEAR)
            .def( "init", &IntervalVector::init, DOCS_INTERVALVECTOR_INIT, py::arg("x"))
            .def( "inflate", (IntervalVector& (IntervalVector::*) (double) ) &IntervalVector::inflate, DOCS_INTERVALVECTOR_INFLATE, "rad"_a)
            .def( "inflate", (IntervalVector& (IntervalVector::*) (double, double) ) &IntervalVector::inflate, DOCS_INTERVALVECTOR_INFLATE, "rad"_a, "chi"_a)
            // .def( "inflate", &IntervalVector::inflate, DOCS_INTERVALVECTOR_INFLATE, py::return_value_policy::reference_internal, py::arg("rad"))
            .def( "resize", &IntervalVector::resize, DOCS_INTERVALVECTOR_RESIZE, py::arg("n"))
            .def( "subvector", &IntervalVector::subvector, DOCS_INTERVALVECTOR_SUBVECTOR, "start_index"_a, "end_index"_a) //, return_value_policy<return_by_value>())
            .def( "put", &IntervalVector::put, DOCS_INTERVALVECTOR_PUT)
            .def( "size" , &IntervalVector::size, DOCS_INTERVALVECTOR_SIZE )
            .def( "lb" , &IntervalVector::lb, DOCS_INTERVALVECTOR_LB )
            .def( "ub" , &IntervalVector::ub, DOCS_INTERVALVECTOR_UB )
            .def( "mid" , &IntervalVector::mid, DOCS_INTERVALVECTOR_MID )
            .def( "mig" , &IntervalVector::mig, DOCS_INTERVALVECTOR_MIG )
            .def( "mag" , &IntervalVector::mag, DOCS_INTERVALVECTOR_MAG )
            .def( "is_empty" , &IntervalVector::is_empty, DOCS_INTERVALVECTOR_IS_EMPTY )
            .def( "is_flat" , &IntervalVector::is_flat, DOCS_INTERVALVECTOR_IS_FLAT )
            .def( "is_unbounded" , &IntervalVector::is_unbounded, DOCS_INTERVALVECTOR_IS_UNBOUNDED )
            .def( "is_subset" , &IntervalVector::is_subset, DOCS_INTERVALVECTOR_IS_SUBSET, "x"_a )
            .def( "is_strict_subset" , &IntervalVector::is_strict_subset, DOCS_INTERVALVECTOR_IS_STRICT_SUBSET, "x"_a )
            .def( "is_interior_subset" , &IntervalVector::is_interior_subset, DOCS_INTERVALVECTOR_IS_INTERIOR_SUBSET, "x"_a )
            .def( "is_strict_interior_subset" , &IntervalVector::is_strict_interior_subset, DOCS_INTERVALVECTOR_IS_STRICT_INTERIOR_SUBSET, "x"_a )
            .def( "is_superset" , &IntervalVector::is_superset, DOCS_INTERVALVECTOR_IS_SUPERSET, "x"_a )
            .def( "is_strict_superset" , &IntervalVector::is_strict_superset, DOCS_INTERVALVECTOR_IS_STRICT_SUPERSET, "x"_a )
            .def( "contains" , &IntervalVector::contains, DOCS_INTERVALVECTOR_CONTAINS, "x"_a)
            .def( "__contains__", &IntervalVector::contains, DOCS_INTERVALVECTOR_CONTAINS, "x"_a)
            .def( "interior_contains" , &IntervalVector::interior_contains, DOCS_INTERVALVECTOR_INTERIOR_CONTAINS, "x"_a )
            .def( "intersects" , &IntervalVector::intersects, DOCS_INTERVALVECTOR_INTERSECTS, "x"_a )
            .def( "overlaps" , &IntervalVector::overlaps, DOCS_INTERVALVECTOR_OVERLAPS, "x"_a )
            .def( "is_disjoint" , &IntervalVector::is_disjoint, DOCS_INTERVALVECTOR_IS_DISJOINT,"x"_a )
            .def( "is_zero" , &IntervalVector::is_zero, DOCS_INTERVALVECTOR_IS_ZERO )
            .def( "is_bisectable" , &IntervalVector::is_bisectable, DOCS_INTERVALVECTOR_IS_BISECTABLE )
            .def( "rad",    &IntervalVector::rad, DOCS_INTERVALVECTOR_RAD )
            .def( "diam",   &IntervalVector::diam, DOCS_INTERVALVECTOR_DIAM )
            .def( "extr_diam_index", &IntervalVector::extr_diam_index, DOCS_INTERVALVECTOR_EXTR_DIAM_INDEX, "min"_a)
            .def( "sort_indices", sort_indices_wrapper, DOCS_INTERVALVECTOR_SORT_INDICES, "min"_a )
            .def( "max_diam",   &IntervalVector::max_diam, DOCS_INTERVALVECTOR_MAX_DIAM)
            .def( "min_diam",   &IntervalVector::min_diam, DOCS_INTERVALVECTOR_MIN_DIAM)
            .def( "volume",     &IntervalVector::volume, DOCS_INTERVALVECTOR_VOLUME )
            .def( "perimeter",  &IntervalVector::perimeter, DOCS_INTERVALVECTOR_PERIMETER )
            .def( "rel_distance",   &IntervalVector::rel_distance, DOCS_INTERVALVECTOR_REL_DISTANCE, "x"_a )
            .def( "width", [](IntervalVector& o){
                Vector diam = o.diam();
                double width=0;
                for(int i = 0; i < diam.size(); i++){
                  width += std::pow(diam[i],2);
                }
                return std::sqrt(width);
            })

            .def( "diff",   &diff_wrapper , DOCS_INTERVALVECTOR_DIFF, "y"_a, "compactness"_a=true)
            .def( "complementary",  &complementary_wrapper , DOCS_INTERVALVECTOR_COMPLEMENTARY)
            .def( "bisect",   &IntervalVector::bisect, DOCS_INTERVALVECTOR_BISECT,  py::arg("i"), py::arg("ratio")=0.5)
            ;

            m.def( "abs", ( IntervalVector (*) (const IntervalVector& ) ) &ibex::abs);
            m.def( "bwd_add", (bool (*) (const IntervalVector&, IntervalVector&, IntervalVector&) ) &ibex::bwd_add);
            m.def( "bwd_sub", (bool (*) (const IntervalVector&, IntervalVector&, IntervalVector&) ) &ibex::bwd_sub);
            m.def( "cart_prod", (IntervalVector (*) (const IntervalVector&, const IntervalVector&)) &ibex::cart_prod);

            // hadamard_product overloading
            // IntervalVector (*hadamard_product_1) (const IntervalVector& , const Vector& ) = &ibex::hadamard_product;
            // IntervalVector (*hadamard_product_2) (const Vector& , const IntervalVector& ) = &ibex::hadamard_product;
            // IntervalVector (*hadamard_product_3) (const IntervalVector& , const IntervalVector& ) = &ibex::hadamard_product;
            m.def( "hadamard_product", (IntervalVector (*) (const IntervalVector& , const Vector& ) ) &ibex::hadamard_product);
            m.def( "hadamard_product", (IntervalVector (*) (const Vector& , const IntervalVector& ) ) &ibex::hadamard_product);
            m.def( "hadamard_product", (IntervalVector (*) (const IntervalVector& , const IntervalVector& ) ) &ibex::hadamard_product);

            // bwd_mul overloading
            // bool (*bwd_mul_1) (const IntervalVector&, Interval&, IntervalVector&) = &ibex::bwd_mul;
            // bool (*bwd_mul_2) (const Interval&, IntervalVector&, IntervalVector&) = &ibex::bwd_mul;
            m.def( "bwd_mul", (bool (*) (const IntervalVector&, Interval&, IntervalVector&)) &ibex::bwd_mul);
            m.def( "bwd_mul", (bool (*) (const Interval&, IntervalVector&, IntervalVector&)) &ibex::bwd_mul);

            m.def("max", ( IntervalVector(*) (const IntervalVector&, const IntervalVector&)) &max_intevalVector);

};
