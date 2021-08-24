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
}