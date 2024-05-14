/** 
 *  Paver classes
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_Paver.h"

using namespace std;
using namespace codac2;

Paver::Paver(const IntervalVector& x0)
  : _x0(x0)
{ }

void Paver::pave(const Ctc_<IntervalVector>& c, double eps)
{
  clock_t t_start = clock();

  if(_figure)
    _figure->draw_box(_x0,"#9C9C9C[cyan]");

  list<IntervalVector> l { _x0 };
  size_t n = 0;

  while(!l.empty())
  {
    IntervalVector x = l.front();
    l.pop_front();

    c.contract(x);

    if(!x.is_empty())
    {
      if(x.max_diam() < eps)
      {
        n++;
        if(_figure)
    			_figure->draw_box(x,"#9C9C9C[#FFDB0055]");
      }

      else
      {
        auto p = x.bisect_largest();
        l.push_back(p.first); l.push_back(p.second);
      }
    }
  }
  
  printf("Computation time: %.4fs, %zd boxes\n", (double)(clock()-t_start)/CLOCKS_PER_SEC, n);
}