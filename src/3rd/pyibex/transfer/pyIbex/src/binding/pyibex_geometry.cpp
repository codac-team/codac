//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================

#include "ibex_Sep.h"
#include "ibex_Interval.h"

#include "pyibex_Catan2.h"
#include "pyibex_CtcPolar.h"
#include "pyibex_SepPolarXY.h"

#include "pyibex_SepPolygon.h"
#include "pyibex_CtcSegment.h"
#include "pyibex_SepDiskExists.h"
#include "pyibex_doc_geometry.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace pybind11::literals;
namespace py = pybind11;

using ibex::Interval;
using ibex::IntervalVector;
// using namespace pyibex;

pyibex::SepPolygon* SepPolygonFromList(std::vector< std::array<double, 2> >& lst){
  int n = lst.size();
  std::vector<double> ax(n), ay(n),bx(n),by(n);
  for(size_t i = 0; i < n; i++){
      ax[i] = lst[i][0];
      ay[i] = lst[i][1];
      bx[i] = lst[ (i+1)%n ][0];
      by[i] = lst[ (i+1)%n ][1];
  }
  return new pyibex::SepPolygon(ax, ay, bx, by);
}


PYBIND11_MODULE(geometry, m)
{
  m.doc() = R"pbdoc(
       pyIbex geometry module
       -----------------------
       .. currentmodule:: geometry
       .. autosummary::
          :toctree: _generate
          CtcPolar
          CtcPolarXY
          SepPolarXY
          CtcSegment
          SepPolygon
          SepDiskExists
          Catan2
          Cmod
          Cmod_bwd

   )pbdoc";

  py::object ctc = (py::object) py::module::import("pyibex").attr("Ctc");
  py::object sep = (py::object) py::module::import("pyibex").attr("Sep");

  py::class_<pyibex::CtcPolar>(m, "CtcPolar", ctc, DOC_CTCPOLAR_TYPE)
      .def(py::init<>())
      .def("contract", ( void (pyibex::CtcPolar::*) (IntervalVector&) ) &pyibex::CtcPolar::contract, DOC_CTCPOLAR_CONTRACT1)
      .def("contract", ( void (pyibex::CtcPolar::*) (Interval&, Interval&, Interval&, Interval&) ) &pyibex::CtcPolar::contract, DOC_CTCPOLAR_CONTRACT2)
      .def("RTfromXY", &pyibex::CtcPolar::RTfromXY)
  ;
  // Export CtcPolarXY
  py::class_<pyibex::CtcPolarXY>(m, "CtcPolarXY", ctc)
      .def(py::init<const Interval &, const Interval& >())
      .def("contract", ( void (pyibex::CtcPolarXY::*) (IntervalVector&) ) &pyibex::CtcPolarXY::contract)
  ;
  // Export CtcPolarXY_landmark
  py::class_<pyibex::CtcPolarXY_landmark>(m, "CtcPolarXY_landmark", ctc)
      .def(py::init<const Interval&, const Interval&, const Interval&, const Interval& >())
      .def("contract", ( void (pyibex::CtcPolarXY_landmark::*) (IntervalVector&) ) &pyibex::CtcPolarXY_landmark::contract)
  ;

  // Export CtcPolarXY_landmark
  py::class_<pyibex::CtcPolarXYT_landmark>(m, "CtcPolarXYT_landmark", ctc)
      .def(py::init<const Interval&, const Interval&, const Interval&, const Interval& >())
      .def("contract", ( void (pyibex::CtcPolarXYT_landmark::*) (IntervalVector&) ) &pyibex::CtcPolarXYT_landmark::contract)
  ;

  // // Export SepPolarXY
  py::class_<pyibex::SepPolarXY>(m, "SepPolarXY", sep)
      .def(py::init<Interval, Interval>())
      .def("separate", &pyibex::SepPolarXY::separate)
  ;

  // // Export CtcSegment
  py::class_<pyibex::CtcSegment>(m, "CtcSegment", ctc)
      .def(py::init<double, double,double,double>())
      .def("contract", &pyibex::CtcSegment::contract)
  ;
  //
  // // Export SepPolygon
  py::class_<pyibex::SepPolygon>(m, "SepPolygon", sep)
    .def(py::init(&SepPolygonFromList), "list"_a)
    // .def(py::init<std::vector< std::vector< std::vector<double> > >& >())
    .def(py::init<std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&>())
    .def("separate", (void (ibex::Sep::*) (IntervalVector&, IntervalVector&)) &pyibex::SepPolygon::separate)
  ;

  // Export SepDiskExists
  py::class_<pyibex::SepDiskExists>(m, "SepDiskExists", sep)
    .def(py::init<Interval, Interval, Interval >())
    .def("separate", &pyibex::SepDiskExists::separate)
  ;


  m.def("Catan2", &pyibex::Catan2);
  m.def("Cmod", &pyibex::Cmod);
  m.def("Cmod_bwd", &pyibex::Cmod_bwd);
  // return m.ptr();
}
