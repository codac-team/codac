/** 
 *  \file
 *  CtcLinobs class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCLINOBS_H__
#define __TUBEX_CTCLINOBS_H__

#include "tubex_Ctc.h"

namespace tubex
{
  /**
   * \class CtcLinobs
   */
  class CtcLinobs : public Ctc
  {
    public:

      /**
       * \brief Creates a contractor object \f$\mathcal{C}_\textrm{linobs}\f$
       */
      // CtcLinobs(const ibex::Matrix& A, const ibex::Vector& b); // not yet available since auto evaluation of e^At not at hand
      CtcLinobs(ibex::IntervalMatrix (*exp_At)(const ibex::Matrix& A, const ibex::Interval& t), const ibex::Vector& b);

      void contract(std::vector<Domain*>& v_domains);
      void contract(TubeVector& x, const Tube& u);

    protected:

      ibex::IntervalMatrix (*m_exp_At)(const ibex::Matrix& A, const ibex::Interval& t);
  };
}

#endif