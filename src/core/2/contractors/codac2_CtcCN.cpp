/** 
 *  CtcCN class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcCN.h"
#include "codac2_Var.h"

using namespace std;
using namespace codac2;

CtcCN::CtcCN(ContractorNetwork& cn, Var<IntervalVector>& var)
  : _cn(cn), _ref_var(&var)
{

}

void CtcCN::contract(IntervalVector& x)
{
  _cn.reset_variables();
  _ref_var->set_value(x);
  _cn.trigger_all_contractors();
  _cn.contract(false, false);
  x = *_ref_var;
}