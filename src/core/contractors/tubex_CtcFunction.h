/** 
 *  \file
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCFUNCTION_H__
#define __TUBEX_CTCFUNCTION_H__

#include "tubex_Ctc.h"
#include "ibex_CtcFwdBwd.h"

namespace tubex
{
  /**
   * \class CtcFunction
   * \brief Generic static \f$\mathcal{C}\f$ that contracts a box \f$[\mathbf{x}]\f$ 
   *        according to the constraint \f$f(\mathbf{x})=0\f$.
   *        It stands on the CtcFwdBwd of IBEX (HC4Revise).
   */
  class CtcFunction : public Ctc
  {
    public:

      CtcFunction(const char* x1, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* f);
      CtcFunction(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* f);
      ~CtcFunction();

      void contract(std::vector<Domain*>& v_domains);

    protected:

      ibex::Function *m_ibex_fnc = NULL;
      ibex::CtcFwdBwd *m_ibex_ctc = NULL;
  };
}

#endif