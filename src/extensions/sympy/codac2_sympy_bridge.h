/** 
 *  \file codac2_sympy_bridge.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <functional>
#include <pybind11/embed.h>

#include "codac2_sympy.h"
#include "codac2_analytic_flat_input_layout.h"

namespace codac2::symbolic::detail
{
  /**
   * \brief Symbol table associated with a flattened analytic input domain.
   *
   * This class maps each scalar entry of a function input domain to a dedicated
   * SymPy symbol, and provides the reverse mapping from such symbols to Codac
   * scalar expressions.
   */
  class FlatSymbolTable
  {
    public:

      /**
       * \brief Builds the symbol table associated with a function argument list.
       *
       * \param args Function argument list.
       */
      explicit FlatSymbolTable(const FunctionArgsList& args);

      /**
       * \brief Returns the SymPy symbol associated with a flat input index.
       *
       * \param k Flat input index.
       * \return SymPy symbol associated with \p k.
       */
      pybind11::object by_flat_index(Index k) const;

      /**
       * \brief Returns the Codac scalar expression associated with a symbol name.
       *
       * \param name SymPy symbol name.
       * \return Corresponding Codac scalar expression.
       */
      ScalarExpr codac_expr_by_name(const std::string& name) const;

      /**
       * \brief Returns the number of scalar entries in the flattened input domain.
       *
       * \return Flattened input domain size.
       */
      Index size() const;

      /**
       * \brief Returns the SymPy symbol associated with a scalar variable.
       *
       * \param x Scalar variable.
       * \return Corresponding SymPy symbol.
       */
      pybind11::object for_scalar_var(const ScalarVar& x) const;

      /**
       * \brief Returns the SymPy symbol associated with a vector component.
       *
       * \param x Vector variable.
       * \param i Component index.
       * \return Corresponding SymPy symbol.
       */
      pybind11::object for_vector_component(const VectorVar& x, Index i) const;

      /**
       * \brief Returns the SymPy symbol associated with a matrix component.
       *
       * \param x Matrix variable.
       * \param i Row index.
       * \param j Column index.
       * \return Corresponding SymPy symbol.
       */
      pybind11::object for_matrix_component(const MatrixVar& x, Index i, Index j) const;

    private:

      /**
       * \brief Builds the symbol name associated with a flat input index.
       *
       * \param flat_index Flat input index.
       * \return Symbol name associated with \p flat_index.
       */
      static std::string make_symbol_name(Index flat_index);

      /**
       * \brief Returns the flattened binding associated with an input identifier.
       *
       * \param id Input expression identifier.
       * \return Binding associated with \p id.
       */
      const FlatInputBinding& binding_of(const ExprID& id) const;

      FlatInputLayout _layout; //!< Flattened input layout.
      std::vector<std::string> _names; //!< Symbol names indexed by flat input index.
      std::unordered_map<std::string,ScalarExpr> _codac_scalars; //!< Reverse mapping from symbol names to Codac scalar expressions.
  };

  /**
   * \brief Exports Codac scalar expressions to SymPy expressions.
   */
  class SympyExporter
  {
    public:

      /**
       * \brief Builds an exporter associated with a symbol table.
       *
       * \param symbols Flat symbol table.
       */
      explicit SympyExporter(const FlatSymbolTable& symbols);

      /**
       * \brief Exports a Codac scalar expression to SymPy.
       *
       * \param e Codac scalar expression.
       * \return Corresponding SymPy expression.
       */
      pybind11::object export_scalar(const ScalarExpr& e) const;

    private:

      pybind11::object export_node(const std::shared_ptr<ExprBase>& e) const;
      pybind11::object export_vector_component(const std::shared_ptr<ExprBase>& e, Index i) const;
      pybind11::object export_matrix_component(const std::shared_ptr<ExprBase>& e, Index i, Index j) const;
      static double scalar_const_value(const ConstValueExpr<ScalarType>& c);
      static std::shared_ptr<ExprBase> child_at(const std::vector<std::shared_ptr<ExprBase>>& children, Index i);

      const FlatSymbolTable& _symbols; //!< Associated symbol table.
  };

  /**
   * \brief Imports SymPy scalar expressions into Codac scalar expressions.
   */
  class SympyImporter
  {
    public:

      /**
       * \brief Builds an importer associated with a symbol table.
       *
       * \param symbols Flat symbol table.
       */
      explicit SympyImporter(const FlatSymbolTable& symbols);

      /**
       * \brief Imports a SymPy scalar expression.
       *
       * \param obj SymPy expression handle.
       * \return Corresponding Codac scalar expression.
       */
      ScalarExpr import_scalar(const pybind11::handle& obj) const;

    private:

      ScalarExpr import_add(const pybind11::handle& obj) const;
      ScalarExpr import_mul(const pybind11::handle& obj) const;
      ScalarExpr import_pow(const pybind11::handle& obj) const;
      ScalarExpr import_function(const pybind11::handle& obj) const;

      const FlatSymbolTable& _symbols; //!< Associated symbol table.
  };

  /**
   * \brief SymPy transformation applied to an exported scalar expression.
   */
  using SympyTransform = std::function<pybind11::object(
    const pybind11::object&, const pybind11::object&, const FlatSymbolTable&)>;

  /**
   * \brief Context gathering the bridge objects needed for one scalar transformation.
   */
  class ScalarBridgeContext
  {
    public:

      /**
       * \brief Builds a scalar bridge context for a given function argument list.
       *
       * \param args Function argument list.
       */
      explicit ScalarBridgeContext(const FunctionArgsList& args);

      /**
       * \brief Returns the imported SymPy module.
       *
       * \return SymPy module.
       */
      const pybind11::object& sympy() const;

      /**
       * \brief Returns the associated flat symbol table.
       *
       * \return Flat symbol table.
       */
      const FlatSymbolTable& symbols() const;

      /**
       * \brief Exports a Codac scalar expression to SymPy.
       *
       * \param y Codac scalar expression.
       * \return Corresponding SymPy expression.
       */
      pybind11::object export_scalar(const ScalarExpr& y) const;

      /**
       * \brief Imports a SymPy scalar expression into Codac.
       *
       * \param obj SymPy expression handle.
       * \return Corresponding Codac scalar expression.
       */
      ScalarExpr import_scalar(const pybind11::handle& obj) const;

      /**
       * \brief Applies a SymPy transformation to a Codac scalar expression.
       *
       * \param y Codac scalar expression.
       * \param transform SymPy transformation.
       * \param do_expand Whether the transformed expression should be expanded.
       * \return Transformed Codac scalar expression.
       */
      ScalarExpr transform_scalar_expr(const ScalarExpr& y, const SympyTransform& transform, bool do_expand = true) const;

    private:
      
      FlatSymbolTable _symbols; //!< Flat symbol table.
      SympyExporter _exporter;  //!< Exporter.
      SympyImporter _importer;  //!< Importer.
  };

  /**
   * \brief Ensures that the embedded Python runtime is initialized.
   */
  void ensure_python_runtime();

  /**
   * \brief Imports a Python module.
   *
   * Imported modules are cached after the first call.
   *
   * \param module_name Python module name.
   * \return Imported Python module.
   */
  const pybind11::object& import_module(const char* module_name);

  /**
   * \brief Imports the SymPy module.
   *
   * \return SymPy module.
   */
  const pybind11::object& import_sympy();

  /**
   * \brief Imports the SymPy polyfuncs module.
   *
   * \return SymPy polyfuncs module.
   */
  const pybind11::object& import_polyfuncs();

  /**
   * \brief Imports the Python builtins module.
   *
   * \return Python builtins module.
   */
  const pybind11::object& import_builtins();

  /**
   * \brief Normalizes a SymPy expression before import into Codac.
   *
   * \param sympy SymPy module.
   * \param expr SymPy expression.
   * \param do_expand Whether the expression should be expanded.
   * \return Normalized SymPy expression.
   */
  pybind11::object normalize_sympy_expr(const pybind11::object& sympy, pybind11::object expr, bool do_expand = true);

  /**
   * \brief Applies a SymPy transformation to a Codac scalar expression.
   *
   * \param args Function argument list.
   * \param y Codac scalar expression.
   * \param transform SymPy transformation.
   * \param do_expand Whether the transformed expression should be expanded.
   * \return Transformed Codac scalar expression.
   */
  ScalarExpr transform_scalar_expr(
    const FunctionArgsList& args,
    const ScalarExpr& y,
    const SympyTransform& transform,
    bool do_expand = true);
}
