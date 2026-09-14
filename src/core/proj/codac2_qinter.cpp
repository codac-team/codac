/** 
 *  codac2_qinter.cp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Clement Carbonnel, Benoit Desrochers, Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <type_traits>
#include "codac2_qinter.h"

using namespace std;

namespace codac2
{
  enum ProjBound { LEFT = 0, RIGHT = 1 };
  struct bound {
    ProjBound side;
    double val;
    double mark;
    explicit bound(double v = 0.0, ProjBound s = ProjBound::LEFT, double m = 1.0) : side(s), val(v), mark(m) {}
    friend bool operator<(const bound &x, const bound &y) 
    {
      if (x.val == y.val)
        if (x.side == y.side)
          return x.mark < y.mark;
        else
          return x.side < y.side;
      else 
        return x.val < y.val; 
    }
  };

  IntervalVector qinter(unsigned int q, const std::list<IntervalVector>& l)
  {
    assert(!l.empty());
    Index n = l.begin()->size();

    assert(([&l,n](){
      for(const auto& xi : l) {
        if(n != xi.size()) {
          return false;
        }
      }
      return true;
    }()));

    if(q > l.size())
      return IntervalVector::empty(n);

    // q is the maximum number of boxes that may be violated.
    // Hence, at least l.size()-q boxes must contain the point.
    const size_t min_satisfied = l.size()-q;

    if(min_satisfied == 0)
      return IntervalVector(n);

    unsigned int p = 0;
    for(const auto& li : l)
      if(!li.is_empty())
        p++;

    // Empty boxes cannot be satisfied.
    if(min_satisfied > p)
      return IntervalVector::empty(n);

    IntervalVector res(n);
    std::vector<bound> b(2*p);

    // Main loop: solve the q-inter independently on each dimension, and return the Cartesian product
    for(Index i = 0 ; i < n ; i++)
    {
      // Solve the q-inter for dimension i
      
      int j = 0;
      for (const auto& xj : l)
      {
        if(xj.is_empty())
          continue;
        b[2*j]   = bound( xj[i].lb(), ProjBound::LEFT  );
        b[2*j+1] = bound( xj[i].ub(), ProjBound::RIGHT );
        j++;
      }
      
      sort(b.begin(), b.end()); // lexicographic order (double first, then ProjBound)

            // Find the left bound
      int c = 0;
      double lb0 = oo, rb0 = 0;
      for(unsigned int k = 0 ; k < 2*p ; k++)
      {
        (b[k].second == ProjBound::LEFT) ? c++ : c--;
        if(c == static_cast<int>(min_satisfied))
        {
          lb0 = b[k].first;
          break;
        }
      }
      
      if(lb0 == oo)
      {
        res.set_empty();
        break;
      }
      
      // Find the right bound
      c = 0;
      for(int k = 2*p-1 ; k >= 0 ; k--)
      {
        (b[k].second == ProjBound::RIGHT) ? c++ : c--;
        if(c == static_cast<int>(min_satisfied))
        {
          rb0 = b[k].first;
          break;
        }
      }
      
      res[i] = { lb0,rb0 };
    }
    
    return res;
  }
  
  IntervalVector fuzzyinter(double alpha, const std::list<IntervalVector>& l, std::vector<double> marks)
  {
    assert(!l.empty());
    assert (marks.size() == l.size());
    Index n = l.begin()->size();

    assert(([&l,n](){
      for(const auto& xi : l) {
        if(n != xi.size()) {
          return false;
        }
      }
      return true;
    }()));

    unsigned int p = 0;
    for(const auto& li : l)
      if(!li.is_empty())
        p++;

    IntervalVector res(n);
    std::vector<bound> b(2*p);

    // Main loop: solve the q-relaxed intersection independently on each dimension,
    // and return the Cartesian product
    for(Index i = 0 ; i < n ; i++)
    {
      // Solve the q-relaxed intersection for dimension i
      
      int j = 0;
      int k = -1;
      for (const auto& xj : l)
      {
        k++;
        if(xj.is_empty())
          continue;
        b[2*j]   = bound( xj[i].lb(), ProjBound::LEFT , marks[k] );
        b[2*j+1] = bound( xj[i].ub(), ProjBound::RIGHT, marks[k] );
        j++;
      }
      
      sort(b.begin(), b.end()); // lexicographic order (double first, then ProjBound)

      // Find the left bound
      int c = 0;
      double lb0 = oo, rb0 = 0;
      for(unsigned int k = 0 ; k < 2*p ; k++)
      {
        (b[k].side == ProjBound::LEFT) ? c += b[k].mark : c -= b[k].mark;
        if(c >= alpha)
        {
          lb0 = b[k].val;
          break;
        }
      }
      
      if(lb0 == oo)
      {
        res.set_empty();
        break;
      }
      
      // Find the right bound
      c = 0;
      for(int k = 2*p-1 ; k >= 0 ; k--)
      {
        (b[k].side == ProjBound::RIGHT) ? c += b[k].mark : c -= b[k].mark;
        if(c >= alpha)
        {
          rb0 = b[k].val;
          break;
        }
      }
      
      res[i] = { lb0,rb0 };
    }
    
    return res;
  }
}
