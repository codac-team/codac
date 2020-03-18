/** 
 *  \file
 *  CtcDelay class
 * ----------------------------------------------------------------------------
 *  \date       2017
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCDELAY_H__
#define __TUBEX_CTCDELAY_H__

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \class CtcDelay
   * \brief \f$\mathcal{C}_{t-\tau}\f$ that contracts a tube \f$[\mathbf{y}](\cdot)\f$ with respect
   *        to the delay constraint \f$\mathbf{y}(t)=\mathbf{x}(t-\tau)\f$
   * \todo  Contract all members
   */
  class CtcDelay : public Ctc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_{t-\tau}\f$
       */
      CtcDelay();
      
      void contract(std::vector<AbstractDomain>& v_domains);

      /**
       * \brief \f$\mathcal{C}_{t-\tau}\big([a],[x](\cdot),[y](\cdot)\big)\f$:
       *        contracts the tube \f$[y](\cdot)\f$ with respect to the constraint \f$y(t)=x(t-a)\f$
       *
       * \param a delay value \f$\tau\f$
       * \param x the scalar tube \f$[x](\cdot)\f$
       * \param y the scalar tube \f$[y](\cdot)\f$ to be contracted
       */
      void contract(const ibex::Interval& a, const Tube& x, Tube& y);

      /**
       * \brief \f$\mathcal{C}_{t-\tau}\big([a],[\mathbf{x}](\cdot),[\mathbf{y}](\cdot)\big)\f$:
       *        contracts the tube \f$[\mathbf{y}](\cdot)\f$ with respect to the constraint \f$\mathbf{y}(t)=\mathbf{x}(t-a)\f$
       *
       * \param a delay value \f$\tau\f$
       * \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$ to be contracted
       */
      void contract(const ibex::Interval& a, const TubeVector& x, TubeVector& y);

    protected:

  };
}

#endif