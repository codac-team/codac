/** 
 *  \file codac2_PavingNode.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>
#include "codac2_Paving.h"
#include "codac2_IntervalVector.h"

namespace codac2
{
  template<typename P>
  class PavingNode : public std::enable_shared_from_this<PavingNode<P>>
  {
    public:

      explicit PavingNode(const P& paving, const IntervalVector& x, std::shared_ptr<PavingNode<P>> top = nullptr)
        : _paving(paving), _x(P::init_tuple(x)), _top(top)
      { }

      const P& paving() const
      {
        return _paving;
      }

      const typename P::NodeTuple_& boxes() const
      {
        return _x;
      }

      typename P::NodeTuple_& boxes()
      {
        return const_cast<typename P::NodeTuple_&>(const_cast<const PavingNode<P>*>(this)->boxes());
      }

      void set_boxes(const typename P::NodeTuple_& x)
      {
        _x = x;
      }

      IntervalVector hull() const
      {
        IntervalVector h = std::get<0>(_x); 
        std::apply([&](auto &&... xs) { ((h |= xs), ...); }, _x);
        return h;
      }

      IntervalVector unknown() const
      {
        IntervalVector h = std::get<0>(_x); 
        std::apply([&](auto &&... xs) { ((h &= xs), ...); }, _x);
        return h;
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

      // === Comment related to the following visit() methods:
      // In some representations, the first level of the tree already holds a contraction
      // and it is no longer possible to reconstruct the original hull box. This is the case
      // for pavings built from contractors. Conversely, for those built from separators, the union
      // of the separated boxes of the root allows to reconstruct the original hull box.
      // Due to the first case, the classical tree structure is changed by duplicating the first
      // level: the box of the root is the original one and there is only one (left) child
      // holding the contracted box (the right child is disabled).
      // Therefore, during the visiting process, the first level is ignored if it is redundant with
      // the second one, as it is the case for pavings built from separators.

      void visit(std::function<bool(std::shared_ptr<const PavingNode<P>>)> visitor) const
      {
        if(!_top && !_right && _left && left()->boxes() == _x)
          left()->visit(visitor);

        else if(visitor(this->shared_from_this()))
        {
          if(_left) left()->visit(visitor);
          if(_right) right()->visit(visitor);
        }
      }

      void visit(std::function<bool(std::shared_ptr<PavingNode<P>>)> visitor)
      {
        if(!_top && !_right && _left && left()->boxes() == _x)
          _left->visit(visitor);

        else if(visitor(this->shared_from_this()))
        {
          if(_left) left()->visit(visitor);
          if(_right) right()->visit(visitor);
        }
      }

      bool is_leaf() const
      {
        return not _left && not _right;
      }

      void bisect()
      {
        bisect([](const IntervalVector& x) { return x.bisect_largest(); });
      }

      void bisect(std::function<std::pair<IntervalVector,IntervalVector>(const IntervalVector&)> bisect_fnc)
      {
        assert_release(is_leaf() && "only leaves can be bisected");
        
        bool bisectable_node = true;
        std::apply([&](auto &&... xs) { ((bisectable_node &= xs.is_bisectable()), ...); }, _x);
        assert_release(bisectable_node);

        auto p = bisect_fnc(unknown());

        _left = make_shared<PavingNode<P>>(_paving, p.first, this->shared_from_this());
        _right = make_shared<PavingNode<P>>(_paving, p.second, this->shared_from_this());
      }

      std::vector<IntervalVector> complementary_value(const typename P::NodeValue_& node_value) const
      {
        return hull().diff(node_value(_x));
      }

    protected:

      const P& _paving;
      typename P::NodeTuple_ _x;
      std::shared_ptr<PavingNode<P>> _top = nullptr;
      std::shared_ptr<PavingNode<P>> _left = nullptr, _right = nullptr;
  };
}