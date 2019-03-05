/** 
 *  @file
 *  VIBesFigTube class
 * ================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGTUBE_H__
#define __TUBEX_VIBESFIGTUBE_H__

#include "tubex_VIBesFig.h"
#include "tubex_Tube.h"
#include "tubex_Slice.h"
#include "tubex_Trajectory.h"

namespace tubex
{
  #define DEFAULT_TUBE_NAME         "[?](·)"
  #define DEFAULT_TRAJ_NAME         "?(·)"
  #define TRAJ_NB_DISPLAYED_POINTS  10000
  
  // HTML color codes:
  #define DEFAULT_TRAJ_COLOR        "#004257"
  #define DEFAULT_FRGRND_COLOR      "#a2a2a2[#a2a2a2]"
  #define DEFAULT_BCKGRND_COLOR     "#d2d2d2[#d2d2d2]"
  #define DEFAULT_SLICES_COLOR      "#828282[#F0F0F0]"
  #define DEFAULT_GATES_COLOR       "#0084AF[#0084AF]"
  #define DEFAULT_POLYGONS_COLOR    "#00536E[#2696BA]"

  /**
   * \enum TubeColorType
   * \brief Defines a set of colors for tube display
   */
  enum TubeColorType { FOREGROUND, BACKGROUND, SLICES, GATES, POLYGONS };

  /**
   * \class VIBesFigTube
   * \brief Two-dimensional graphical item to display scalar tubes or trajectories
   *
   * One figure is linked to some tube or trajectory pointers, so that
   * any update on these objects can be easily displayed on the figure. 
   */
  class VIBesFigTube : public VIBesFig
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Creates a VIBesFigTube
       *
       * \param fig_name name of the figure as displayed in the window title
       * \param tube an optional const pointer to the tube to be displayed (`NULL` by default)
       * \param traj an optional const pointer to a trajectory to be displayed (`NULL` by default)
       */
      VIBesFigTube(const std::string& fig_name, const Tube *tube = NULL, const Trajectory *traj = NULL);

      /**
       * \brief VIBesFigTube destructor
       */
      ~VIBesFigTube();

      /**
       * \brief Displays this figure
       */
      void show();

      /**
       * \brief Displays this figure with optional details
       *
       * \param detail_slices if `true`, each slice will be displayed as a box,
       *        otherwise, only polygon envelopes of the tubes will be shown (fast display)
       */
      void show(bool detail_slices);

      /// @}
      /// \name Handling tubes
      /// @{

      /**
       * \brief Adds a tube object to this figure
       *
       * \param tube a const pointer to a Tube object to be displayed
       * \param name a name to identify this object
       * \param color_frgrnd an optional color for the current values of the tube
       * \param color_bckgrnd an optional color for the previous values of the tube, before any new contraction
       */
      void add_tube(const Tube *tube, const std::string& name, const std::string& color_frgrnd = DEFAULT_FRGRND_COLOR, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);

      /**
       * \brief Sets a new name for a tube
       *
       * \param tube the const pointer to the Tube object to be renamed
       * \param name a new name to identify this object
       */
      void set_tube_name(const Tube *tube, const std::string& name);

      /**
       * \brief Links a tube to its derivative for display purposes
       *
       * When available, a derivative tube \f$[\dot{x}](\cdot)\f$ allows to draw slices of
       * \f$[x](\cdot)\f$ as polygons, thus displaying a thinner envelope of the tube \f$[x](\cdot)\f$. 
       *
       * \param tube the const pointer to the Tube object that will be displayed
       * \param derivative the const pointer its derivative
       */
      void set_tube_derivative(const Tube *tube, const Tube *derivative);
      
      void set_tube_color(const Tube *tube, const std::string& color_frgrnd, const std::string& color_bckgrnd);
      void set_tube_color(const Tube *tube, int color_type, const std::string& color);
      void remove_tube(const Tube *tube);

      /// @}
      /// \name Handling trajectories
      /// @{

      /**
       * \brief Adds a trajectory object to this figure
       *
       * \param traj a const pointer to a Trajectory object to be displayed
       * \param name a name to identify this object
       * \param color an optional color to draw this trajectory
       */
      void add_trajectory(const Trajectory *traj, const std::string& name, const std::string& color = DEFAULT_TRAJ_COLOR);

      /**
       * \brief Sets a new name for a trajectory
       *
       * \param traj the const pointer to the Trajectory object to be renamed
       * \param name a new name to identify this object
       */
      void set_trajectory_name(const Trajectory *traj, const std::string& name);
      void set_trajectory_color(const Trajectory *traj, const std::string& color);
      void remove_trajectory(const Trajectory *traj);

      /// @}

    protected:

      void create_group_color(const Tube *tube, int color_type);
      void create_groups_color(const Tube *tube);
      const ibex::IntervalVector draw_tube(const Tube *tube, bool detail_slices = false);
      void draw_slice(const Slice& slice, const vibes::Params& params);
      void draw_slice(const Slice& slice, const Slice& deriv_slice, const vibes::Params& params_slice, const vibes::Params& params_polygon);
      void draw_gate(const ibex::Interval& gate, double t, const vibes::Params& params);
      const ibex::IntervalVector draw_trajectory(const Trajectory *traj, float points_size = 0.);

    protected:

      struct FigTubeParams
      {
        std::map<int,std::string> m_colors;
        const Tube *tube_copy = NULL; // to display previous values in background
        const Tube *tube_derivative = NULL; // to display polygons enclosed by slices
        std::string name;
      };

      struct FigTrajParams
      {
        std::string color;
        std::string name;
      };

      std::map<const Tube*,FigTubeParams> m_map_tubes;
      std::map<const Trajectory*,FigTrajParams> m_map_trajs;

      friend class VIBesFigTubeVector;
  };
}

#endif