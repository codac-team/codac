//============================================================================
//                               P Y I B E X
// File        : pyibex_ThickInterval.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Aug  31, 2016
//============================================================================

// pyIbex_ThickInterval.cpp


#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using py::self;
using py::init;
using py::keep_alive;
using py::class_;


#include "codac_ThickInterval.h"
#include "codac_ThickBox.h"
#include "codac_ThickFunction.h"
#include "codac_ThickPaving.h"
using ibex::Interval;

const ThickInterval EMPTY_THICK(){
    const static ThickInterval *empty = new ThickInterval(Interval::EMPTY_SET, Interval::EMPTY_SET);
    return *empty;
  };

// const ThickInterval EMPTY_THICK = ThickInterval::EMPTY_THICK();
// const ThickInterval ThickInterval::EMPTY_THICK = {Interval(1, -1), Interval(1, -1)} ;
// const ThickInterval EMPTY_THICK(Interval::EMPTY_SET, Interval::EMPTY_SET);

ThickInterval* ThickIntervalFromList(std::vector<double> &lb, std::vector<double> &ub){
  if(lb.size() != 2 || ub.size() != 2){
    throw std::invalid_argument("Format is ThickInterval([lb, lb], [ub, ub])");
  }
  return new ThickInterval(Interval(lb[0], lb[1]), Interval(ub[0], ub[1]));
}

ThickInterval get_empty(){
  return ThickInterval(Interval::EMPTY_SET, Interval::EMPTY_SET);
}

void export_thickInterval(py::module& m)
{
  // py::object itv = (py::object) py::module::import("pyIbex").attr("Interval");
  // py::module m("thickInterval", "python binding ThickInterval Class");



  class_<ThickInterval>(m, "ThickInterval")
    .def(init<const Interval&, const Interval&>())
    .def(init<const double&, const double&>())
    .def(init<const Interval&>())
    .def(init(&ThickIntervalFromList))
    .def("intersects"  , &ThickInterval::intersects)
    .def("is_disjoint"  , &ThickInterval::is_disjoint )
    .def("is_not_subset" , &ThickInterval::is_not_subset  )
    .def("is_subset"  , &ThickInterval::is_subset )

    .def("superset", &ThickInterval::superset)
    .def("subset", &ThickInterval::subset)
    .def("is_interval", &ThickInterval::is_interval)
    .def("is_degenerated", &ThickInterval::is_degenerated)
    .def("is_empty", &ThickInterval::is_empty)
    .def("lb", &ThickInterval::lb)
    .def("ub", &ThickInterval::ub)
    .def("is_inside", &ThickInterval::is_inside)


    .def(py::self == py::self)

    .def("__and__",  &ThickInterval::inter)
    .def("inter",  &ThickInterval::inter)
    .def("__repr__", &ThickInterval::to_string)

    .def("EMPTY_THICK", &EMPTY_THICK)
    ;
    py::implicitly_convertible<Interval, ThickInterval>();



    class_<ThickBox>(m, "ThickBox")
      .def(init<int, ThickInterval>())
      .def(init<std::vector<ThickInterval>& >())
      .def(init<IntervalVector&, IntervalVector&>())
      .def("__getitem__", [](ThickBox& self, size_t idx){
                            if (idx > self.size())   throw py::index_error();
                            return self[idx];
                          })
      .def("__setitem__", [](ThickBox& self, size_t idx, ThickInterval& itv){
                            if (idx > self.size())   throw py::index_error();
                            self[idx]=itv;
                          })
      .def("superset", &ThickBox::superset)
      .def("to_string", &ThickBox::to_string)
      .def("__repr__", &ThickBox::to_string)
      .def("inner_box", &ThickBox::inner_box)
      .def("inter", &ThickBox::inter)
      .def("__and__", &ThickBox::inter)
      .def("intersects", &ThickBox::intersects)
      // .def("is_disjoint", &ThickBox::is_disjoint)
      .def("is_not_subset", &ThickBox::is_not_subset)
      .def("is_subset",  (bool (ThickBox::*) (const IntervalVector&) const) &ThickBox::is_subset)
      .def("is_subset",  (bool (ThickBox::*) (const ThickBox&) const) &ThickBox::is_subset)
      .def("is_inside", &ThickBox::is_inside)
      .def( py::self == py::self )
      .def_property_readonly("size" , [](ThickBox& self) {return self.size();})
    ;

    class_<ThickFunction>(m, "ThickFunction")
      .def(init<RawFunc&,RawFunc&>(), py::keep_alive<1,2>(), py::keep_alive<1,3>())
      .def("eval", &ThickFunction::eval)
    ;

    // return m.ptr();
}
