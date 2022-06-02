//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================


#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <codac_type_caster.h>
namespace py = pybind11;
using py::self;
using py::class_;
using py::init;
using py::keep_alive;
using namespace pybind11::literals;


#include "codac_IntervalVector.h"
#include "ibex_Sep.h"

#include "codac_QInterProjF.h"
#include "codac_SepTransform.h"
#include "codac_SepFixPoint.h"
#include "codac_SepProj.h"
#include "codac_SepCtcPairProj.h"
#include "codac_SepUnionBbox.h"


#include "../../core/separators/codac_py_Sep_docs.h"
#include "../../core/separators/codac_py_Sep.h"

// #include "ibex_SepPolygon.h"
// #include "ibex_SepTransform.h"
// #include "ibex_SepCtcPairProj.h"

// #include "ibex_Set.h"
// #include "ibex_SetInterval.h"
using namespace ibex;
using namespace codac;



void export_unsupported_sep(py::module& m, py::class_<Sep, pySep>& sep){



  class_<SepUnionBbox>(m, "SepUnionBbox", sep)
    .def(init<Array<Sep>, std::vector<IntervalVector> & >(), keep_alive<1,2>(), py::arg("list"), py::arg("bbox"))
    .def("separate", &SepUnionBbox::separate, py::call_guard<py::gil_scoped_release>())
  ;
  // class_<SepQInterProjF>(m, "SepQInterProjF", sep, __DOC_SEP_SEPQINTERPROJF)
  //   .def(init<Array<Sep> >(), keep_alive<1,2>(), py::arg("list"))
  //   .def("separate", &SepQInterProjF::separate)
  //   .def_property("q", py::cpp_function(&SepQInterProjF::get_q), py::cpp_function(&SepQInterProjF::set_q))
  // ;


  // // Export SepProj
  // py::class_<SepProj>(m, "SepProj", sep, __DOC_SEP_SEPPROJ)
  //   .def(py::init<ibex::Sep&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
  //         py::arg("sep"), py::arg("y_init"), py::arg("prec")=1e-3 )
  //   .def(py::init<ibex::Sep&,const Interval&, double>(), py::keep_alive<1,2>(),
  //         py::arg("sep"), py::arg("y_init"), py::arg("prec")=1e-3 )
  //   .def(py::init(
  //       [](ibex::Sep& S,const std::array<double, 2>& itv, double eps){
  //         return std::unique_ptr<SepProj> (
  //           new SepProj(S, Interval(itv[0], itv[1]), eps)
  //         );
  //       }),
  //       py::keep_alive<1,2>(),
  //       py::arg("sep"),
  //       py::arg("y_init"),
  //       py::arg("prec")=1e-3 )

  //   .def("separate", &SepProj::separate, py::call_guard<py::gil_scoped_release>())
  // ;

  // // Export SepCtcPairProj
  // py::class_<SepCtcPairProj>(m, "SepCtcPairProj", sep, __DOC_SEP_SEPCTCPAIRPROJ)
  //   .def(py::init<ibex::Ctc&, ibex::Ctc&,const IntervalVector&, double>(), py::keep_alive<1,2>(), py::keep_alive<1,3>(),
  //         py::arg("ctc_in"), py::arg("ctc_out"), py::arg("y_init"), py::arg("prec"))
  //   .def(py::init<ibex::SepCtcPair&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
  //         py::arg("sep"), py::arg("y_init"), py::arg("prec") )
  //   .def(py::init<ibex::Sep&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
  //         py::arg("sep"), py::arg("y_init"), py::arg("prec") )
  //   .def("separate", &SepCtcPairProj::separate, py::call_guard<py::gil_scoped_release>())
  // ;

  // Export SepTransform
  py::class_<SepTransform>(m, "SepTransform", sep)
    .def(py::init<ibex::Sep&, ibex::Function&, ibex::Function& >(), py::keep_alive<1,2>(), py::keep_alive<1,3>(), py::keep_alive<1,4>())
    .def("separate", &SepTransform::separate, py::call_guard<py::gil_scoped_release>())
  ;

  // // Export SepFixPoint
  // py::class_<SepFixPoint>(m, "SepFixPoint", sep)
  //   .def(py::init<ibex::Sep&, double >(), py::keep_alive<1,2>(), "sep"_a, "ratio"_a=0.01)
  //   .def("separate", &SepFixPoint::separate, py::call_guard<py::gil_scoped_release>())
  // ;

  // py::implicitly_convertible< std::array<double, 2>&, Interval>();

}
