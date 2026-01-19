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

  IntervalVector qinter(unsigned int q, const std::list<IntervalVector>& l)
  {
    assert(!l.empty());
    Index n = l.begin()->size();
    
    if(q > l.size())
      return IntervalVector::empty(n);

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
    std::vector<std::pair<double,ProjBound>> b(2*p);

    // Main loop: solve the q-inter independently on each dimension, and return the Cartesian product
    for(Index i = 0 ; i < n ; i++)
    {
      // Solve the q-inter for dimension i
      
      int j = 0;
      for(const auto& xj : l)
      {
        if(xj.is_empty())
          continue;
        b[2*j]   = { xj[i].lb(), ProjBound::LEFT };
        b[2*j+1] = { xj[i].ub(), ProjBound::RIGHT };
        j++;
      }
      
      sort(b.begin(), b.end()); // lexicographic order (double first, then ProjBound)

      // Find the left bound
      int c = 0;
      double lb0 = oo, rb0 = 0;
      for(unsigned int k = 0 ; k < 2*p ; k++)
      {
        (b[k].second == ProjBound::LEFT) ? c++ : c--;
        if(c == (int)q)
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
        if(c == (int)q)
        {
          rb0 = b[k].first;
          break;
        }
      }
      
      res[i] = { lb0,rb0 };
    }
    
    return res;
  }
}