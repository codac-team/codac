/* ============================================================================
 *  tubex-lib - VibesFigure_Map class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGUREMAP_H__
#define __TUBEX_VIBESFIGUREMAP_H__

#include "tubex_VibesFigure_Tube.h"
#include "tubex_TubeVector.h"
#include "tubex_Beacon.h"

namespace tubex
{
  // HTML color codes:
  #define DEFAULT_BEACON_COLOR        "#FF5D00"

  class VibesFigure_Map : public VibesFigure_Tube
  {
    public:

      VibesFigure_Map(const std::string& fig_name, const TubeVector *tube, const TrajectoryVector *traj = NULL);
      
      void add_beacon(const Beacon& beacon, const std::string& color = DEFAULT_BEACON_COLOR);
      void add_beacons(const std::vector<Beacon>& v_beacons, const std::string& color = DEFAULT_BEACON_COLOR);
  
    protected:

      const ibex::IntervalVector draw_tube(const TubeVector *tube, bool detail_slices);
      const ibex::IntervalVector draw_trajectory(const TrajectoryVector *traj, float points_size = 0.);
      void draw_tube_slices(const TubeVector *tube, const vibes::Params& params, bool background_tube);
      const std::string shaded_slice_color(float r) const;
      void draw_slice(const Slice& slice, const std::string& color, const vibes::Params& params);
      void draw_beacon(const Beacon& beacon, const std::string& color, const vibes::Params& params);
      const ibex::IntervalVector keep_ratio(const ibex::IntervalVector& viewbox) const;
  };
}

#endif