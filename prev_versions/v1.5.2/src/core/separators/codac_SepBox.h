/** 
 *  \file
 *  SepBox class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SEPBOX_H__
#define __CODAC_SEPBOX_H__

#include "ibex_Sep.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \class SepBox
   * \brief Separator \f$\mathcal{S}_{box}\f$ that separates two boxes
   *        according to the constraint \f$\mathbf{x}\in[\mathbf{b}]\f$
   */
  class SepBox : public ibex::Sep
  {
    public:

      /**
       * \brief Creates a separator for the constraint \f$\mathbf{x}\in[\mathbf{b}]\f$
       *
       * \param b the support box \f$[\mathbf{b}] \in \mathbb{IR}^n\f$
       */
      SepBox(const IntervalVector& b) : Sep(b.size()), m_b(b) {};

      /**
       * \brief \f$\mathcal{S}\big([\mathbf{x}_{\textrm{in}}],[\mathbf{x}_{\textrm{out}}]\big)\f$
       *
       * \param x_in the n-dimensional box \f$[\mathbf{x}_{\textrm{in}}]\f$ to be inner-contracted
       * \param x_out the n-dimensional box \f$[\mathbf{x}_{\textrm{out}}]\f$ to be outer-contracted
       */
      void separate(IntervalVector& x_in, IntervalVector& x_out);

    protected:
      
      const IntervalVector m_b; //<! The support box
  };
}

#endif