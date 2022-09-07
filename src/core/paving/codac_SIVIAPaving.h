/** 
 *  \file
 *  SIVIAPaving class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou, Julien Damers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SIVIAPAVING_H__
#define __CODAC_SIVIAPAVING_H__

#include "codac_Paving.h"
#include "codac_Function.h"
#include "codac_Ctc.h"
#include "codac_IntervalVector.h"
#include "codac_Ctc.h"
#include "ibex_Sep.h"

namespace codac
{
  /**
   * \class SIVIAPaving
   * \brief Paving resulting from a Set-Inversion Via Interval Analysis
   */
  class SIVIAPaving : public Paving
  {
    public:

      /**
       * \brief Initializes a SIVIA paving
       *
       * \param init_box n-dimensional box defining the hull of the paving
       */
      SIVIAPaving(const IntervalVector &init_box);

      /**
       * \brief Computes the paving from the constraint \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$.
       *
       * \param f IBEX static function \f$\mathbf{f}\f$, possibly non-linear
       * \param y box \f$[\mathbf{y}]\f$
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       */
      void compute(const Function &f, const IntervalVector &y, float precision);

      /**
       * \brief Computes the paving from using a contractor.
       *
       * \param ctc static contractor on boxes
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       */
      void compute(Ctc& ctc, float precision);

      /**
       * \brief Computes the paving from a separator.
       * \param sep separator object
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       */
      void compute(ibex::Sep& sep, float precision);
  };
}

#endif