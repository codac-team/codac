/** 
 *  \file
 *  Ctc class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTC_H__
#define __TUBEX_CTC_H__

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /**
   * \enum TPropagation
   * \brief Specifies the temporal propagation way (forward or backward in time)
   */
  enum TPropagation 
  {
    FORWARD = 0x01, ///< forward in time (from \f$t^-\f$ to \f$t^+\f$)
    BACKWARD = 0x02 ///< backward in time (from \f$t^+\f$ to \f$t^-\f$)
  };
  
  /**
   * \brief Allows a combination of propagation ways
   *
   * \note For instance: `FORWARD | BACKWARD`
   *
   * \param a first TPropagation operand
   * \param b second TPropagation operand
   * \return union of propagation ways, such as `FORWARD | BACKWARD`
   */
  inline TPropagation operator|(TPropagation a, TPropagation b)
  { return static_cast<TPropagation>(static_cast<int>(a) | static_cast<int>(b)); }

  /**
   * \class Ctc
   * \brief Contractor interface
   */
  class Ctc
  {
    public:

      /**
       * \brief Creates a contractor
       */
      Ctc();

      /**
       * \brief Specifies whether the contractor can impact the tube's slicing or not
       *
       * \param preserve if true, the slicing will remain the same
       */
      void preserve_slicing(bool preserve = true);

      /**
       * \brief Specifies an optional fast mode of contraction
       *
       * \note For some contractors, pessimistic but faster contractions can be achieved.
       *
       * \param fast_mode if true, fast mode enabled
       */
      void set_fast_mode(bool fast_mode = true);

      /**
       * \brief Limits the temporal domain of contractions
       *
       * \param domain Interval domain of restriction \f$[t_1,t_2]\f$
       */
      void set_restricted_domain(const ibex::Interval& domain);

    protected:

      bool m_preserve_slicing = true; //!< if `true`, tube's slicing will not be affected by the contractor
      bool m_fast_mode = false; //!< some contractors may propose more pessimistic but faster execution modes
      ibex::Interval m_restricted_domain; //!< limits the contractions to the specified temporal domain
  };
}

#endif