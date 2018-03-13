/* ============================================================================
 *  tubex-lib - VibesFigure_Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef VIBESFIGURETUBE_HEADER
#define VIBESFIGURETUBE_HEADER

#include "tubex_VibesFigure.h"
#include "tubex_Tube.h"
#include "tubex_Trajectory.h"

// HTML color codes:
#define TRAJ_COLOR                "#276279"
#define TUBE_FRGRND_COLOR         "#a2a2a2"
#define TUBE_BCKGRND_COLOR        "#d2d2d2"
#define TRAJ_NB_DISPLAYED_POINTS  10000

namespace tubex
{
  class VibesFigure_Tube : public VibesFigure
  {
    public:

      VibesFigure_Tube(const std::string& fig_name, Tube *tube = NULL, Trajectory *traj = NULL);
      ~VibesFigure_Tube();

      void addTube(Tube *tube, const std::string& color_frgrnd = TUBE_FRGRND_COLOR, const std::string& color_bckgrnd = TUBE_BCKGRND_COLOR);
      void setTubeColor(Tube *tube, const std::string& color_frgrnd, const std::string& color_bckgrnd);
      void removeTube(Tube *tube);

      void addTrajectory(Trajectory *traj, const std::string& color = TRAJ_COLOR);
      void setTrajectoryColor(Trajectory *traj, const std::string& color);
      void removeTrajectory(Trajectory *traj);

      virtual void show();
      void show(bool detail_slices);

      static void draw(const std::string& fig_name, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, Tube *tube, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, Trajectory *traj, int x = 0, int y = 0);
      static void draw(const std::string& fig_name, Tube *tube, Trajectory *traj, int x = 0, int y = 0);
      static void endDrawing();

    protected:

      int idTube(Tube *tube);
      int idTrajectory(Trajectory *traj);
      const ibex::IntervalVector drawTube(Tube *tube, bool detail_slices = false);
      static void computePolygonEnvelope(const Tube& tube, std::vector<double>& v_x, std::vector<double>& v_y);
      void drawSlice(const ibex::IntervalVector& slice, const vibes::Params& params) const;
      const ibex::IntervalVector drawTrajectory(Trajectory *traj, float points_size = 0.);

    protected:

      enum TubeColorType { TUBE_FOREGROUND_COLOR, TUBE_BACKGROUND_COLOR, TUBE_SLICES_COLOR };

      std::vector<Tube*> m_v_tubes;
      mutable std::map<Tube*,Tube*> m_m_tubes_copy;
      std::map<Tube*,std::map<int,std::string> > m_m_tubes_color;

      std::vector<Trajectory*> m_v_traj;
      std::map<Trajectory*,std::string> m_m_traj_color;

      static std::vector<VibesFigure_Tube*> v_vibesfig_tube;
  };
}

#endif