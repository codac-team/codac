/* ============================================================================
 *  tubex-lib - CtcDeriv class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_CTCDERIV_H__
#define __TUBEX_CTCDERIV_H__

#include "tubex_Ctc.h"
#include "tubex_Slice.h"

// todo: contract v if x degenerated

namespace tubex
{
  /**
   * \brief CtcDeriv class.
   *
   * Constraint: \dot{x}=v
   */
  class CtcDeriv : public Ctc
  {
    public:

      CtcDeriv();
      void contract(Tube& x, const Tube& v, TPropagation t_propa = FORWARD | BACKWARD);
      void contract(TubeVector& x, const TubeVector& v, TPropagation t_propa = FORWARD | BACKWARD);

    protected:

      void contract(Slice& x, const Slice& v, TPropagation t_propa = FORWARD | BACKWARD);
      void contract_gates(Slice& x, const Slice& v);
      
      friend class CtcEval; // contract_gates used by CtcEval
  };
}

#endif