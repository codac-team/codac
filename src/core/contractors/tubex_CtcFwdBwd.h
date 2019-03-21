/** 
 *  \file
 *  CtcFwdBwd class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCFWDBWD_H__
#define __TUBEX_CTCFWDBWD_H__

#include "tubex_Ctc.h"
#include "ibex_CtcFwdBwd.h"
#include "tubex_Function.h"

namespace tubex
{
  /**
   * \class CtcFwdBwd
   * \brief \f$\mathcal{C}_{\textrm{FwdBwd}}\f$ that contracts a tube \f$[\mathbf{x}](\cdot)\f$ with respect
   *        to the constraint \f$f(\mathbf{x})=0\f$
   * \note  This contractor is based on the \f$\mathcal{C}_{\textrm{FwdBwd}}\f$ of IBEX, also known
   *        as \f$\mathcal{C}_{\textrm{HC4Revise}}\f$ in the litterature.
   * \todo  Implement propagation methods?
   * \todo  Allow \f$f(\mathbf{x})\leq 0\f$ ?
   */
  class CtcFwdBwd : public Ctc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_{\textrm{FwdBwd}}\f$ for tubes
       *
       * \param f the function \f$f\f$ defining the constraint \f$f(\mathbf{x})=0\f$
       */
      CtcFwdBwd(const tubex::Function& f);

      /**
       * \brief Constractor destructor
       */
      ~CtcFwdBwd();

      /**
       * \brief \f$\mathcal{C}_{\textrm{FwdBwd}}\big([\mathbf{x}](\cdot)\big)\f$
       *
       * \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ to be contracted
       */
      void contract(TubeVector& x);

    protected:

      tubex::Function *m_f; //!< the function need to be stored in order to keep the contractor working
      ibex::CtcFwdBwd *m_ctc_fwdbwd; //!< related IBEX contractor
  };
}

#endif