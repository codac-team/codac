/** 
 *  \file
 *  Function Python binding
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoît Desrochers, Simon Rohou, Fabrice Le Bars
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <sstream>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <codac2_analytic_variables.h>
#include <codac2_analytic_operations.h>
#include <codac2_AnalyticFunction.h>
#include "codac2_py_AnalyticFunction_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_analytic_operations_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)

#include "codac2_py_doc.h"

#include "codac2_py_wrapper.h"

using namespace std;
using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;


template<typename T>
void export_AnalyticFunction(py::module& m, const std::string& export_name)
{
  py::class_<AnalyticFunction<T>, std::shared_ptr<AnalyticFunction<T>> /* due to enable_shared_from_this */>
    exported(m, export_name.c_str(), ANALYTICFUNCTION_MAIN);
  exported
  
    .def(py::init(
      [](const py::list& l, const ExprWrapper<T>& e)
      {
        vector<std::reference_wrapper<VarBase>> args;

        for(const auto& li : l)
        {
          if(py::isinstance<ScalarVar>(li))
            args.push_back(*std::make_shared<ScalarVar>(li.cast<ScalarVar>()));

          else if(py::isinstance<VectorVar>(li))
            args.push_back(*std::make_shared<VectorVar>(li.cast<VectorVar>()));

          else { assert(false && "unhandlded case"); }
        }

        return std::make_unique<AnalyticFunction<T>>(args, std::dynamic_pointer_cast<codac2::AnalyticExpr<T>>(e._e->copy()));
      }
    ), ANALYTICFUNCTION_TTYPENAME_ANALYTICFUNCTION_CONST_VECTOR_REFERENCE_WRAPPER_VARBASE_REF_CONST_SHARED_PTR_ANALYTICEXPR_T_REF);

  const char* EVAL_DOC = T_DOMAIN_ANALYTICFUNCTION_TTYPENAME_EVAL_CONST_ARGS_REF_VARIADIC_CONST;
  using I = const Interval&; using IV = const IntervalVector&;

  exported

    // Several cases of scalar inputs
    .def("eval", [](AnalyticFunction<T>& f, I x1) { return f.eval(x1); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2) { return f.eval(x1,x2); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3) { return f.eval(x1,x2,x3); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4) { return f.eval(x1,x2,x3,x4); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5) { return f.eval(x1,x2,x3,x4,x5); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6) { return f.eval(x1,x2,x3,x4,x5,x6); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7) { return f.eval(x1,x2,x3,x4,x5,x6,x7); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8) { return f.eval(x1,x2,x3,x4,x5,x6,x7,x8); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8, I x9) { return f.eval(x1,x2,x3,x4,x5,x6,x7,x8,x9); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8, I x9, I x10) { return f.eval(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, EVAL_DOC)

    // Several cases of vector inputs
    .def("eval", [](AnalyticFunction<T>& f, IV x1) { return f.eval(x1); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2) { return f.eval(x1,x2); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3) { return f.eval(x1,x2,x3); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4) { return f.eval(x1,x2,x3,x4); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5) { return f.eval(x1,x2,x3,x4,x5); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6) { return f.eval(x1,x2,x3,x4,x5,x6); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7) { return f.eval(x1,x2,x3,x4,x5,x6,x7); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8) { return f.eval(x1,x2,x3,x4,x5,x6,x7,x8); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8, IV x9) { return f.eval(x1,x2,x3,x4,x5,x6,x7,x8,x9); }, EVAL_DOC)
    .def("eval", [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8, IV x9, IV x10) { return f.eval(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, EVAL_DOC)

    // Mixed scalar/vector inputs are not supported yet

    .def("__repr__", [](const AnalyticFunction<T>& f) {
          std::ostringstream stream;
          stream << f;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_ANALYTICFUNCTION_T_REF)
  ;
}

template<typename T>
void export_ExprWrapper(py::module& m, const std::string& export_name)
{
  py::class_<ExprWrapper<T>>
    exported(m, export_name.c_str(), "todo");
  
  if constexpr(std::is_same_v<T,ScalarOpValue>)
    exported.def(py::init<ScalarVar>(),
      "Converting a ScalarVar into an ExprWrapper (binding object).");
  
  if constexpr(std::is_same_v<T,VectorOpValue>)
    exported.def(py::init<VectorVar>(),
      "Converting a VectorVar into an ExprWrapper (binding object).");
  
  exported

    .def(py::init<std::shared_ptr<AnalyticExpr<T>>>())
  ;
}

inline void export_expression_operations(py::module& m)
{
  m .def("cos", [](const ExprWrapper<ScalarOpValue>& e) { return ExprWrapper<ScalarOpValue>(cos(e._e)); },
      SCALAREXPR_PTR_COS_CONST_SCALAREXPR_PTR_REF,
      "x1"_a);

  const char* EVAL_VEC_DOC = VECTOREXPR_PTR_VEC_CONST_SHARED_PTR_ANALYTICEXPR_X_REF_VARIADIC;
  using S = const ExprWrapper<ScalarOpValue>&;

  m .def("vec", [](S e1) { return ExprWrapper<VectorOpValue>(vec(e1._e)); }, EVAL_VEC_DOC, "x1"_a)
    .def("vec", [](S e1, S e2) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a)
    .def("vec", [](S e1, S e2, S e3) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a)
    .def("vec", [](S e1, S e2, S e3, S e4) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e,e5._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e,e5._e,e6._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e,e5._e,e6._e,e7._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e,e5._e,e6._e,e7._e,e8._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8, S e9) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e,e5._e,e6._e,e7._e,e8._e,e9._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a,"x9"_a)
    .def("vec", [](S e1, S e2, S e3, S e4, S e5, S e6, S e7, S e8, S e9, S e10) { return ExprWrapper<VectorOpValue>(vec(e1._e,e2._e,e3._e,e4._e,e5._e,e6._e,e7._e,e8._e,e9._e,e10._e)); }, EVAL_VEC_DOC, "x1"_a,"x2"_a,"x3"_a,"x4"_a,"x5"_a,"x6"_a,"x7"_a,"x8"_a,"x9"_a,"x10"_a)
  ;
}