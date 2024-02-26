/** 
 *  \file
 *  CtcDelay class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Raphael Voges
 *  \copyright  Copyright 2020 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CTCDELAY_H__
#define __CODAC_CTCDELAY_H__

#include "codac_DynCtc.h"

namespace codac
{
  /**
   * \class CtcDelay
   * \brief \f$\mathcal{C}_{delay}\f$ that contracts the tubes \f$[x](\cdot)\f$ and \f$[y](\cdot)\f$
   *        with respect to their delay \f$[\tau]\f$ according to
   *        the delay constraint \f$\mathbf{x}(t)=\mathbf{y}(t+\tau)\f$
   */
  class CtcDelay : public DynCtc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_{delay}\f$
       */
      CtcDelay();

      /*
       * \brief Contracts a set of abstract domains
       *
       * This method makes the contractor available in the CN framework.
       *
       * \param v_domains vector of Domain pointers
       */
      void contract(std::vector<Domain*>& v_domains);

      /**
       * \brief \f$\mathcal{C}_{delay}\big([a],[x](\cdot),[y](\cdot)\big)\f$:
       *        contracts the tubes \f$[x](\cdot)\f$, \f$[y](\cdot)\f$ and the delay \f$[a]\f$
       *        with respect to the constraint \f$x(t)=y(t+a)\f$
       *
       * \param a the delay value \f$\tau\f$ to be contracted
       * \param x the scalar tube \f$[x](\cdot)\f$ to be contracted
       * \param y the scalar tube \f$[y](\cdot)\f$ to be contracted
       */
      void contract(Interval& a, Tube& x, Tube& y);

      /**
       * \brief \f$\mathcal{C}_{delay}\big([a],[\mathbf{x}](\cdot),[\mathbf{y}](\cdot)\big)\f$:
       *        contracts the tube vectors \f$[\mathbf{x}](\cdot)\f$, \f$[\mathbf{y}](\cdot)\f$ and the delay \f$[a]\f$
       *        with respect to the constraint \f$\mathbf{x}(t)=\mathbf{y}(t+a)\f$
       *
       * \param a the delay value \f$\tau\f$ to be contracted
       * \param x the n-dimensional tube \f$[\mathbf{x}](\cdot)\f$ to be contracted
       * \param y the n-dimensional tube \f$[\mathbf{y}](\cdot)\f$ to be contracted
       */
      void contract(Interval& a, TubeVector& x, TubeVector& y);

    protected:

      static const std::string m_ctc_name; //!< class name (mainly used for CN Exceptions)
      static std::vector<std::string> m_str_expected_doms; //!< allowed domains signatures (mainly used for CN Exceptions)
      friend class ContractorNetwork;
  };
}

#endif
