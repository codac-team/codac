/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_PAVING_H__
#define __CODAC2_PAVING_H__

#include <list>
#include <memory>
#include <functional>
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include <codac2_Domain.h>

namespace codac2
{
  template<int N>
  class Paving_;

  template<int N=Dynamic>
  class PavingNode_ : public std::enable_shared_from_this<PavingNode_<N>>
  {
    public:

      explicit PavingNode_(Paving_<N>& paving, const IntervalVector_<N>& x)
       : _paving(paving), _x(x)
      { }

      virtual ~PavingNode_() = default;

      const IntervalVector_<N>& box() const
      {
        return _x;
      }

      IntervalVector_<N>& box()
      {
        return _x;
      }

      std::shared_ptr<PavingNode_<N>> left()
      {
        return _left;
      }

      std::shared_ptr<PavingNode_<N>> right()
      {
        return _right;
      }

      bool is_empty() const
      {
        if(is_leaf())
          return _x.is_empty();

        else
          return (_left && _left->is_empty()) && (_right && _right->is_empty());
      }

      bool is_leaf() const
      {
        return not _left && not _right;
      }

      void bisect(float ratio = 0.49)
      {
        assert(Interval(0.,1.).interior_contains(ratio));
        assert(is_leaf() && "only leaves can be bisected");
        assert(_x.is_bisectable());
        auto p = _x.bisect(ratio);

        _left = std::make_shared<PavingNode_<N>>(_paving, p.first);
        _right = std::make_shared<PavingNode_<N>>(_paving, p.second);

        auto wp = this->weak_from_this();
        _paving._leaves.remove_if([wp](std::weak_ptr<PavingNode_<N>> p)
        {
          auto swp = wp.lock(), sp = p.lock();
          if(swp && sp) return swp == sp;
          return false;
        });

        _paving._leaves.push_back(_left->weak_from_this());
        _paving._leaves.push_back(_right->weak_from_this());
      }

    protected:

      template<int N_>
      friend class Paving_;

      void recurse__boxes_list(std::list<std::reference_wrapper<const IntervalVector_<N>>>& l, const IntervalVector_<N>& intersect) const
      {
        if(is_leaf() && !_x.is_empty()/* && _x.intersects(intersect)*/) // todo
          l.push_back(std::cref(_x));
        else
        {
          if(_left) _left->recurse__boxes_list(l, intersect);
          if(_right) _right->recurse__boxes_list(l, intersect);
        }
      }

      void recurse__contract(IntervalVector_<N>& x)
      {
        if(is_leaf())
          x &= _x;

        else if(!x.intersects(_x))
          x.set_empty();

        else
        {
          IntervalVector_<N>& x_left(x), x_right(x);
          if(_left) _left->recurse__contract(x_left);
          if(_right) _right->recurse__contract(x_right);
          x = x_left | x_right;
        }
      }

    protected:

      Paving_<N>& _paving;
      IntervalVector_<N> _x;
      //std::weak_ptr<PavingNode_<N>> _parent = nullptr;
      std::shared_ptr<PavingNode_<N>> _left = nullptr, _right = nullptr;
  };

  template<int N=Dynamic>
  class Paving_ : public Domain
  {
    public:

      explicit Paving_(size_t n = N)
       : Paving_<N>(IntervalVector_<N>(n))
      {
        assert(n > 0);
      }

      explicit Paving_(const IntervalVector_<N>& x)
       : _tree(std::make_shared<PavingNode_<N>>(*this, x))
      {
        _leaves.push_back(_tree->weak_from_this());
      }

      Paving_(const Paving_<N>& x)
        : _leaves(x._leaves), _tree(x._tree)
      {

      }

      Paving_& operator=(const Paving_<N>& x)
      {
        _leaves = x._leaves;
        _tree = x._tree;
        return *this;
      }

      size_t size() const
      {
        return _tree->_x.size();
      }

      size_t nb_leaves() const
      {
        update_leaves();
        return _leaves.size();
      }

      IntervalVector_<N> hull_box() const
      {
        IntervalVector_<N> hull = IntervalVector_<N>::empty_set(size());
        for(const auto& l : _leaves)
          hull |= l.lock()->box();
        return hull;
      }

      bool is_empty() const
      {
        for(const auto& l : _leaves)
          if(!l.lock()->box()->is_empty())
            return false;
        return true;
      }

      double volume() const
      {
        double vol = 0.;
        for(const auto& l : _leaves)
          vol += l.lock()->box().volume();
        return vol;
      }

      void update_leaves() const
      {
        _leaves.remove_if([](std::weak_ptr<PavingNode_<N>> p)
        {
          auto sp = p.lock();
          return sp->box().is_empty();
        });
      }

      std::list<std::weak_ptr<PavingNode_<N>>>& leaves()
      {
        update_leaves();
        return _leaves;
      }

      std::list<std::reference_wrapper<const IntervalVector_<N>>> boxes_list() const
      {
        return boxes_list(IntervalVector_<N>(size()));
      }

      std::list<std::reference_wrapper<const IntervalVector_<N>>> boxes_list(const IntervalVector_<N>& intersect) const
      {
        std::list<std::reference_wrapper<const IntervalVector_<N>>> l;
        _tree->recurse__boxes_list(l, intersect);
        return l;
      }

      void contract(IntervalVector_<N>& x)
      {
        _tree->recurse__contract(x);
      }

      template<int N_>
      friend class PavingNode_;
      
      template<int N_>
      friend std::ostream& operator<<(std::ostream& os, const Paving_<N_>& p);


    public: // todo

      using leaves_container = std::list<std::weak_ptr<PavingNode_<N>>>;
      mutable leaves_container _leaves;
      std::shared_ptr<PavingNode_<N>> _tree; // must be a shared_ptr to allow enable_shared_from_this

    public:

      struct const_iterator : public leaves_container::const_iterator
      {
        public:
          
          const_iterator(typename leaves_container::const_iterator it) : 
            leaves_container::const_iterator(it) { }

          const IntervalVector_<N>& operator*() const
          {
            return (this->leaves_container::const_iterator::operator*()).lock()->box();
          }
      };

      auto begin() const { return const_iterator(_leaves.cbegin()); }
      auto end() const   { return const_iterator(_leaves.cend()); }
  };

  template<int N_=Dynamic>
  inline std::ostream& operator<<(std::ostream& os, const Paving_<N_>& p)
  {
    size_t n = p.nb_leaves();
    os << "Paving (" << n << " box" << (n > 1 ? "es" : "") << ", hull=" << p.hull_box() << ")" << std::flush;
    return os;
  }

  using Paving = Paving_<>;

} // namespace codac

#endif