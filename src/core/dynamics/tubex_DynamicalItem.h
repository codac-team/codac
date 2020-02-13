/** 
 *  \file
 *  DynamicalItem class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DYNAMICALITEM_H__
#define __TUBEX_DYNAMICALITEM_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  /**
   * \class DynamicalItem
   * \brief Abstract class for common properties of Tube, TubeVector,
   *        Slice, Trajectory, TrajectoryVector objects
   */
  class DynamicalItem
  {
    public:

      /**
       * \brief Returns the dimension of the object
       *
       * \return n
       */
      virtual int size() const = 0;

      /**
       * \brief Returns the temporal definition domain of this object
       *
       * \return an Interval object \f$[t_0,t_f]\f$
       */
      virtual const ibex::Interval domain() const = 0;

      /**
       * \brief Returns the box of the feasible values along \f$[t_0,t_f]\f$
       *
       * \note Used for genericity purposes
       *
       * \return the envelope of codomain values
       */
      virtual const ibex::IntervalVector codomain_box() const = 0;

      /**
       * \brief Returns the name of this class
       *
       * \note Only used for some generic display method
       *
       * \return the predefined name
       */
      virtual const std::string class_name() const = 0;

      /**
       * \brief Verifies that this interval is a feasible temporal domain
       *
       * \note The domain must be non-empty, bounded and not degenerated
       * \todo Allow unbounded domains such as \f$[t_0,\infty]\f$?
       *
       * \param domain Interval domain \f$[t_0,t_f]\f$ to be tested
       * \return true in case of valid temporal domain
       */
      static bool valid_domain(const ibex::Interval& domain);
  };
}

#endif