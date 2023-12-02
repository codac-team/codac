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
  template<class P,int N=Dynamic>
  class PavingBase : virtual public Domain
  {
    public:

      explicit PavingBase(const IntervalVector_<N>& x)
       : _x(x)
      {

      }

      virtual ~PavingBase() = default;

      const IntervalVector_<N>& box() const
      {
        return _x;
      }

      std::shared_ptr<P> left()
      {
        return _left;
      }

      std::shared_ptr<P> right()
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

      double volume() const
      {
        if(is_leaf())
          return _x.volume();
        double v = 0.;
        if(_left) v += _left->volume();
        if(_right) v += _right->volume();
        return v;
      }

      virtual void bisect(float ratio = 0.49)
      {
        assert(Interval(0.,1.).interior_contains(ratio));
        assert(is_leaf() && "only leaves can be bisected");
        assert(_x.is_bisectable());
        auto p = _x.bisect(ratio);
        _left = std::make_shared<P>(p.first);
        _right = std::make_shared<P>(p.second);
      }

      IntervalVector_<N> hull_box() const
      {
        if(is_leaf())
          return _x;
        auto hull = IntervalVector_<N>::empty_set();
        if(_left) hull |= _left->hull_box();
        if(_right) hull |= _right->hull_box();
        return hull;
      }

      std::list<std::reference_wrapper<const IntervalVector_<N>>> boxes_list(const IntervalVector_<N>& intersect = IntervalVector_<N>()) const
      {
        std::list<std::reference_wrapper<const IntervalVector_<N>>> l;
        boxes_list_push(l, intersect);
        return l;
      }

      std::list<P*> leaves_list()
      {
        std::list<P*> l;
        leaves_list_push(l);
        return l;
      }

      size_t nb_leaves() const
      {
        if(is_leaf())
          return 1;
        else
          return (_left ? _left->nb_leaves() : 0) +
            (_right ? _right->nb_leaves() : 0);
      }
      
      DomainVolume dom_volume() const
      {
        DomainVolume vol;
        dom_volume_push(vol);
        return vol;
      }
      
      template<class P_,int N_>
      friend std::ostream& operator<<(std::ostream& os, const PavingBase<P_,N_>& p);

    protected:

      void boxes_list_push(std::list<std::reference_wrapper<const IntervalVector_<N>>>& l, const IntervalVector_<N>& intersect = IntervalVector_<N>()) const
      {
        if(is_leaf() && !_x.is_empty() && _x.intersects(intersect))
          l.push_back(std::cref(_x));
        else
        {
          if(_left) _left->boxes_list_push(l);
          if(_right) _right->boxes_list_push(l);
        }
      }

      void leaves_list_push(std::list<P*>& l)
      {
        if(is_leaf() && !_x.is_empty())
          l.push_back(dynamic_cast<P*>(this));
        else
        {
          if(_left) _left->leaves_list_push(l);
          if(_right) _right->leaves_list_push(l);
        }
      }

      void dom_volume_push(DomainVolume& vol) const
      {
        if(is_leaf())
          vol.push_back(_x.volume());
        else
        {
          if(_left) _left->dom_volume_push(vol);
          if(_right) _right->dom_volume_push(vol);
        }
      }

    public: // todo

      IntervalVector_<N> _x;
      std::shared_ptr<P> _left = nullptr, _right = nullptr;
  };

  template<class P_,int N_>
  inline std::ostream& operator<<(std::ostream& os, const PavingBase<P_,N_>& p)
  {
    os << "Paving";
    return os;
  }

  template<int N>
  class Paving : public PavingBase<Paving<N>,N>
  {
    public:

      explicit Paving(size_t n)
       : PavingBase<Paving<N>,N>(IntervalVector_<N>(n))
      { }

      explicit Paving(const IntervalVector_<N>& x)
       : PavingBase<Paving<N>,N>(x)
      { }
  };


} // namespace codac

#endif