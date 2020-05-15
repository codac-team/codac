/** 
 *  CtcFwdBwd class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
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
    : CtcStatic(*new ibex::CtcFwdBwd(*new ibex::Function(f.ibex_function())), true)
  {
    //m_f = new tubex::Function(f);
    //m_ibex_ctc = ibex::CtcFwdBwd(m_f->ibex_function());
  }
  
  CtcFwdBwd::~CtcFwdBwd()
  {
    delete m_f;
  }
  
  void CtcFwdBwd::contract(vector<Domain*>& v_domains)
  {
    CtcStatic::contract(v_domains);
  }

  void CtcFwdBwd::contract(TubeVector& x)
  {
    return CtcStatic::contract(x);
  }

  void CtcFwdBwd::contract(Tube& x1)
  {
    return CtcStatic::contract(x1);
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2)
  {
    return CtcStatic::contract(x1, x2);
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3)
  {
    return CtcStatic::contract(x1, x2, x3);
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4)
  {
    return CtcStatic::contract(x1, x2, x3, x4);
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5)
  {
    return CtcStatic::contract(x1, x2, x3, x4, x5);
  }

  void CtcFwdBwd::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6)
  {
    return CtcStatic::contract(x1, x2, x3, x4, x5, x6);
  }
}