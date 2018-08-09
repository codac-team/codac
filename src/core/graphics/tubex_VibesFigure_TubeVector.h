/* ============================================================================
 *  tubex-lib - VibesFigure_TubeVector class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGURETUBEVECTOR_H__
#define __TUBEX_VIBESFIGURETUBEVECTOR_H__

#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_VibesFigure_Tube.h"

namespace tubex
{
  class VibesFigure_TubeVector
  {
    public:

      VibesFigure_TubeVector(const std::string& fig_name, int dim = 1);
      VibesFigure_TubeVector(const std::string& fig_name, const TubeVector *tube, const TrajectoryVector *traj = NULL);
      ~VibesFigure_TubeVector();
      int size() const;

      void add_tube(const TubeVector *tube, const std::string& name, const std::string& color_frgrnd = DEFAULT_FRGRND_COLOR, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);
      void set_tube_name(const TubeVector *tube, const std::string& name);
      void set_tube_derivative(const TubeVector *tube, const TubeVector *derivative);
      void set_tube_color(const TubeVector *tube, const std::string& color_frgrnd, const std::string& color_bckgrnd);
      void set_tube_color(const TubeVector *tube, int color_type, const std::string& color);
      void remove_tube(const TubeVector *tube);

      void add_trajectory(const TrajectoryVector *traj, const std::string& name, const std::string& color = DEFAULT_TRAJ_COLOR);
      void set_trajectory_name(const TrajectoryVector *traj, const std::string& name);
      void set_trajectory_color(const TrajectoryVector *traj, const std::string& color);
      void remove_trajectory(const TrajectoryVector *traj);

      void show(bool detail_slices = false);

      // Static methods (shortcuts for fast and simple use)
      static void draw(const std::string& fig_name, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, const TubeVector *tube, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, const TrajectoryVector *traj, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, const TubeVector *tube, const TrajectoryVector *traj, int x = 0, int y = 0);
      static void end_drawing();

    protected:


  };
}

#endif