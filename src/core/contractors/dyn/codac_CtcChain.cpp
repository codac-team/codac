/** 
 *  CtcChain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcChain.h"
#include "codac_Domain.h"
#include "codac_DomainsTypeException.h"

using namespace std;
using namespace ibex;

namespace codac
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

  // Static members for contractor signature (mainly used for CN Exceptions)
  const string CtcChain::m_ctc_name = "CtcChain";
  vector<string> CtcChain::m_str_expected_doms(
  {
    "Tube, Tube, Tube"
  });

  void CtcChain::contract(vector<Domain*>& v_domains)
  {
    if(v_domains.size() != 3
      || v_domains[0]->type() != Domain::Type::T_TUBE
      || v_domains[1]->type() != Domain::Type::T_TUBE
      || v_domains[2]->type() != Domain::Type::T_TUBE)
      throw DomainsTypeException(m_ctc_name, v_domains, m_str_expected_doms);

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