//
// Created by julien-damers on 19/11/2021.
//

#include "codac_CtcCN.h"

using namespace ibex;
using namespace std;

namespace codac
{
  CtcCN::CtcCN(ContractorNetwork& cn, IntervalVectorVar& box_var)
    : Ctc(box_var.size()), m_cn(cn), m_box_var(box_var)
  {

  }

  void CtcCN::contract(IntervalVector &x)
  {
    assert(x.size() == m_box_var.size() && "the box you are trying to contract does not match the size of the symbolic box");
    m_cn.reset_interm_vars();
    m_cn.contract({
      {m_box_var, x} // the "box" var is associated with "x"
                     // for this contraction procedure
    });
  }

}