/** 
 *  \file
 *  Trajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_RANDTRAJECTORY_H__
#define __CODAC_RANDTRAJECTORY_H__

#include "codac_Trajectory.h"

namespace codac
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
      RandTrajectory(const Interval& tdomain, double timestep, const Interval& bounds);
  };
}

#endif