/** 
 *  \file
 *  SepFunction class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SEPFUNCTION_H__
#define __CODAC_SEPFUNCTION_H__

#include "ibex_SepFwdBwd.h"
#include "codac_Function.h"
#include "codac_IntervalVector.h"

namespace codac
{
  /**
   * \class SepFunction
   * \brief Generic static \f$\mathcal{S}\f$ that separates two boxes
   *        according to the constraint \f$\mathbf{f}(\mathbf{x})=\mathbf{0}\f$ or \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$.
   *        It stands on the SepFwdBwd of IBEX (involving HC4Revise).
   */
  class SepFunction : public ibex::SepFwdBwd
  {
    public:

      /**
       * \brief Creates a separator for the constraint \f$\mathbf{f}(\mathbf{x})=\mathbf{0}\f$
       *
       * \param f the function \f$\mathbf{f}\f$
       */
      SepFunction(Function& f);

      /**
       * \brief Creates a separator for the constraint \f$f(\mathbf{x})\in[y]\f$
       *        for the scalar case
       *
       * \param f the function \f$f\f$
       * \param y the interval \f$[y]\f$
       */
      SepFunction(Function& f, const Interval& y);

      /**
       * \brief Creates a separator for the constraint \f$\mathbf{f}(\mathbf{x})\in[\mathbf{y}]\f$
       *        for the vectorial case
       *
       * \param f the function \f$\mathbf{f}\f$
       * \param y the IntervalVector \f$[\mathbf{y}]\f$
       */
      SepFunction(Function& f, const IntervalVector& y);
      
      /**
       * \brief \f$\mathcal{S}\big([\mathbf{x}_{\textrm{in}}],[\mathbf{x}_{\textrm{out}}]\big)\f$
       *
       * \param x_in the n-dimensional box \f$[\mathbf{x}]\f$ to be inner-contracted
       * \param x_out the n-dimensional box \f$[\mathbf{x}]\f$ to be outer-contracted
       */
      void separate(IntervalVector& x_in, IntervalVector& x_out);
  };
}

#endif