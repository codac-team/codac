/** 
 *  \file codac2_Subpaving.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <unordered_set>
#include <list>
#include <memory>
#include "codac2_assert.h"
#include "codac2_IntervalVector.h"
#include "codac2_Paving.h"
#include "codac2_template_tools.h"
#include "codac2_Figure2D.h"

namespace codac2
{
  template<typename P>
  class PavingNode;

  template<typename P>
  class Subpaving : public std::list<IntervalVector>
  {
    public:

      Subpaving(std::initializer_list<IntervalVector> l)
        : std::list<IntervalVector>(l)
      {
        assert_release(!this->empty());
      }

      Subpaving(const std::list<IntervalVector>& l)
        : std::list<IntervalVector>(l)
      {
        assert_release(!this->empty());
      }

      IntervalVector box() const
      {
        IntervalVector h = IntervalVector::empty(this->front().size());
        for(const auto& bi : *this)
          h |= bi;
        return h;
      }

      std::list<IntervalVector> boxes() const
      {
        return *this;
      }

      #if 0
      std::list<IntervalVector> contour(bool sort = false) const
      {
        if constexpr(std::is_same_v<PavingOut,P>)
          return contour(PavingOut::outer_complem, sort);

        else if constexpr(std::is_same_v<PavingInOut,P>)
          return contour(PavingInOut::outer_complem, sort);

        else
        {
          assert_release(false &&
            "cannot find a default complementary \"node value\" function for such Subaving class");
          return { };
        }
      }

      std::list<IntervalVector> contour(const P::NodeValue_& node_complementary_value = nullptr, bool sort = false) const
      {
        std::list<IntervalVector> l_bound;

        for(const auto& xi : *this)
        {
          auto x_boxes = _node_value(xi);
          auto neighb_nodes = this->front()->paving().neighbours(xi, _node_value, node_complementary_value);
          std::list<IntervalVector> neighb_boxes;

          for(const auto& neighb_ni : neighb_nodes)
            neighb_boxes.splice(neighb_boxes.end(), node_complementary_value(neighb_ni));

          for(const auto& x_bi : x_boxes)
            for(const auto& neighb_bi : neighb_boxes)
            {
              auto inter = x_bi & neighb_bi;
              if(!inter.is_empty() && !inter.is_degenerated())
                l_bound.push_back(inter);
            }
        }

        // Removing duplicates
        remove_duplicates_from_list(l_bound);

        assert([&]() -> bool { for(const auto& bi : l_bound) { if(!bi.is_flat()) return false; } return true; } ()
          && "boundary boxes should be flat");
        return sort ? sort_contour(l_bound) : l_bound;
      }

      static Vector next_pt(const IntervalVector& x, const Vector& pt)
      {
        assert(!x.is_degenerated());
        assert(x.size() == 2 && pt.size() == 2);
        assert(x.is_flat());
        assert(x.lb() == pt || x.ub() == pt);
        return pt == x.lb() ? x.ub() : x.lb();
      }

      std::list<IntervalVector> sort_contour(std::list<IntervalVector> l) const
      {
        assert_release(!l.empty());
        assert_release(l.front().size() == 2 && "only 2d contours can be sorted");

        const size_t nl = l.size();

        Vector current_pt = l.front().ub(), first_pt = current_pt;
        std::list<IntervalVector> s { l.front() };
        l.pop_front();

        while(!l.empty())
        {
          std::list<IntervalVector>::iterator it = l.begin();
          while(it != l.end())
          {
            if(it->contains(current_pt))
            {
              s.push_back(*it);
              current_pt = Subpaving<P>::next_pt(*it, current_pt);
              l.erase(it);
              break;
            }

            else
              ++it;
          }

          if(current_pt == first_pt)
            break;
        }

        assert(nl == s.size());
        return s;
      }
    #endif
  };

  using SubpavingOut = Subpaving<PavingOut>;
  using SubpavingInOut = Subpaving<PavingInOut>;
}