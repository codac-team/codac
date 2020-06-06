/** 
 *  \file
 *  DynCtc class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DYNCTC_H__
#define __TUBEX_DYNCTC_H__

#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  class Domain;
  
  /**
   * \enum TimePropag
   * \brief Specifies the temporal propagation way (forward or backward in time)
   */
  enum class TimePropag
  {
    FORWARD = 0x01, ///< forward in time (from \f$t^-\f$ to \f$t^+\f$)
    BACKWARD = 0x02 ///< backward in time (from \f$t^+\f$ to \f$t^-\f$)
  };
  
  /**
   * \brief Allows tests on combinations of propagation ways
   *
   * Used for tests such as `if(time_propag & TimePropag::FORWARD) { ... }`
   *
   * \param a first TimePropag operand
   * \param b second TimePropag operand
   * \return intersection of propagation ways
   */
  inline int operator&(TimePropag a, TimePropag b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }
  
  /**
   * \brief Allows a combination of propagation ways
   *
   * \note For instance: `FORWARD | BACKWARD`
   *
   * \param a first TimePropag operand
   * \param b second TimePropag operand
   * \return union of propagation ways, such as `FORWARD | BACKWARD`
   */
  inline TimePropag operator|(TimePropag a, TimePropag b)
  { return static_cast<TimePropag>(static_cast<int>(a) | static_cast<int>(b)); }

  /**
   * \class DynCtc
   * \brief Contractor interface
   */
  class DynCtc
  {
    public:

      /**
       * \brief Creates a contractor
       *
       * \param inter-temporal set to `true` if it implements an inter-temporal constraint
       */
      DynCtc(bool intertemporal = true); // todo: remove default value

      /**
       * \brief DynCtc destructor
       */
      virtual ~DynCtc();

      /*
       * \brief Contracts a set of abstract domains
       *
       * This method has to be overridden in order to make the contractor
       * available in the CN framework.
       *
       * \param v_domains vector of Domain pointers
       */
      virtual void contract(std::vector<Domain*>& v_domains) = 0;

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
       * \param tdomain Interval tdomain of restriction \f$[t_1,t_2]\f$
       */
      void restrict_tdomain(const ibex::Interval& tdomain);

      /**
       * \brief Tests if the related constraint is inter-temporal or not
       *
       * \return `true` in case of inter-temporality
       */
      bool is_intertemporal() const;

    protected:

      bool m_preserve_slicing = true; //!< if `true`, tube's slicing will not be affected by the contractor
      bool m_fast_mode = false; //!< some contractors may propose more pessimistic but faster execution modes
      ibex::Interval m_restricted_tdomain; //!< limits the contractions to the specified temporal domain
      const bool m_intertemporal = true; //!< defines if the related constraint is inter-temporal or not (true by default)
  };
}

#endif