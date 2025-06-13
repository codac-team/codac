/** 
 *  codac2_pave.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_pave.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  PavingOut pave(const IntervalVector& x, std::shared_ptr<const CtcBase<IntervalVector>> c,
    double eps, bool verbose)
  {
    return pave(x, *c, eps, verbose);
  }

  PavingOut pave(const IntervalVector& x, const CtcBase<IntervalVector>& c, double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x.is_empty());
    assert_release(c.size() >= 2 && "cannot reveal 1d contractors");
    
    clock_t t_start = clock();
    Index n_boundary = 0;

    PavingOut p(x);
    // In order to be able to reconstruct the initial box, the first level represents the
    // initial domain x (the left node is x, the right one is an empty box).
    p.tree()->bisect();
    p.tree()->left()->boxes() = { x };
    get<0>(p.tree()->right()->boxes()).set_empty();

    std::shared_ptr<PavingOut_Node> n;
    list<std::shared_ptr<PavingOut_Node>> l { p.tree()->left() };

    while(!l.empty())
    {
      n = l.front();
      l.pop_front();

      c.contract(get<0>(n->boxes()));

      if(!get<0>(n->boxes()).is_empty())
      {
        if(get<0>(n->boxes()).max_diam() > eps)
        {
          n->bisect();
          l.push_back(n->left());
          l.push_back(n->right());
        }
        
        else if(verbose)
          n_boundary++;
      }
    }    

    if(verbose)
      printf("Computation time: %.4fs, %ld boxes\n",
        (double)(clock()-t_start)/CLOCKS_PER_SEC, n_boundary);
    return p;
  }
  
  PavingInOut pave(const IntervalVector& x, std::shared_ptr<const SepBase> s,
    double eps, bool verbose)
  {
    return pave(x, *s, eps, verbose);
  }

  PavingInOut pave(const IntervalVector& x, const SepBase& s, double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x.is_empty());
    assert_release(s.size() >= 2 && "cannot reveal 1d separators");
    
    clock_t t_start = clock();

    PavingInOut p(x);
    std::shared_ptr<PavingInOut_Node> n;
    list<std::shared_ptr<PavingInOut_Node>> l { p.tree() };

    while(!l.empty())
    {
      n = l.front();
      l.pop_front();

      auto xs = s.separate(get<0>(n->boxes()));
      auto boundary = (xs.inner & xs.outer);
      n->boxes() = { xs.outer, xs.inner };

      if(!boundary.is_empty() && boundary.max_diam() > eps)
      {
        n->bisect();
        l.push_back(n->left());
        l.push_back(n->right());
      }
    }

    if(verbose)
      printf("Computation time: %.4fs\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    return p;
  }

  PavingInOut regular_pave(const IntervalVector& x,
    const std::function<BoolInterval(const IntervalVector&)>& test,
    double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x.is_empty());

    clock_t t_start = clock();

    PavingInOut p(x);
    std::list<std::shared_ptr<PavingInOut_Node>> l { p.tree() };

    while(!l.empty())
    {
      auto n = l.front();
      l.pop_front();

      assert(n->is_leaf());
      auto b = test(std::get<1>(n->boxes()));

      switch(b)
      {
        case BoolInterval::TRUE:
          std::get<1>(n->boxes()).set_empty();
          break;

        case BoolInterval::FALSE:
          std::get<0>(n->boxes()).set_empty();
          break;

        default:
          if(n->unknown().max_diam() > eps)
          {
            n->bisect();
            l.push_back(n->left());
            l.push_back(n->right());
          }
      }
    }

    if(verbose)
      printf("Computation time: %.4fs\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    return p;
  }
}