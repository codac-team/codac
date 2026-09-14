/**
 *  codac2_sympy_bridge.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_sympy_bridge.h"

#include <cmath>
#include <memory>
#include <mutex>
#include <sstream>
#include <unordered_map>

#include "codac2_assert.h"

namespace codac2
{
  namespace
  {
    using PosNode = AnalyticOperationExpr<AddOp,ScalarType,ScalarType>;
    using NegNode = AnalyticOperationExpr<SubOp,ScalarType,ScalarType>;
    using AddNode = AnalyticOperationExpr<AddOp,ScalarType,ScalarType,ScalarType>;
    using SubNode = AnalyticOperationExpr<SubOp,ScalarType,ScalarType,ScalarType>;
    using MulNode = AnalyticOperationExpr<MulOp,ScalarType,ScalarType,ScalarType>;
    using DivNode = AnalyticOperationExpr<DivOp,ScalarType,ScalarType,ScalarType>;
    using PowNode = AnalyticOperationExpr<PowOp,ScalarType,ScalarType,ScalarType>;
    using SinNode = AnalyticOperationExpr<SinOp,ScalarType,ScalarType>;
    using CosNode = AnalyticOperationExpr<CosOp,ScalarType,ScalarType>;
    using ExpNode = AnalyticOperationExpr<ExpOp,ScalarType,ScalarType>;
    using LogNode = AnalyticOperationExpr<LogOp,ScalarType,ScalarType>;
    using SqrtNode = AnalyticOperationExpr<SqrtOp,ScalarType,ScalarType>;
    using SqrNode = AnalyticOperationExpr<SqrOp,ScalarType,ScalarType>;
    using TanNode = AnalyticOperationExpr<TanOp,ScalarType,ScalarType>;
    using AsinNode = AnalyticOperationExpr<AsinOp,ScalarType,ScalarType>;
    using AcosNode = AnalyticOperationExpr<AcosOp,ScalarType,ScalarType>;
    using AtanNode = AnalyticOperationExpr<AtanOp,ScalarType,ScalarType>;
    using Atan2Node = AnalyticOperationExpr<Atan2Op,ScalarType,ScalarType,ScalarType>;
    using SinhNode = AnalyticOperationExpr<SinhOp,ScalarType,ScalarType>;
    using CoshNode = AnalyticOperationExpr<CoshOp,ScalarType,ScalarType>;
    using TanhNode = AnalyticOperationExpr<TanhOp,ScalarType,ScalarType>;
    using AbsNode = AnalyticOperationExpr<AbsOp,ScalarType,ScalarType>;
    using SignNode = AnalyticOperationExpr<SignOp,ScalarType,ScalarType>;
    using FloorNode = AnalyticOperationExpr<FloorOp,ScalarType,ScalarType>;
    using CeilNode = AnalyticOperationExpr<CeilOp,ScalarType,ScalarType>;
    using VecCompNode = AnalyticOperationExpr<ComponentOp,ScalarType,VectorType>;
    using MatCompNode = AnalyticOperationExpr<ComponentOp,ScalarType,MatrixType>;

    pybind11::tuple py_args(const pybind11::handle& obj)
    {
      return pybind11::reinterpret_borrow<pybind11::tuple>(obj.attr("args"));
    }

    bool py_truth(const pybind11::handle& obj, const char* attr)
    {
      return pybind11::cast<bool>(obj.attr(attr));
    }

    double sympy_number_to_double(const pybind11::handle& obj)
    {
      pybind11::object borrowed = pybind11::reinterpret_borrow<pybind11::object>(obj);
      pybind11::object as_float = symbolic::detail::import_builtins().attr("float")(borrowed);
      return pybind11::cast<double>(as_float);
    }

    std::vector<std::shared_ptr<ExprBase>> maybe_children_expr_base(const std::shared_ptr<ExprBase>& e)
    {
      if(!e)
        return {};
      return e->children_expr_base();
    }
  }

  namespace symbolic::detail
  {
    void ensure_python_runtime()
    {
      static std::once_flag once;

      if(Py_IsInitialized() != 0)
        return;

      std::call_once(once, []()
      {
        if(Py_IsInitialized() == 0)
          pybind11::initialize_interpreter();
      });
    }

    namespace
    {
      std::unordered_map<std::string,pybind11::object>& imported_modules_cache()
      {
        // Intentionally leaked: cached Python module objects must not be destroyed
        // during C++ static teardown, because their destructors may run after the
        // Python runtime has already started shutting down.
        static auto* modules = new std::unordered_map<std::string,pybind11::object>();
        return *modules;
      }
    }

    const pybind11::object& import_module(const char* module_name)
    {
      ensure_python_runtime();
      pybind11::gil_scoped_acquire gil;

      auto& modules = imported_modules_cache();
      const std::string key(module_name);
      auto it = modules.find(key);
      if(it == modules.end())
        it = modules.emplace(key, pybind11::module_::import(module_name)).first;

      return it->second;
    }

    const pybind11::object& import_sympy()
    {
      return import_module("sympy");
    }

    const pybind11::object& import_polyfuncs()
    {
      return import_module("sympy.polys.polyfuncs");
    }

    const pybind11::object& import_builtins()
    {
      return import_module("builtins");
    }

    pybind11::object normalize_sympy_expr(const pybind11::object& sympy, pybind11::object expr, bool do_expand)
    {
      expr = expr.attr("rewrite")(sympy.attr("sin"));
      expr = expr.attr("rewrite")(sympy.attr("cos"));
      expr = expr.attr("rewrite")(sympy.attr("sinh"));
      expr = expr.attr("rewrite")(sympy.attr("cosh"));
      if(do_expand)
        expr = sympy.attr("expand")(expr);
      return expr;
    }

    ScalarBridgeContext::ScalarBridgeContext(const FunctionArgsList& args)
      : _symbols(args),
        _exporter(_symbols),
        _importer(_symbols)
    {
    }

    const pybind11::object& ScalarBridgeContext::sympy() const
    {
      return import_sympy();
    }

    const FlatSymbolTable& ScalarBridgeContext::symbols() const
    {
      return _symbols;
    }

    pybind11::object ScalarBridgeContext::export_scalar(const ScalarExpr& y) const
    {
      return _exporter.export_scalar(y);
    }

    ScalarExpr ScalarBridgeContext::import_scalar(const pybind11::handle& obj) const
    {
      return _importer.import_scalar(obj);
    }

    ScalarExpr ScalarBridgeContext::transform_scalar_expr(
      const ScalarExpr& y,
      const SympyTransform& transform,
      bool do_expand) const
    {
      ensure_python_runtime();
      pybind11::gil_scoped_acquire gil;
      const pybind11::object& sympy = import_sympy();
      pybind11::object ys = _exporter.export_scalar(y);
      pybind11::object out = transform(sympy, ys, _symbols);
      return _importer.import_scalar(normalize_sympy_expr(sympy, out, do_expand));
    }

    ScalarExpr transform_scalar_expr(
      const FunctionArgsList& args,
      const ScalarExpr& y,
      const SympyTransform& transform,
      bool do_expand)
    {
      ensure_python_runtime();
      pybind11::gil_scoped_acquire gil;
      ScalarBridgeContext ctx(args);
      return ctx.transform_scalar_expr(y, transform, do_expand);
    }

    FlatSymbolTable::FlatSymbolTable(const FunctionArgsList& args)
      : _layout(args)
    {
      _names.reserve(static_cast<std::size_t>(_layout.size()));

      for(const auto& arg : args)
      {
        const auto& b = _layout.binding_of(arg->unique_id());

        if(auto s = std::dynamic_pointer_cast<ScalarVar>(arg))
        {
          const auto name = make_symbol_name(b.offset);
          _names.push_back(name);
          _codac_scalars.emplace(name, ScalarExpr(*s));
          continue;
        }

        if(auto v = std::dynamic_pointer_cast<VectorVar>(arg))
        {
          VectorExpr vv(*v);
          for(Index i = 0 ; i < v->size() ; ++i)
          {
            const auto name = make_symbol_name(b.offset + i);
            _names.push_back(name);
            _codac_scalars.emplace(name, vv[i]);
          }
          continue;
        }

        if(auto m = std::dynamic_pointer_cast<MatrixVar>(arg))
        {
          MatrixExpr mm(*m);
          for(Index i = 0 ; i < m->rows() ; ++i)
          {
            for(Index j = 0 ; j < m->cols() ; ++j)
            {
              const auto name = make_symbol_name(b.offset + b.cols*i + j);
              _names.push_back(name);
              _codac_scalars.emplace(name, mm(i,j));
            }
          }
          continue;
        }

        assert_release(false && "Unsupported variable type in FlatSymbolTable");
      }
    }

    pybind11::object FlatSymbolTable::by_flat_index(Index k) const
    {
      assert_release(k >= 0 && k < static_cast<Index>(_names.size())
        && "Flat input index out of bounds");

      ensure_python_runtime();
      pybind11::gil_scoped_acquire gil;
      return import_sympy().attr("Symbol")(_names[static_cast<std::size_t>(k)], pybind11::arg("real")=true);
    }

    ScalarExpr FlatSymbolTable::codac_expr_by_name(const std::string& name) const
    {
      auto it = _codac_scalars.find(name);
      assert_release(it != _codac_scalars.end() && "Unknown SymPy symbol in importer");
      return it->second;
    }

    Index FlatSymbolTable::size() const
    {
      return _layout.size();
    }

    pybind11::object FlatSymbolTable::for_scalar_var(const ScalarVar& x) const
    {
      const auto& b = binding_of(x.unique_id());
      assert_release(b.is_scalar() && "Internal binding mismatch for scalar variable");
      return by_flat_index(b.offset);
    }

    pybind11::object FlatSymbolTable::for_vector_component(const VectorVar& x, Index i) const
    {
      const auto& b = binding_of(x.unique_id());
      assert_release(b.is_vector() && "Internal binding mismatch for vector variable");
      assert_release(i >= 0 && i < b.rows && "Vector component out of bounds");
      return by_flat_index(b.offset + i);
    }

    pybind11::object FlatSymbolTable::for_matrix_component(const MatrixVar& x, Index i, Index j) const
    {
      const auto& b = binding_of(x.unique_id());
      assert_release(b.is_matrix() && "Internal binding mismatch for matrix variable");
      assert_release(i >= 0 && i < b.rows && j >= 0 && j < b.cols
        && "Matrix component out of bounds");

      return by_flat_index(b.offset + b.cols*i + j);
    }

    std::string FlatSymbolTable::make_symbol_name(Index flat_index)
    {
      std::ostringstream oss;
      oss << "_codac_sym_" << flat_index;
      return oss.str();
    }

    const FlatInputBinding& FlatSymbolTable::binding_of(const ExprID& id) const
    {
      return _layout.binding_of(id);
    }

    SympyExporter::SympyExporter(const FlatSymbolTable& symbols)
      : _symbols(symbols)
    {
    }

    pybind11::object SympyExporter::export_scalar(const ScalarExpr& e) const
    {
      ensure_python_runtime();
      pybind11::gil_scoped_acquire gil;
      return export_node(std::static_pointer_cast<ExprBase>(e));
    }

    pybind11::object SympyExporter::export_node(const std::shared_ptr<ExprBase>& e) const
    {
      const pybind11::object& sympy = import_sympy();

      if(auto c = std::dynamic_pointer_cast<ConstValueExpr<ScalarType>>(e))
      {
        const double v = scalar_const_value(*c);
        const double r = std::round(v);
        if(std::abs(v - r) <= 0.)
          return sympy.attr("Integer")(static_cast<long long>(r));
        return pybind11::float_(v);
      }

      if(auto x = std::dynamic_pointer_cast<ScalarVar>(e))
        return _symbols.for_scalar_var(*x);

      if(auto op = std::dynamic_pointer_cast<PosNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_node(child_at(ch,0));
      }

      if(auto op = std::dynamic_pointer_cast<NegNode>(e))
      {
        auto ch = op->children_expr_base();
        return -export_node(child_at(ch,0));
      }

      if(auto op = std::dynamic_pointer_cast<AddNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_node(child_at(ch,0)) + export_node(child_at(ch,1));
      }

      if(auto op = std::dynamic_pointer_cast<SubNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_node(child_at(ch,0)) - export_node(child_at(ch,1));
      }

      if(auto op = std::dynamic_pointer_cast<MulNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_node(child_at(ch,0)) * export_node(child_at(ch,1));
      }

      if(auto op = std::dynamic_pointer_cast<DivNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_node(child_at(ch,0)) / export_node(child_at(ch,1));
      }

      if(auto op = std::dynamic_pointer_cast<PowNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("Pow")(export_node(child_at(ch,0)), export_node(child_at(ch,1)));
      }

      if(auto op = std::dynamic_pointer_cast<SinNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("sin")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<CosNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("cos")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<ExpNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("exp")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<LogNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("log")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<SqrtNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("sqrt")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<SqrNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("Pow")(export_node(child_at(ch,0)), 2);
      }

      if(auto op = std::dynamic_pointer_cast<TanNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("tan")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<AsinNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("asin")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<AcosNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("acos")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<AtanNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("atan")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<Atan2Node>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("atan2")(
          export_node(child_at(ch,0)), export_node(child_at(ch,1)));
      }

      if(auto op = std::dynamic_pointer_cast<SinhNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("sinh")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<CoshNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("cosh")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<TanhNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("tanh")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<AbsNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("Abs")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<SignNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("sign")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<FloorNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("floor")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<CeilNode>(e))
      {
        auto ch = op->children_expr_base();
        return sympy.attr("ceiling")(export_node(child_at(ch,0)));
      }

      if(auto op = std::dynamic_pointer_cast<VecCompNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_vector_component(child_at(ch,0), op->i());
      }

      if(auto op = std::dynamic_pointer_cast<MatCompNode>(e))
      {
        auto ch = op->children_expr_base();
        return export_matrix_component(child_at(ch,0), op->i(), op->j());
      }

      assert_release(false && "Unsupported Codac scalar node in SympyExporter");
      return pybind11::none();
    }

    pybind11::object SympyExporter::export_vector_component(const std::shared_ptr<ExprBase>& e, Index i) const
    {
      if(auto v = std::dynamic_pointer_cast<VectorVar>(e))
        return _symbols.for_vector_component(*v, i);

      if(auto ve = std::dynamic_pointer_cast<AnalyticExpr<VectorType>>(e))
      {
        auto shape = ve->output_shape();
        auto children = maybe_children_expr_base(e);
        if(shape.second == 1 && i >= 0 && i < shape.first && children.size() == static_cast<std::size_t>(shape.first))
          return export_node(child_at(children,i));
      }

      assert_release(false
        && "Unsupported vector-component expression. Supported cases: direct VectorVar components, or vector expressions exposing scalar children via children_expr_base().");
      return pybind11::none();
    }

    pybind11::object SympyExporter::export_matrix_component(const std::shared_ptr<ExprBase>& e, Index i, Index j) const
    {
      if(auto m = std::dynamic_pointer_cast<MatrixVar>(e))
        return _symbols.for_matrix_component(*m, i, j);

      if(auto me = std::dynamic_pointer_cast<AnalyticExpr<MatrixType>>(e))
      {
        auto shape = me->output_shape();
        auto children = maybe_children_expr_base(e);
        if(i >= 0 && i < shape.first && j >= 0 && j < shape.second
          && children.size() == static_cast<std::size_t>(shape.second))
          return export_vector_component(child_at(children,j), i);
      }

      assert_release(false
        && "Unsupported matrix-component expression. Supported cases: direct MatrixVar components, or matrix expressions exposing column children via children_expr_base().");
      return pybind11::none();
    }

    double SympyExporter::scalar_const_value(const ConstValueExpr<ScalarType>& c)
    {
      const Interval& v = c.value();
      assert_release(v.is_degenerated() && "Only degenerate scalar constants can be exported to SymPy");
      return v.mid();
    }

    std::shared_ptr<ExprBase> SympyExporter::child_at(
      const std::vector<std::shared_ptr<ExprBase>>& children, Index i)
    {
      assert_release(i >= 0 && i < static_cast<Index>(children.size())
        && "Arity mismatch while exporting SymPy expression");
      return children[static_cast<std::size_t>(i)];
    }

    SympyImporter::SympyImporter(const FlatSymbolTable& symbols)
      : _symbols(symbols)
    {
    }

    ScalarExpr SympyImporter::import_scalar(const pybind11::handle& obj) const
    {
      ensure_python_runtime();
      pybind11::gil_scoped_acquire gil;

      if(py_truth(obj, "is_number"))
        return const_value(sympy_number_to_double(obj));

      if(py_truth(obj, "is_Symbol"))
        return _symbols.codac_expr_by_name(pybind11::cast<std::string>(pybind11::str(obj)));

      if(py_truth(obj, "is_Add"))
        return import_add(obj);

      if(py_truth(obj, "is_Mul"))
        return import_mul(obj);

      if(py_truth(obj, "is_Pow"))
        return import_pow(obj);

      return import_function(obj);
    }

    ScalarExpr SympyImporter::import_add(const pybind11::handle& obj) const
    {
      pybind11::tuple args = py_args(obj);
      assert_release(!args.empty() && "Unexpected empty Add in SymPy importer");

      ScalarExpr acc = import_scalar(args[0]);
      for(pybind11::size_t i = 1 ; i < args.size() ; ++i)
        acc = acc + import_scalar(args[i]);
      return acc;
    }

    ScalarExpr SympyImporter::import_mul(const pybind11::handle& obj) const
    {
      pybind11::tuple args = py_args(obj);
      assert_release(!args.empty() && "Unexpected empty Mul in SymPy importer");

      ScalarExpr acc = import_scalar(args[0]);
      for(pybind11::size_t i = 1 ; i < args.size() ; ++i)
        acc = acc * import_scalar(args[i]);
      return acc;
    }

    ScalarExpr SympyImporter::import_pow(const pybind11::handle& obj) const
    {
      pybind11::tuple args = py_args(obj);
      assert_release(args.size() == 2 && "SymPy Pow node should have arity 2");

      const pybind11::object& sympy = import_sympy();
      const auto base = import_scalar(args[0]);
      pybind11::handle expo = args[1];

      if(pybind11::isinstance(expo, sympy.attr("Integer")))
      {
        const long n = pybind11::cast<long>(expo);

        if(n == 0)  return const_value(1.);
        if(n == 1)  return base;
        if(n == 2)  return sqr(base);
        if(n == -1) return const_value(1.) / base;
        if(n == -2) return const_value(1.) / sqr(base);

        ScalarExpr acc = const_value(1.);
        if(n > 0)
        {
          for(long k = 0 ; k < n ; ++k)
            acc = acc * base;
          return acc;
        }
        else
        {
          for(long k = 0 ; k < -n ; ++k)
            acc = acc * base;
          return const_value(1.) / acc;
        }
      }

      if(pybind11::isinstance(expo, sympy.attr("Rational")))
      {
        const long num = pybind11::cast<long>(expo.attr("p"));
        const long den = pybind11::cast<long>(expo.attr("q"));

        if(num == 1 && den == 2)
          return sqrt(base);
        if(num == -1 && den == 2)
          return const_value(1.) / sqrt(base);
      }

      return pow(base, import_scalar(expo));
    }

    ScalarExpr SympyImporter::import_function(const pybind11::handle& obj) const
    {
      const pybind11::object& sympy = import_sympy();
      pybind11::object func = obj.attr("func");
      pybind11::tuple args = py_args(obj);

      if(args.size() == 1)
      {
        auto x = import_scalar(args[0]);

        if(func.is(sympy.attr("sin")))     return sin(x);
        if(func.is(sympy.attr("cos")))     return cos(x);
        if(func.is(sympy.attr("tan")))     return tan(x);
        if(func.is(sympy.attr("asin")))    return asin(x);
        if(func.is(sympy.attr("acos")))    return acos(x);
        if(func.is(sympy.attr("atan")))    return atan(x);
        if(func.is(sympy.attr("sinh")))    return sinh(x);
        if(func.is(sympy.attr("cosh")))    return cosh(x);
        if(func.is(sympy.attr("tanh")))    return tanh(x);
        if(func.is(sympy.attr("exp")))     return exp(x);
        if(func.is(sympy.attr("log")))     return log(x);
        if(func.is(sympy.attr("sqrt")))    return sqrt(x);
        if(func.is(sympy.attr("Abs")))     return abs(x);
        if(func.is(sympy.attr("sign")))    return sign(x);
        if(func.is(sympy.attr("floor")))   return floor(x);
        if(func.is(sympy.attr("ceiling"))) return ceil(x);
      }
      else if(args.size() == 2)
      {
        auto x1 = import_scalar(args[0]);
        auto x2 = import_scalar(args[1]);

        if(func.is(sympy.attr("atan2"))) return atan2(x1, x2);
      }

      assert_release(false && "Unsupported SymPy node in importer");
      return const_value(0.);
    }
  }
}
