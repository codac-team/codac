//============================================================================
//                                  I B E X
// File        : pyIbex.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include <pybind11/pybind11.h>

namespace py = pybind11;

#include <string>
#include <sstream>
#include <ibex_Setting.h>
#include <ibex_BoolInterval.h>

void export_Interval(py::module&m);
void export_IntervalVector(py::module& m);
void export_IntervalMatrix(py::module& m);
void export_Function(py::module& m);
void export_Bsc(py::module& m);
void export_Ctc(py::module& m);
void export_Separators(py::module& m);


PYBIND11_MODULE(pyibex, m)
{
  // py::module m("pyibex", "python binding of ibex core fonctionnality");
  m.doc() = "python binding of ibex core fonctionnality";
  export_Interval(m);
  export_IntervalVector(m);
  export_Function(m);
  export_IntervalMatrix(m);
  export_Bsc(m);
  export_Ctc(m);
  export_Separators(m);
  m.attr("ibex_version") = _IBEX_RELEASE_;

  // Export bool inteval
  py::enum_<ibex::BoolInterval>(m, "BoolInterval")
    .value( "YES", 	ibex::YES)
    .value( "MAYBE", 	ibex::MAYBE)
    .value( "NO", 	ibex::NO)
    .value( "EMPTY_BOOL", 	ibex::EMPTY_BOOL)
    .export_values()
  ;
  // return m.ptr();

}
