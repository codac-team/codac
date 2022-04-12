/** 
 *  \file
 *  CtcPolar class
 *  Originated from the former pyIbex library (Benoît Desrochers)
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Benoit Desrochers, Mohamed Saad Ibn Seddik
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCPOLAR_H__
#define __CODAC_CTCPOLAR_H__

#include <codac_Interval.h>
#include <codac_IntervalVector.h>
#include <codac_Ctc.h>

using namespace std;

namespace codac
{
  /**
   * \class CtcPolar
   * \brief Minimal contractor for the polar constraint:
   *   x        = rho*cos(theta)
   *   y        = rho*sin(theta)
   *   theta    = angle(x,y)
   *   sqr(rho) = sqr(x)+sqr(y)
   */
  class CtcPolar : public Ctc
  {
    public:

      /**
       * \brief Creates the contractor.
       */
      CtcPolar();

      /**
       * \brief \f$\mathcal{C}_{\textrm{polar}}\big([\mathbf{x}]\big)\f$
       *
       * \param x the 4d box of domains: (x,y,rho,theta)
       */
      virtual void contract(IntervalVector& x);

      /**
       * \brief \f$\mathcal{C}_{\textrm{polar}}\big([x],[y],[\rho],[\theta]\big)\f$
       *
       * \param x first Cartesian component
       * \param y second Cartesian component
       * \param rho first polar component
       * \param theta second polar component
       */
      void contract(Interval& x, Interval& y, Interval& rho, Interval& theta);
  };

  std::tuple<Interval,Interval,Interval> Catan2(const Interval& x, const Interval& y, const Interval& th);
}

#endif