/* ============================================================================
 *  tubex-lib - CtcFwdBwd class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCFWDBWD_H__
#define __TUBEX_CTCFWDBWD_H__

#include "tubex_Ctc.h"
#include "ibex_CtcFwdBwd.h"
#include "tubex_Slice.h"

namespace tubex
{
  /**
   * \brief CtcFwdBwd class.
   */
  class CtcFwdBwd : public Ctc
  {
    public:

      CtcFwdBwd(const tubex::Function& f);
      ~CtcFwdBwd();
      void contract(TubeVector& x);

    protected:

      tubex::Function *m_f = NULL;
  };
}

#endif