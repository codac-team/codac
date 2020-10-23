/** 
 *  CtcChain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcChain.h"
#include "tubex_Domain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  IntervalMatrix exp_At(const Matrix& A, const Interval& t)
  {
    assert(A.nb_rows() == 2 && A.nb_cols() == 2);
    return IntervalMatrix({{1,t},{0,1}});
  }

  CtcChain::CtcChain()
    : CtcLinobs(Matrix({{0,1},{0,0}}), Vector({0,1}), &exp_At)
  {

  }

  void CtcChain::contract(vector<Domain*>& v_domains)
  {
    assert(v_domains.size() == 3);
    assert(v_domains[0]->type() == Domain::Type::T_TUBE
        && v_domains[1]->type() == Domain::Type::T_TUBE
        && v_domains[2]->type() == Domain::Type::T_TUBE);

    TubeVector x({ // todo: improve this
      v_domains[0]->tube(),
      v_domains[1]->tube()
    });

    CtcLinobs::contract(x, v_domains[2]->tube());

    // todo: improve this
    v_domains[0]->tube() &= x[0];
    v_domains[1]->tube() &= x[1];
  }
  
  void CtcChain::contract(Tube& x, Tube& v, const Tube& a, TimePropag t_propa)
  {
    // todo: improve this
    TubeVector vec({ x, v });

    CtcLinobs::contract(vec, a, t_propa);

    // todo: improve this
    x &= vec[0];
    v &= vec[1];
  }
}