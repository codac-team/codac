/** 
 *  \file codac2_analytic_flat_input_layout.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_analytic_flat_input_layout.h"

#include "codac2_assert.h"
#include "codac2_analytic_variables.h"
#include "codac2_component.h"

namespace codac2
{
  bool FlatInputBinding::is_scalar() const
  {
    return type == std::type_index(typeid(ScalarType));
  }

  bool FlatInputBinding::is_vector() const
  {
    return type == std::type_index(typeid(VectorType));
  }

  bool FlatInputBinding::is_matrix() const
  {
    return type == std::type_index(typeid(MatrixType));
  }

  FlatInputLayout::FlatInputLayout(const FunctionArgsList& args)
  {
    Index flat = 0;

    for(const auto& arg : args)
    {
      if(std::dynamic_pointer_cast<ScalarVar>(arg))
      {
        _bindings.emplace(arg->unique_id().id(), FlatInputBinding{typeid(ScalarType), flat, 1, 1});
        ++flat;
      }

      else if(auto v = std::dynamic_pointer_cast<VectorVar>(arg))
      {
        _bindings.emplace(arg->unique_id().id(), FlatInputBinding{typeid(VectorType), flat, v->size(), 1});
        flat += v->size();
      }

      else if(auto m = std::dynamic_pointer_cast<MatrixVar>(arg))
      {
        _bindings.emplace(arg->unique_id().id(), FlatInputBinding{typeid(MatrixType), flat, m->rows(), m->cols()});
        flat += m->rows() * m->cols();
      }

      else
        assert_release(false && "FlatInputLayout: unsupported variable type in function argument list");
    }

    _size = flat;
  }

  Index FlatInputLayout::size() const
  {
    return _size;
  }

  bool FlatInputLayout::same_domain_as(const FlatInputLayout& other) const
  {
    return size() == other.size();
  }

  bool FlatInputLayout::same_domain_as(const FunctionArgsList& other_args) const
  {
    return same_domain_as(FlatInputLayout(other_args));
  }

  Index FlatInputLayout::flat_index_of(const ScalarVar& x) const
  {
    const auto& b = binding_of(x.unique_id());
    assert_release(b.is_scalar() && "FlatInputLayout::flat_index_of: expected a scalar input variable");
    return b.offset;
  }

  Index FlatInputLayout::flat_index_of(const VectorVar& x, Index i) const
  {
    const auto& b = binding_of(x.unique_id());
    assert_release(b.is_vector() && "FlatInputLayout::flat_index_of: expected a vector input variable");
    assert_release(i >= 0 && i < b.rows && "FlatInputLayout::flat_index_of: vector component out of bounds");
    return b.offset + i;
  }

  Index FlatInputLayout::flat_index_of(const MatrixVar& x, Index i, Index j) const
  {
    const auto& b = binding_of(x.unique_id());
    assert_release(b.is_matrix() && "FlatInputLayout::flat_index_of: expected a matrix input variable");
    assert_release(i >= 0 && i < b.rows && j >= 0 && j < b.cols
      && "FlatInputLayout::flat_index_of: matrix component out of bounds");
    return b.offset + b.cols*i + j;
  }

  bool FlatInputLayout::flat_index_of(const ScalarExpr& x, Index& flat_index) const
  {
    if(auto s = std::dynamic_pointer_cast<ScalarVar>(x))
    {
      const auto* b = find_binding(s->unique_id());
      if(!b || !b->is_scalar())
        return false;

      flat_index = b->offset;
      return true;
    }

    if(auto cv = std::dynamic_pointer_cast<AnalyticOperationExpr<ComponentOp,ScalarType,VectorType>>(x))
    {
      const auto children = cv->children_expr_base();
      if(children.size() != 1)
        return false;

      auto v = std::dynamic_pointer_cast<VectorVar>(children[0]);
      if(!v)
        return false;

      const auto* b = find_binding(v->unique_id());
      if(!b || !b->is_vector())
        return false;

      if(cv->i() < 0 || cv->i() >= b->rows)
        return false;

      flat_index = b->offset + cv->i();
      return true;
    }

    if(auto cm = std::dynamic_pointer_cast<AnalyticOperationExpr<ComponentOp,ScalarType,MatrixType>>(x))
    {
      const auto children = cm->children_expr_base();
      if(children.size() != 1)
        return false;

      auto m = std::dynamic_pointer_cast<MatrixVar>(children[0]);
      if(!m)
        return false;

      const auto* b = find_binding(m->unique_id());
      if(!b || !b->is_matrix())
        return false;

      if(cm->i() < 0 || cm->i() >= b->rows || cm->j() < 0 || cm->j() >= b->cols)
        return false;

      flat_index = b->offset + b->cols*cm->i() + cm->j();
      return true;
    }

    return false;
  }

  const FlatInputBinding& FlatInputLayout::binding_of(const ExprID& id) const
  {
    const auto* b = find_binding(id);
    assert_release(b != nullptr && "FlatInputLayout::binding_of: unknown input expression identifier");
    return *b;
  }

  const FlatInputBinding* FlatInputLayout::find_binding(const ExprID& id) const
  {
    auto it = _bindings.find(id.id());
    return it == _bindings.end() ? nullptr : &it->second;
  }
}
