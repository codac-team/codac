//============================================================================
//                               P Y I B E X
// File        : pyIbex_TestExploredArea.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Sep 19, 2015
//============================================================================

#include "sweepTest.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

using py::init;
using py::keep_alive;


// #include <pyIbex_type_caster.h>

// #include "thickPaving.h"
#include "sweepTest.h"

using ibex::SweepTest;

std::tuple<Tube, Tube> genV1All_wrapper(SweepTest& t, IntervalVector& X){
  Tube v1_plus(10, Interval(0)),v1_moins(10, Interval(0));
  t.genV1(X, v1_plus, v1_moins, -1, -1); //,
  return make_tuple(v1_plus, v1_moins);
}


std::tuple<Tube, Tube> genV2All_wrapper(SweepTest& t, IntervalVector& X){
  Tube vp(10, Interval(0)),vm(10, Interval(0));
  t.genV2(X, vp, vm, -1, -1); //,
  return make_tuple(vp, vm);
}

PYBIND11_MODULE(sweeptest, m)
{
  // py::module m("sweeptest", "python binding CtcIMage Class");

  py::object thickTest = (py::object) py::module::import("pyibex_thickset").attr("ThickTest");
  // py::object ctc = (py::object) py::module::import("pyibex").attr("Ctc");
  py::class_<ibex::SweepTest>(m, "SweepTest", thickTest)
      .def(init<const Tube&, const Tube&, const Tube&, const Tube&, double>(),
          keep_alive<1,2>() , keep_alive<1,3>() , keep_alive<1,4>() , keep_alive<1,5>() )
      .def(init<const Tube&, const Tube&, const Tube&, const Tube&, const Tube&>(),
          keep_alive<1,2>() , keep_alive<1,3>() , keep_alive<1,4>() , keep_alive<1,5>(), keep_alive<1,6>() )
      .def( "test", &SweepTest::test)
      .def_property_readonly("x", [](SweepTest& o) { return o.x;})
      .def_property_readonly("y", [](SweepTest& o) { return o.y;})
      .def_property_readonly("ux", [](SweepTest& o) { return o.ux;})
      .def_property_readonly("uy", [](SweepTest& o) { return o.uy;})
      .def ("genV1", ( Tube (SweepTest::*) (IntervalVector &, int, int) ) &SweepTest::genV1)
      .def ("genV2", ( Tube (SweepTest::*) (IntervalVector &, int, int) ) &SweepTest::genV2)
      .def ("genV1All", &genV1All_wrapper)
      .def ("genV2All", &genV2All_wrapper)
      .def ("genFunction", &SweepTest::genFunction )
      .def( "getBounds" , &SweepTest::getBounds)
  ;
    // return m.ptr();
}
