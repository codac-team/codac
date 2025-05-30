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
#include <codac2_AnalyticFunction.h>
#include <codac2_analytic_variables.h>
#include "codac2_py_AnalyticFunction_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_AnalyticFunction_impl_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_FunctionBase_docs.h" // Generated file from Doxygen XML (doxygen2docstring.py)
#include "codac2_py_AnalyticExprWrapper.h"
#include "codac2_py_cast.h"

using namespace codac2;
namespace py = pybind11;
using namespace pybind11::literals;

#define bind_mode_(exported, op_name, op, doc) \
  \
  exported \
  \
    /* Several cases of scalar inputs */ \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m) { return f.op(m); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1) { return f.op(m,x1); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2) { return f.op(m,x1,x2); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3) { return f.op(m,x1,x2,x3); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4) { return f.op(m,x1,x2,x3,x4); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4, I x5) { return f.op(m,x1,x2,x3,x4,x5); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4, I x5, I x6) { return f.op(m,x1,x2,x3,x4,x5,x6); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4, I x5, I x6, I x7) { return f.op(m,x1,x2,x3,x4,x5,x6,x7); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8, I x9) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8,x9); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, I x1, I x2, I x3, I x4, I x5, I x6, I x7, I x8, I x9, I x10) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, doc) \
  \
    /* Several cases of vector inputs */ \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m) { return f.op(m); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, py::list x1) { return f.op(m,cast<IntervalVector>(x1)); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1) { return f.op(m,x1); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2) { return f.op(m,x1,x2); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3) { return f.op(m,x1,x2,x3); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4) { return f.op(m,x1,x2,x3,x4); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4, IV x5) { return f.op(m,x1,x2,x3,x4,x5); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6) { return f.op(m,x1,x2,x3,x4,x5,x6); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7) { return f.op(m,x1,x2,x3,x4,x5,x6,x7); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8, IV x9) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8,x9); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IV x1, IV x2, IV x3, IV x4, IV x5, IV x6, IV x7, IV x8, IV x9, IV x10) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, doc) \
  \
    /* Several cases of matrix inputs */ \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m) { return f.op(m); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1) { return f.op(m,x1); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2) { return f.op(m,x1,x2); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3) { return f.op(m,x1,x2,x3); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4) { return f.op(m,x1,x2,x3,x4); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4, IM x5) { return f.op(m,x1,x2,x3,x4,x5); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6) { return f.op(m,x1,x2,x3,x4,x5,x6); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7) { return f.op(m,x1,x2,x3,x4,x5,x6,x7); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7, IM x8) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7, IM x8, IM x9) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8,x9); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, const EvalMode& m, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7, IM x8, IM x9, IM x10) { return f.op(m,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, doc) \
  \
  ; \

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
    .def(op_name, [](AnalyticFunction<T>& f, py::list x1) { return f.op(cast<IntervalVector>(x1)); }, doc) \
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
    /* Several cases of matrix inputs */ \
    .def(op_name, [](AnalyticFunction<T>& f) { return f.op(); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1) { return f.op(x1); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2) { return f.op(x1,x2); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3) { return f.op(x1,x2,x3); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4) { return f.op(x1,x2,x3,x4); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4, IM x5) { return f.op(x1,x2,x3,x4,x5); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6) { return f.op(x1,x2,x3,x4,x5,x6); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7) { return f.op(x1,x2,x3,x4,x5,x6,x7); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7, IM x8) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7, IM x8, IM x9) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8,x9); }, doc) \
    .def(op_name, [](AnalyticFunction<T>& f, IM x1, IM x2, IM x3, IM x4, IM x5, IM x6, IM x7, IM x8, IM x9, IM x10) { return f.op(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10); }, doc) \
  \
  ; \

inline FunctionArgsList create_FunctionArgsList(const std::vector<py::object>& l)
{
  FunctionArgsList args {};
  Index i = 0;

  for(const auto& li : l)
  {
    i++;

    if(py::isinstance<ScalarVar>(li))
      args.push_back(li.cast<ScalarVar>().arg_copy());

    else if(py::isinstance<VectorVar>(li))
      args.push_back(li.cast<VectorVar>().arg_copy());

    else if(py::isinstance<MatrixVar>(li))
      args.push_back(li.cast<MatrixVar>().arg_copy());

    else
      throw std::invalid_argument("Argument " + std::to_string(i) + " is invalid. Only variables are accepted.");
  }

  args.compute_unique_arg_names();
  return args;
}

template<typename T>
void export_AnalyticFunction(py::module& m, const std::string& export_name)
{
  py::class_<AnalyticFunction<T>, std::shared_ptr<AnalyticFunction<T>> /* due to enable_shared_from_this */>
    exported(m, export_name.c_str(), ANALYTICFUNCTION_MAIN);
  exported
  
    .def(py::init(
      [](const std::vector<py::object>& l, const AnalyticExprWrapper<T>& expr)
      {
        return std::make_unique<AnalyticFunction<T>>(
          create_FunctionArgsList(l),
          AnalyticExprWrapper<T>{ std::dynamic_pointer_cast<AnalyticExpr<T>>(expr.get()->copy()) }
        );
      }
    ), ANALYTICFUNCTION_T_ANALYTICFUNCTION_CONST_FUNCTIONARGSLIST_REF_CONST_ANALYTICEXPRWRAPPER_T_REF)
  ;

  if constexpr(std::is_same_v<T,VectorType>)
  {
    exported.def(py::init(
      [](const std::vector<py::object>& l, const std::vector<py::object>& v_expr)
      {
        return std::make_unique<AnalyticFunction<T>>(
          create_FunctionArgsList(l),
          [&]() -> VectorExpr {
            switch(v_expr.size())
            {
              case 1:
                return vec(
                  cast<ScalarExpr>(v_expr[0])
                );
              case 2:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1])
                );
              case 3:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2])
                );
              case 4:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3])
                );
              case 5:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3]),
                  cast<ScalarExpr>(v_expr[4])
                );
              case 6:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3]),
                  cast<ScalarExpr>(v_expr[4]), cast<ScalarExpr>(v_expr[5])
                );
              case 7:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3]),
                  cast<ScalarExpr>(v_expr[4]), cast<ScalarExpr>(v_expr[5]),
                  cast<ScalarExpr>(v_expr[6])
                );
              case 8:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3]),
                  cast<ScalarExpr>(v_expr[4]), cast<ScalarExpr>(v_expr[5]),
                  cast<ScalarExpr>(v_expr[6]), cast<ScalarExpr>(v_expr[7])
                );
              case 9:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3]),
                  cast<ScalarExpr>(v_expr[4]), cast<ScalarExpr>(v_expr[5]),
                  cast<ScalarExpr>(v_expr[6]), cast<ScalarExpr>(v_expr[7]),
                  cast<ScalarExpr>(v_expr[8])
                );
              case 10:
                return vec(
                  cast<ScalarExpr>(v_expr[0]), cast<ScalarExpr>(v_expr[1]),
                  cast<ScalarExpr>(v_expr[2]), cast<ScalarExpr>(v_expr[3]),
                  cast<ScalarExpr>(v_expr[4]), cast<ScalarExpr>(v_expr[5]),
                  cast<ScalarExpr>(v_expr[6]), cast<ScalarExpr>(v_expr[7]),
                  cast<ScalarExpr>(v_expr[8]), cast<ScalarExpr>(v_expr[9])
                );
              default:
                assert_release("cannot create AnalyticFunction with provided vector arguments");
                return { nullptr };
            }
          }()
        );
      }
    ), ANALYTICFUNCTION_T_ANALYTICFUNCTION_CONST_FUNCTIONARGSLIST_REF_CONST_SCALAREXPRLIST_REF);
  }

  exported

    .def(py::init<const AnalyticFunction<T>&>(),
      ANALYTICFUNCTION_T_ANALYTICFUNCTION_CONST_ANALYTICFUNCTION_T_REF)

    .def("input_size", &AnalyticFunction<T>::input_size,
      INDEX_FUNCTIONBASE_E_INPUT_SIZE_CONST)

    .def("output_size", &AnalyticFunction<T>::output_size,
      INDEX_ANALYTICFUNCTION_T_OUTPUT_SIZE_CONST)

    .def("__call__", [](const AnalyticFunction<T>& f, const ScalarExpr& x)
      {
        return AnalyticExprWrapper<T>(
          std::dynamic_pointer_cast<AnalyticExpr<T>>(
            f(x)->copy()));
      },
      SHARED_PTR_E_FUNCTIONBASE_E_OPERATORCALL_CONST_X_REF_VARIADIC_CONST)

    .def("__call__", [](const AnalyticFunction<T>& f, const ScalarVar& x)
      {
        return AnalyticExprWrapper<T>(
          std::dynamic_pointer_cast<AnalyticExpr<T>>(
            f(x)->copy()));
      },
      SHARED_PTR_E_FUNCTIONBASE_E_OPERATORCALL_CONST_X_REF_VARIADIC_CONST)

    .def("__call__", [](const AnalyticFunction<T>& f, py::list& x)
      {
        std::vector<std::shared_ptr<ExprBase>> v(x.size());
        for(size_t i = 0 ; i < x.size() ; i++)
          v[i] = cast<std::shared_ptr<ExprBase>>(x[i]);
        return AnalyticExprWrapper<T>(std::dynamic_pointer_cast<AnalyticExpr<T>>(f(v)->copy()));
      },
      SHARED_PTR_E_FUNCTIONBASE_E_OPERATORCALL_CONST_X_REF_VARIADIC_CONST)
  ;

  using I = const Interval&;
  using IV = const IntervalVector&;
  using IM = const IntervalMatrix&;

  bind_(exported, "real_eval", real_eval, AUTO_ANALYTICFUNCTION_T_REAL_EVAL_CONST_ARGS_REF_VARIADIC_CONST);
  bind_mode_(exported, "eval", eval, T_DOMAIN_ANALYTICFUNCTION_T_EVAL_CONST_ARGS_REF_VARIADIC_CONST);
  bind_(exported, "eval", eval, T_DOMAIN_ANALYTICFUNCTION_T_EVAL_CONST_ARGS_REF_VARIADIC_CONST);
  bind_(exported, "diff", diff, AUTO_ANALYTICFUNCTION_T_DIFF_CONST_ARGS_REF_VARIADIC_CONST);

  exported

    // Mixed scalar/vector inputs are not supported yet

    .def("__repr__", [](const AnalyticFunction<T>& f) {
          std::ostringstream stream;
          stream << f;
          return std::string(stream.str()); 
        },
      OSTREAM_REF_OPERATOROUT_OSTREAM_REF_CONST_ANALYTICFUNCTION_U_REF)
  ;
}