/** 
 *  \file
 *  CtcChain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCCHAIN_H__
#define __TUBEX_CTCCHAIN_H__

#include "tubex_CtcLinobs.h"

namespace tubex
{
  /**
   * \class CtcChain
   */
  class CtcChain : public CtcLinobs
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_\textrm{chain}\f$
       */
      CtcChain();

      void contract(std::vector<Domain*>& v_domains);
      void contract(Tube& x, Tube& v, const Tube& a, TimePropag t_propa = TimePropag::FORWARD | TimePropag::BACKWARD);


    protected:

  };
}

#endif