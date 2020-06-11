/** 
 *  \file
 *  SIVIAPaving class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_SIVIAPAVING_H__
#define __TUBEX_SIVIAPAVING_H__

#include "tubex_Paving.h"
#include "ibex_Function.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  /**
   * \class SIVIAPaving
   * \brief Paving resulting from a Set-Inversion Via Interval Analysis
   */
  class SIVIAPaving : public Paving
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Initializes a SIVIA paving
       *
       * \param init_box n-dimensional box defining the hull of the paving
       */
      SIVIAPaving(const ibex::IntervalVector& init_box);

      /**
       * \brief Computes the paving from the constraint \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$.
       *
       * \param f IBEX static function \f$\mathbf{f}\f$, possibly non-linear
       * \param y box \f$[\mathbf{y}]\f$
       * \param precision precision \f$\epsilon\f$ of the SIVIA approximation
       */
      void compute(const ibex::Function& f, const ibex::IntervalVector& y, float precision);

      /// @}
  };
}

#endif