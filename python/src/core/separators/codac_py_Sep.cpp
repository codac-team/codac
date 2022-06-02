/** 
 *  \file
 *  Sep Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Benoît Desrochers, Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include "codac_type_caster.h"

#include "codac_py_Sep.h"
// Generated file from Doxygen XML (doxygen2docstring.py):
#include "codac_py_Sep_docs.h" // todo: generate this file from Doxygen doc
#include "codac_py_SepFunction_docs.h"

#include <codac_Ctc.h>
#include <codac_Function.h>
#include <codac_SepFunction.h>
#include <codac_SepBox.h>
#include <codac_SepCtcPairProj.h>
#include <codac_SepProj.h>
#include <codac_SepFixPoint.h>
#include <codac_QInterProjF.h>

#include <ibex_SepUnion.h>
#include <ibex_SepInter.h>
#include <ibex_SepNot.h>
#include <ibex_SepCtcPair.h>
#include <ibex_SepFwdBwd.h>
#include <ibex_SepInverse.h>
#include <ibex_SepQInter.h>

using namespace std;
using namespace codac;
namespace py = pybind11;
using namespace pybind11::literals;


py::class_<ibex::Sep,pySep> export_Sep(py::module& m)
{
  // Export Ctc
  py::class_<ibex::Sep,pySep> sep(m, "Sep", __DOC_SEP_TYPE);
  sep

    .def(py::init<int>())
    .def_readonly("nb_var", &ibex::Sep::nb_var, __DOC_SEP_NB_VAR)
    .def("separate", (void (ibex::Sep::*)(IntervalVector&,IntervalVector&)) &ibex::Sep::separate,
      __DOC_SEP_SEPARATE,
      py::arg("x_in"), py::arg("x_out"), py::call_guard<py::gil_scoped_release>())

    .def("__or__", [](ibex::Sep& s1, ibex::Sep& s2)
      {
        return new ibex::SepUnion(s1, s2);
        // todo: manage delete
      },
      __DOC_SEP_OR,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::keep_alive<0,2>(), py::call_guard<py::gil_scoped_release>())

    .def("__and__", [](ibex::Sep& s1, ibex::Sep& s2)
      {
        return new ibex::SepInter(s1, s2);
        // todo: manage delete
      },
      __DOC_SEP_AND,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::keep_alive<0,2>(), py::call_guard<py::gil_scoped_release>())

    .def("__invert__", [](ibex::Sep& s)
      {
        return new ibex::SepNot(s);
        // todo: manage delete
      },
      __DOC_SEP_INVERSE,
      py::return_value_policy::take_ownership,
      py::keep_alive<0,1>(), py::call_guard<py::gil_scoped_release>())
    ;

  // Export SepUnion
  py::class_<ibex::SepUnion>(m, "SepUnion", sep, __DOC_SEP_SEPUNION)
    .def(py::init<ibex::Array<ibex::Sep>>(), py::keep_alive<1,2>(), py::arg("list"))
    .def("separate", &ibex::SepUnion::separate)
  ;

  // Export SepInter
  py::class_<ibex::SepInter>(m, "SepInter", sep, __DOC_SEP_SEPINTER)
    .def(py::init<ibex::Array<ibex::Sep>>(), py::keep_alive<1,2>(), py::arg("list"))
    .def("separate", &ibex::SepInter::separate, py::call_guard<py::gil_scoped_release>())
  ;

  // Export SepCtcPair
  py::class_<ibex::SepCtcPair>(m, "SepCtcPair", sep, __DOC_SEP_SEPCTCPAIR)
    .def(py::init<Ctc&,Ctc&>(), py::keep_alive<1,2>(), py::keep_alive<1,3>(), py::arg("ctc_in"), py::arg("ctc_out"))
    .def("separate", (void (ibex::Sep::*)(IntervalVector&, IntervalVector&)) &ibex::SepCtcPair::separate, py::call_guard<py::gil_scoped_release>())
    .def("ctc_in", [](const ibex::SepCtcPair* s) -> const Ctc& { return s->ctc_in; }, py::return_value_policy::reference_internal)
    .def("ctc_out", [](const ibex::SepCtcPair* s) -> const Ctc& { return s->ctc_out; }, py::return_value_policy::reference_internal)
  ;

  // Export SepFwdBwd
  py::class_<ibex::SepFwdBwd>(m, "SepFwdBwd", sep, __DOC_SEP_SEPFWDBWD)
    .def(py::init<Function&,ibex::CmpOp>(), py::keep_alive<1,2>(), py::arg("f"), py::arg("op"))
    .def(py::init<Function&,Interval&>(), py::keep_alive<1,2>(), py::arg("f"), py::arg("itv_y"))
    .def(py::init<Function&,IntervalVector&>(), py::keep_alive<1,2>(), py::arg("f"), py::arg("itv_y"))
    .def(py::init([](Function& f,const array<double, 2>& itv)
      {
        return unique_ptr<ibex::SepFwdBwd>(new ibex::SepFwdBwd(f, Interval(itv[0], itv[1])));
      }),
      py::keep_alive<1,2>(), py::arg("f"), py::arg("itv_y"))
    .def("separate", (void (ibex::Sep::*)(IntervalVector&,IntervalVector&)) &ibex::SepFwdBwd::separate)
  ;

  // Export SepFunction
  py::class_<SepFunction>(m, "SepFunction", sep, SEPFUNCTION_MAIN)
    .def(py::init<Function&>(),
      SEPFUNCTION_SEPFUNCTION_FUNCTION,
      "f"_a.noconvert())
    .def(py::init<Function&,const Interval&>(),
      SEPFUNCTION_SEPFUNCTION_FUNCTION_INTERVAL,
      "f"_a.noconvert(), "y"_a.noconvert())
    .def(py::init<Function&,const IntervalVector&>(),
      SEPFUNCTION_SEPFUNCTION_FUNCTION_INTERVALVECTOR,
      "f"_a.noconvert(), "y"_a.noconvert())
    .def("separate", &SepFunction::separate,
      SEPFUNCTION_VOID_SEPARATE_INTERVALVECTOR_INTERVALVECTOR,
      "x_in"_a.noconvert(), "x_out"_a.noconvert())
  ;

  // Export SepNot
  py::class_<ibex::SepNot>(m, "SepNot", sep, __DOC_SEP_SEPNOT)
    .def(py::init<ibex::Sep&>(), py::keep_alive<1,2>(), py::arg("sep"))
    .def("separate", &ibex::SepNot::separate)
  ;

  // Export SepInverse
  py::class_<ibex::SepInverse>(m, "SepInverse", sep, __DOC_SEP_SEPINVERSE)
    .def(py::init<ibex::Sep&,Function&>(), py::keep_alive<1,2>(), py::keep_alive<1,3>(), py::arg("sep"), py::arg("f"))
    .def("separate", &ibex::SepInverse::separate, py::call_guard<py::gil_scoped_release>())
  ;

  // Export SepQInter
  py::class_<ibex::SepQInter>(m, "SepQInter", sep, __DOC_SEP_SEPQINTER)
    .def(py::init<ibex::Array<ibex::Sep>>(), py::keep_alive<1,2>(), py::arg("list"))
    .def("separate", &ibex::SepQInter::separate)
    .def_property("q", py::cpp_function(&ibex::SepQInter::get_q), py::cpp_function(&ibex::SepQInter::set_q))
  ;

  // Export SepBox
  py::class_<SepBox>(m, "SepBox", sep, __DOC_SEP_SEPINTER)
    .def(py::init<const IntervalVector&>())
    .def("separate", &SepBox::separate)
  ;

  // Export SepQInterProjF
  py::class_<SepQInterProjF>(m, "SepQInterProjF", sep, __DOC_SEP_SEPQINTERPROJF)
    .def(py::init<Array<Sep> >(), py::keep_alive<1,2>(), py::arg("list"))
    .def("separate", &SepQInterProjF::separate)
    .def_property("q", py::cpp_function(&SepQInterProjF::get_q), py::cpp_function(&SepQInterProjF::set_q))
  ;


  // Export SepProj
  py::class_<SepProj>(m, "SepProj", sep, __DOC_SEP_SEPPROJ)
    .def(py::init<ibex::Sep&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec")=1e-3 )
    .def(py::init<ibex::Sep&,const Interval&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec")=1e-3 )
    .def(py::init(
        [](ibex::Sep& S,const std::array<double, 2>& itv, double eps){
          return std::unique_ptr<SepProj> (
            new SepProj(S, Interval(itv[0], itv[1]), eps)
          );
        }),
        py::keep_alive<1,2>(),
        py::arg("sep"),
        py::arg("y_init"),
        py::arg("prec")=1e-3 )

    .def("separate", &SepProj::separate, py::call_guard<py::gil_scoped_release>())
  ;

  // Export SepCtcPairProj
  py::class_<SepCtcPairProj>(m, "SepCtcPairProj", sep, __DOC_SEP_SEPCTCPAIRPROJ)
    .def(py::init<ibex::Ctc&, ibex::Ctc&,const IntervalVector&, double>(), py::keep_alive<1,2>(), py::keep_alive<1,3>(),
          py::arg("ctc_in"), py::arg("ctc_out"), py::arg("y_init"), py::arg("prec"))
    .def(py::init<ibex::SepCtcPair&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec") )
    .def(py::init<ibex::Sep&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec") )
    .def("separate", &SepCtcPairProj::separate, py::call_guard<py::gil_scoped_release>())
  ;

  // Export SepFixPoint
  py::class_<SepFixPoint>(m, "SepFixPoint", sep)
    .def(py::init<ibex::Sep&, double >(), py::keep_alive<1,2>(), "sep"_a, "ratio"_a=0.01)
    .def("separate", &SepFixPoint::separate, py::call_guard<py::gil_scoped_release>())
  ;


  return sep;
}