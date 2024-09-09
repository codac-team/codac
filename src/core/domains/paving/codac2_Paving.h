/** 
 *  \file codac2_Paving.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include "codac2_IntervalVector.h"
#include "codac2_Domain.h"

namespace codac2
{
  template<typename... X>
    requires (std::is_same_v<X,IntervalVector> && ...)
  class Paving;
  
  template<typename P>
  class PavingNode;

  template<typename P>
  class PavingNode : public std::enable_shared_from_this<PavingNode<P>>
  {
    public:

      explicit PavingNode(P& paving, const IntervalVector& x, std::shared_ptr<PavingNode<P>> top = nullptr)
        : _paving(paving), _x(P::init_tuple(x)), _top(top)
      { }

      const typename P::PavingNodeTuple& boxes() const
      {
        return _x;
      }

      typename P::PavingNodeTuple& boxes()
      {
        return const_cast<typename P::PavingNodeTuple&>(const_cast<const PavingNode<P>*>(this)->boxes());
      }

      std::shared_ptr<const PavingNode<P>> top() const
      {
        return _top;
      }

      std::shared_ptr<PavingNode<P>> top()
      {
        return std::const_pointer_cast<PavingNode<P>>(const_cast<const PavingNode<P>*>(this)->top());
      }

      std::shared_ptr<const PavingNode<P>> left() const
      {
        return _left;
      }

      std::shared_ptr<PavingNode<P>> left()
      {
        return std::const_pointer_cast<PavingNode<P>>(const_cast<const PavingNode<P>*>(this)->left());
      }

      std::shared_ptr<const PavingNode<P>> right() const
      {
        return _right;
      }

      std::shared_ptr<PavingNode<P>> right()
      {
        return std::const_pointer_cast<PavingNode<P>>(const_cast<const PavingNode<P>*>(this)->right());
      }

      void visit(std::function<void(const PavingNode<P>&)> visitor) const
      {
        visitor(*this);
        if(_left) left()->visit(visitor);
        if(_right) right()->visit(visitor);
      }

      void visit(std::function<void(PavingNode<P>&)> visitor)
      {
        visitor(*this);
        if(_left) left()->visit(visitor);
        if(_right) right()->visit(visitor);
      }

      bool is_leaf() const
      {
        return not _left && not _right;
      }

      void bisect()
      {
        assert_release(is_leaf() && "only leaves can be bisected");
        bool bisectable_node = true;
        std::apply([&](auto &&... xs) { ((bisectable_node &= xs.is_bisectable()), ...); }, _x);
        assert_release(bisectable_node);

        IntervalVector x(std::get<0>(_x).size());
        std::apply([&](auto &&... xs) { ((x &= xs), ...); }, _x);
        auto p = x.bisect_largest();

        _left = make_shared<PavingNode<P>>(_paving, p.first, this->shared_from_this());
        _right = make_shared<PavingNode<P>>(_paving, p.second, this->shared_from_this());
      }

    protected:

      P& _paving;
      typename P::PavingNodeTuple _x;
      std::shared_ptr<PavingNode<P>> _top = nullptr;
      std::shared_ptr<PavingNode<P>> _left = nullptr, _right = nullptr;
  };

  template<typename... X>
    requires (std::is_same_v<X,IntervalVector> && ...)
  class Paving : public Domain
  {
    public:

      using PavingNodeTuple = std::tuple<X...>;

      Paving(size_t n)
        : Paving(IntervalVector(n))
      {
        assert_release(n > 0);
      }

      Paving(const IntervalVector& x)
        : _tree(std::make_shared<PavingNode<Paving<X...>>>(*this, x))
      { }

      size_t size() const
      {
        return std::get<0>(_tree->boxes()).size();
      }

      std::shared_ptr<const PavingNode<Paving<X...>>> tree() const
      {
        return _tree;
      }

      std::shared_ptr<PavingNode<Paving<X...>>> tree()
      {
        return std::const_pointer_cast<PavingNode<Paving<X...>>>(const_cast<const Paving<X...>*>(this)->tree());
      }

    protected:

      friend class PavingNode<Paving<X...>>;

      static PavingNodeTuple init_tuple(const IntervalVector& x)
      {
        return std::make_tuple(((X)x)...);
      }

      std::shared_ptr<PavingNode<Paving<X...>>> _tree; // must be a shared_ptr to allow enable_shared_from_this
  };
}