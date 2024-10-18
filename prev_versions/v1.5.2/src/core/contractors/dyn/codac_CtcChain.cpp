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
  CtcChain::CtcChain()
    : CtcLinobs(Matrix({{0,1},{0,0}}), Vector({0,1}))
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

    CtcLinobs::contract(v_domains[0]->tube(), v_domains[1]->tube(), v_domains[2]->tube());
  }
  
  void CtcChain::contract(Tube& x, Tube& v, const Tube& a, TimePropag t_propa)
  {
    CtcLinobs::contract(x, v, a, t_propa);
  }

  /*void CtcChain::contract(codac2::Tube<IntervalVector>& x, const codac2::Tube<Interval>& a, TimePropag t_propa)
  {
    assert(x.size() == 2 && codac2::TDomain::are_same(x.tdomain(),a.tdomain()));
    codac::TubeVector _x = to_codac1(x);
    codac::Tube _a = to_codac1(a);
    contract(_x[0], _x[1], _a, t_propa);
    x &= codac2::to_codac2(_x);
  }

  void CtcChain::contract(codac2::Tube<Interval>& x, codac2::Tube<Interval>& v, const codac2::Tube<Interval>& a, TimePropag t_propa)
  {
    assert(codac2::TDomain::are_same(x.tdomain(),v.tdomain()) && codac2::TDomain::are_same(x.tdomain(),a.tdomain()));
    codac::Tube _x = to_codac1(x);
    codac::Tube _v = to_codac1(v);
    codac::Tube _a = to_codac1(a);
    contract(_x, _v, _a, t_propa);
    x &= codac2::to_codac2(_x);
    v &= codac2::to_codac2(_v);
  }*/
}