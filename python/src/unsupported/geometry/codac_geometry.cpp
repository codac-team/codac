//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================

#include "ibex_Sep.h"
#include "codac_Interval.h"

#include "codac_CtcPolar.h"
#include "codac_SepPolarXY.h"
#include "codac_SepPolygon.h"
#include "codac_CtcSegment.h"
#include "codac_SepDiskExists.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../../core/contractors/static/codac_py_Ctc.h"
#include "../../core/separators/codac_py_Sep.h"
using namespace pybind11::literals;
namespace py = pybind11;

using namespace ibex;
using namespace codac;

SepPolygon* SepPolygonFromList(std::vector< std::array<double, 2> >& lst){
  int n = lst.size();
  std::vector<double> ax(n), ay(n),bx(n),by(n);
  for(size_t i = 0; i < n; i++){
      ax[i] = lst[i][0];
      ay[i] = lst[i][1];
      bx[i] = lst[ (i+1)%n ][0];
      by[i] = lst[ (i+1)%n ][1];
  }
  return new SepPolygon(ax, ay, bx, by);
}

void export_unsupported_geometry(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<Sep, pySep>& sep)
{

  /*py::class_<CtcPolar>(m, "CtcPolar", ctc, DOC_CTCPOLAR_TYPE)
      .def(py::init<>())
      .def("contract", ( void (CtcPolar::*) (IntervalVector&) ) &CtcPolar::contract, DOC_CTCPOLAR_CONTRACT1)
      .def("contract", ( void (CtcPolar::*) (Interval&, Interval&, Interval&, Interval&) ) &CtcPolar::contract, DOC_CTCPOLAR_CONTRACT2)
      //.def("RTfromXY", &CtcPolar::RTfromXY)
  ;*/
  /*// Export CtcPolarXY
  py::class_<CtcPolarXY>(m, "CtcPolarXY", ctc)
      .def(py::init<const Interval &, const Interval& >())
      .def("contract", ( void (CtcPolarXY::*) (IntervalVector&) ) &CtcPolarXY::contract)
  ;
  // Export CtcPolarXY_landmark
  py::class_<CtcPolarXY_landmark>(m, "CtcPolarXY_landmark", ctc)
      .def(py::init<const Interval&, const Interval&, const Interval&, const Interval& >())
      .def("contract", ( void (CtcPolarXY_landmark::*) (IntervalVector&) ) &CtcPolarXY_landmark::contract)
  ;

  // Export CtcPolarXY_landmark
  py::class_<CtcPolarXYT_landmark>(m, "CtcPolarXYT_landmark", ctc)
      .def(py::init<const Interval&, const Interval&, const Interval&, const Interval& >())
      .def("contract", ( void (CtcPolarXYT_landmark::*) (IntervalVector&) ) &CtcPolarXYT_landmark::contract)
  ;*/

  // // Export SepPolarXY
  // py::class_<SepPolarXY>(m, "SepPolarXY", sep)
  //     .def(py::init<Interval, Interval>())
  //     .def("separate", &SepPolarXY::separate)
  // ;


  // // Export CtcSegment
  // py::class_<CtcSegment>(m, "CtcSegment", ctc)
  //     .def(py::init<double, double,double,double>())
  //     .def("contract", &CtcSegment::contract)
  // ;
  
  // // Export SepPolygon
  // py::class_<SepPolygon>(m, "SepPolygon", sep)
  //   .def(py::init(&SepPolygonFromList), "list"_a)
  //   // .def(py::init<std::vector< std::vector< std::vector<double> > >& >())
  //   .def(py::init<std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&>())
  //   .def("separate", (void (ibex::Sep::*) (IntervalVector&, IntervalVector&)) &SepPolygon::separate)
  // ;

  // Export SepDiskExists
  py::class_<SepDiskExists>(m, "SepDiskExists", sep)
    .def(py::init<Interval, Interval, Interval >())
    .def("separate", &SepDiskExists::separate)
  ;

}
