/** 
 *  \file
 *  CtcBox class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCBOX_H__
#define __CODAC_CTCBOX_H__

#include "codac_Ctc.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \class CtcBox
   * \brief Contractor around a box
   */
  class CtcBox : public Ctc
  {
    public:

      /**
       * \brief Creates a contractor around a support box
       * 
       * \param b the support box
       */
      CtcBox(const IntervalVector& b) : Ctc(b.size()), _b(b) {};

      /**
       * \brief \f$\mathcal{C}_\mathbf{b}\big([\mathbf{x}]\big)\f$
       *
       * \param x the n-dimension box to be contracted
       */
      void contract(IntervalVector& x);


    protected:

      const IntervalVector _b; //!< the support box
  };
}

#endif