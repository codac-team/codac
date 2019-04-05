/** 
 *  CtcFwdBwd class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcFwdBwd.h"
#include "ibex_CtcFwdBwd.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcFwdBwd::CtcFwdBwd(const tubex::Function& f)
    : CtcIbex(NULL)
  {
    m_f = new tubex::Function(f);
    m_ibex_ctc = new ibex::CtcFwdBwd(m_f->ibex_function());
  }
  
  CtcFwdBwd::~CtcFwdBwd()
  {
    delete m_ibex_ctc;
    delete m_f;
  }
}