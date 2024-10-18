/** 
 *  Codac binding (core)
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
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
#include "codac2_py_FunctionBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_ExprWrapper.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

#define bind_(exported, op_name, op, doc) \
  \
  exported \
  \
    /* Several cases of scalar inputs */ \
    .def(op_name, [](AnalyticFunction<T>& f) { return f.op(); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1) { return f.op(x1); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2) { return f.op(x1,x2); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3) { return f.op(x1,x2,x3); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4) { return f.op(x1,x2,x3,x4); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5) { return f.op(x1,x2,x3,x4,x5); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6) { return f.op(x1,x2,x3,x4,x5,x6); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7) { return f.op(x1,x2,x3,x4,x5,x6,x7); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8, I x9) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8,x9); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8, I x9, I x10) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, doc) \
  \
    /* Several cases of vector inputs */ \
    .def(op_name, [](AnalyticFunction<T>& f) { return f.op(); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1) { return f.op(x1); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2) { return f.op(x1,x2); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3) { return f.op(x1,x2,x3); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4) { return f.op(x1,x2,x3,x4); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5) { return f.op(x1,x2,x3,x4,x5); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6) { return f.op(x1,x2,x3,x4,x5,x6); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7) { return f.op(x1,x2,x3,x4,x5,x6,x7); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8, IV x9) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8,x9); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8, IV x9, IV x10) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, doc) \
  \
  ; \

template<typename T>
void export_AnalyticFunction(py::module& m, const std::string& export_name)
{
  py::class_<AnalyticFunction<T>, std::shared_ptr<AnalyticFunction<T>> /* due to enable_shared_from_this */>
    exported(m, export_name.c_str(), ANALYTICFUNCTION_MAIN);
  exported
  
    .def(py::init(
      [](const std::vector<py::object>& l, const ExprWrapper<T>& expr)
      {
        FunctionArgsList args {};
        size_t i = 0;

        for(const auto& li : l)
        {
          i++;

          if(py::isinstance<ScalarVar>(li))
            args.push_back(li.cast<ScalarVar>().arg_copy());

          else if(py::isinstance<VectorVar>(li))
            args.push_back(li.cast<VectorVar>().arg_copy());

          else
            throw std::invalid_argument("Argument " + std::to_string(i) + " is invalid. Only variables are accepted.");
        }
        
        return std::make_unique<AnalyticFunction<T>>(args, expr.copy());
      }
    ), ANALYTICFUNCTION_T_ANALYTICFUNCTION_CONST_FUNCTIONARGSLIST_REF_CONST_SHARED_PTR_ANALYTICEXPR_T_REF)

    .def("input_size", &AnalyticFunction<T>::input_size,
      SIZET_FUNCTIONBASE_E_INPUT_SIZE_CONST)

    .def("__call__", [](const AnalyticFunction<T>& f, const std::vector<ExprWrapperBase>& x)
      {
        std::vector<std::shared_ptr<ExprBase>> v(x.size());
        for(size_t i = 0 ; i < x.size() ; i++)
          v[i] = x[i].eb->copy();
        return ExprWrapper<T>(std::dynamic_pointer_cast<AnalyticExpr<T>>(f(v)->copy()));
      },
      SHARED_PTR_E_FUNCTIONBASE_E_OPERATORCALL_CONST_X_REF_VARIADIC_CONST)
  ;

  using I = const Interval&; using IV = const IntervalVector&;

  bind_(exported, "eval", eval, T_DOMAIN_ANALYTICFUNCTION_T_EVAL_CONST_ARGS_REF_VARIADIC_CONST);
  bind_(exported, "natural_eval", natural_eval, T_DOMAIN_ANALYTICFUNCTION_T_NATURAL_EVAL_CONST_ARGS_REF_VARIADIC_CONST);
  bind_(exported, "centered_eval", centered_eval, T_DOMAIN_ANALYTICFUNCTION_T_CENTERED_EVAL_CONST_ARGS_REF_VARIADIC_CONST);
  bind_(exported, "diff", diff, AUTO_ANALYTICFUNCTION_T_DIFF_CONST_ARGS_REF_VARIADIC_CONST);

  exported

    // Mixed scalar/vector inputs are not supported yet

    .def("__repr__", [](const AnalyticFunction<T>& f) {
          std::ostringstream stream;
          stream << f;
          return string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_ANALYTICFUNCTION_T_REF)
  ;
}