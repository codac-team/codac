/** 
 *  Codac - Examples
 *  Comparisons of two distance contractors
 * ----------------------------------------------------------------------------
 *
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  // Comparisons of two distance contractors


  // Using manual forward / backward algorithm

  clock_t t_start = clock();

  for(int i = 0 ; i < 100000 ; i++)
  {
    IntervalVector x({{2,2.5},{0,0.5},{1.5,2.5},{4,11},{7,8}});
    ctc::dist.contract(x); // contractor from Codac
  }

  printf("[ Manual ctc   ] Time: %.3f\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);


  // Using automatic HC-Revise algorithm (IBEX)

  CtcFunction ctc_f(
      Function("a[2]", "b[2]", "d",
               "d - sqrt((a[0]-b[0])^2+(a[1]-b[1])^2)"));

  t_start = clock();

  for(int i = 0 ; i < 100000 ; i++)
  {
    IntervalVector x({{2,2.5},{0,0.5},{1.5,2.5},{4,11},{7,8}});
    ctc_f.contract(x);
  }

  printf("[ HC4 IBEX ctc ] Time: %.3f\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
}