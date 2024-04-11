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

#include "codac_SepPolygon.h"
#include "codac_CtcSegment.h"
#include "codac_SepPolarXY.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../contractors/static/codac_py_Ctc.h"
#include "../separators/codac_py_Sep.h"
#include "codac_Polygon.h"

// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_CtcSegment_docs.h"
#include "codac_py_SepPolygon_docs.h"

using namespace pybind11::literals;
namespace py = pybind11;

using namespace ibex;
using namespace codac;

Polygon* create_polygon_from_pylist(const vector<py::list>& lst)
{
  vector<Vector> v_pts;

  if(lst.size() < 1)
    throw invalid_argument("size of the input list is 0");
  
  //double (*tmp)[2] = new double[lst.size()][2];
  for(size_t i = 0; i < lst.size(); i++)
  {
    if(lst[i].size() != 2)
    {
      //delete[] tmp;
      throw invalid_argument("sub list must contain only two elements");
    }

    //tmp[i][0] = lst[i][0].cast<double>();
    //tmp[i][1] = lst[i][1].cast<double>();
    v_pts.push_back({ lst[i][0].cast<double>(), lst[i][1].cast<double>() });
  }

  //IntervalVector *instance = new IntervalVector(lst.size(), tmp);
  //delete[] tmp;
  //return instance;
  return new Polygon(v_pts);
  // todo: manage delete
}

SepPolygon* SepPolygonFromList(std::vector< std::array<double, 2> >& lst){
  size_t n = lst.size();
  std::vector<double> ax(n), ay(n),bx(n),by(n);
  for(size_t i = 0; i < n; i++){
      ax[i] = lst[i][0];
      ay[i] = lst[i][1];
      bx[i] = lst[ (i+1)%n ][0];
      by[i] = lst[ (i+1)%n ][1];
  }
  return new SepPolygon(ax, ay, bx, by);
}

void export_geometry(py::module& m, py::class_<Ctc, pyCtc>& ctc, py::class_<Sep, pySep>& sep)
{

  // Export CtcSegment
  py::class_<CtcSegment>(m, "CtcSegment", ctc)
      .def(py::init<double, double,double,double>(), 
        CTCSEGMENT_CTCSEGMENT_DOUBLE_DOUBLE_DOUBLE_DOUBLE) 
      .def("contract", &CtcSegment::contract, CTCSEGMENT_VOID_CONTRACT_INTERVALVECTOR)
  ;
  
  // Export SepPolygon
  py::class_<SepPolygon>(m, "SepPolygon", sep)
    .def(py::init(&SepPolygonFromList), "list"_a)
    // .def(py::init<std::vector< std::vector< std::vector<double> > >& >())
    .def(py::init<std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&>(),
        SEPPOLYGON_SEPPOLYGON_VECTORDOUBLE_VECTORDOUBLE_VECTORDOUBLE_VECTORDOUBLE
      )
    .def("separate", (void (ibex::Sep::*) (IntervalVector&, IntervalVector&)) &SepPolygon::separate)
  ;

  // Export SepPolarXY
  py::class_<SepPolarXY>(m, "SepPolarXY", sep)
      .def(py::init<Interval, Interval>())
      .def("separate", &SepPolarXY::separate)
  ;

  // Export Polygon
  py::class_<Polygon>(m, "Polygon")
    .def(py::init(&create_polygon_from_pylist), "list"_a)
    .def(py::init<std::vector<Vector>>())
  ;

}
