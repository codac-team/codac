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
  template<int N=Dynamic>
  class Paving;

  template<int N=Dynamic>
  class PavingNode : public std::enable_shared_from_this<PavingNode<N>>
  {
    public:

      explicit PavingNode(Paving<N>& paving, const IntervalVector_<N>& x)
       : _paving(paving), _x(x)
      { }

      virtual ~PavingNode() = default;

      const IntervalVector_<N>& box() const
      {
        return _x;
      }

      std::shared_ptr<PavingNode<N>> left()
      {
        return _left;
      }

      std::shared_ptr<PavingNode<N>> right()
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

        _left = std::make_shared<PavingNode<N>>(_paving, p.first);
        _right = std::make_shared<PavingNode<N>>(_paving, p.second);

        auto wp = this->weak_from_this();
        _paving._leaves.remove_if([wp](std::weak_ptr<PavingNode<N>> p)
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
      friend class Paving;

      void recurse__boxes_list(std::list<std::reference_wrapper<const IntervalVector_<N>>>& l, const IntervalVector_<N>& intersect = IntervalVector_<N>()) const
      {
        if(is_leaf() && !_x.is_empty() && _x.intersects(intersect))
          l.push_back(std::cref(_x));
        else
        {
          if(_left) _left->recurse__boxes_list(l);
          if(_right) _right->recurse__boxes_list(l);
        }
      }

    public: // todo

      IntervalVector_<N> _x;
      //std::weak_ptr<PavingNode<N>> _parent = nullptr;
      std::shared_ptr<PavingNode<N>> _left = nullptr, _right = nullptr;
      Paving<N>& _paving;
  };

  template<int N>
  class Paving : public Domain
  {
    public:

      explicit Paving(size_t n)
       : Paving<N>(IntervalVector_<N>(n))
      { }

      explicit Paving(const IntervalVector_<N>& x)
       : _tree(std::make_shared<PavingNode<N>>(*this, IntervalVector_<N>(x)))
      {
        _leaves.push_back(_tree->weak_from_this());
      }

      Paving(const Paving<N>&) = delete;
      Paving& operator=(const Paving<N>&) = delete;

      size_t nb_leaves() const
      {
        return _leaves.size();
      }

      IntervalVector_<N> hull_box() const
      {
        IntervalVector_<N> hull = IntervalVector_<N>::empty_set();
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
      
      DomainVolume dom_volume() const
      {
        DomainVolume vol;
        for(const auto& l : _leaves)
          vol.push_back(l.lock()->box().volume());
        return vol;
      }

      std::list<std::weak_ptr<PavingNode<N>>>& leaves()
      {
        return _leaves;
      }

      std::list<std::reference_wrapper<const IntervalVector_<N>>> boxes_list(const IntervalVector_<N>& intersect = IntervalVector_<N>()) const
      {
        std::list<std::reference_wrapper<const IntervalVector_<N>>> l;
        _tree->recurse__boxes_list(l, intersect);
        return l;
      }

      template<int N_>
      friend class PavingNode;
      
      template<int N_>
      friend std::ostream& operator<<(std::ostream& os, const Paving<N_>& p);


    public: // todo

      using leaves_container = std::list<std::weak_ptr<PavingNode<N>>>;
      leaves_container _leaves;
      std::shared_ptr<PavingNode<N>> _tree; // must be a shared_ptr to allow enable_shared_from_this

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

  template<int N_>
  inline std::ostream& operator<<(std::ostream& os, const Paving<N_>& p)
  {
    size_t n = p.nb_leaves();
    os << "Paving (" << n << " box" << (n > 1 ? "es)" : ")") << std::flush;
    return os;
  }


} // namespace codac

#endif