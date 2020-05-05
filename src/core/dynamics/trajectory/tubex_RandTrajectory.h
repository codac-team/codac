/** 
 *  \file
 *  Trajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_RANDTRAJECTORY_H__
#define __TUBEX_RANDTRAJECTORY_H__

#include "tubex_Trajectory.h"

namespace tubex
{  
  /**
   * \class RandTrajectory
   * \brief One dimensional random trajectory \f$x(\cdot)\f$, used to represent noises
   */
  class RandTrajectory : public Trajectory
  {
    public:

      /// \name Definition
      /// @{

      /**
       * \brief Creates a scalar trajectory \f$x(\cdot)\f$ made of random values
       *
       * \param tdomain temporal domain \f$[t_0,t_f]\f$
       * \param timestep sampling value \f$\delta\f$ for the temporal discretization (double)
       * \param bounds interval range for random values
       */
      RandTrajectory(const ibex::Interval& tdomain, double timestep, const ibex::Interval& bounds);
  };
}

#endif