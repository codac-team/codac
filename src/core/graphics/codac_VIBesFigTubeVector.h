/** 
 *  \file
 *  VIBesFigTubeVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_VIBESFIGTUBEVECTOR_H__
#define __CODAC_VIBESFIGTUBEVECTOR_H__

#include "codac_VIBesFigTube.h"
#include "codac_TubeVector.h"
#include "codac_TrajectoryVector.h"

namespace codac
{
  /**
   * \class VIBesFigTubeVector
   * \brief Multi-view item to display vector tubes or trajectories
   *
   * A set of figures is linked to some tube vector or trajectory pointers, so that
   * any update on these objects can be easily displayed on the figures.
   *
   * There will be as many windows as dimensions to be represented.
   *
   * \todo test this class
   */
  class VIBesFigTubeVector : public Figure
  {
    public:

      /// \name Basics
      /// @{

      /**
       * \brief Creates a VIBesFigTubeVector
       *
       * \param fig_name name of the figure as displayed in the window title
       */
      VIBesFigTubeVector(const std::string& fig_name);

      /**
       * \brief Creates a VIBesFigTubeVector
       *
       * \param fig_name name of the figure as displayed in the window title
       * \param start_index the first dimension to be displayed
       * \param end_index the last dimension to be displayed
       */
      VIBesFigTubeVector(const std::string& fig_name, int start_index, int end_index);

      /**
       * \brief Creates a VIBesFigTube
       *
       * The dimension of the figure (number of windows) is based on the `tube` parameter.
       *
       * \param fig_name name of the figure as displayed in the window title
       * \param tube a const pointer to the tube vector to be displayed
       * \param traj an optional const pointer to a trajectory to be displayed (`nullptr` by default)
       */
      VIBesFigTubeVector(const std::string& fig_name, const TubeVector *tube, const TrajectoryVector *traj = nullptr);

      /**
       * \brief VIBesFigTubeVector destructor
       */
      ~VIBesFigTubeVector();

      /**
       * \brief Returns the dimension \f$n\f$ of the objects displayed in the figure
       *
       * \note Some dimensions may not be displayed (see the subfigs_number method)
       *
       * \return n
       */
      int size() const;

      /**
       * \brief Returns number of dimensions to be displayed
       *
       * This number corresponds to the number of windows to be created.
       *
       * \return the number of subfigures
       */
      int subfigs_number() const;

      /**
       * \brief Returns a pointer to the ith VIBesFigTube of this figure
       *
       * \param index the index of this ith component
       * \return a pointer to the ith component
       */
      VIBesFigTube* operator[](int index);

      /**
       * \brief Returns a const pointer to the ith VIBesFigTube of this figure
       *
       * \param index the index of this ith component
       * \return a const pointer to the ith component
       */
      const VIBesFigTube* operator[](int index) const;
      
      /**
       * \brief Sets the properties (coordinates and dimensions) of this figure
       *
       * \param x horizontal coordinate (in pixels)
       * \param y vertical coordinate (in pixels)
       * \param width width value (in pixels)
       * \param height height value (in pixels)
       */
      void set_properties(int x, int y, int width, int height);

      /**
       * \brief Displays this figure with optional details
       *
       * \param detail_slices if `true`, each slice will be displayed as a box,
       *        otherwise, only polygon envelopes of the tubes will be shown (fast display)
       */
      void show(bool detail_slices = false);

      /**
       * \brief Set the position of the temporal cursor of this figure
       *
       * \param t position on the tdomain
       */
      void set_cursor(double t);

      /**
       * \brief Displays the cursor on this figure
       *
       * \param display parameter (`true` by default)
       */
      void show_cursor(bool display = true);

      /// @}
      /// \name Simple drawings
      /// @{

      /**
       * \brief Draws a multi-dimensional box
       *
       * \param tdomain the tdomain related to the box
       * \param box the \f$n\f$d IntervalVector to be displayed
       * \param params VIBes parameters related to the box
       */
      void draw_box(const Interval& tdomain, const IntervalVector& box, const vibes::Params& params);

      /**
       * \brief Draws a multi-dimensional box
       *
       * \param tdomain the tdomain related to the box
       * \param box the \f$n\f$d IntervalVector to be displayed
       * \param color the optional color of the box (black by default) 
       * \param params VIBes parameters related to the box (none by default)
       */
      void draw_box(const Interval& tdomain, const IntervalVector& box, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /// @}
      /// \name Handling tubes
      /// @{

      /**
       * \brief Adds a tube vector object to this figure
       *
       * \param tube a const pointer to a TubeVector object to be displayed
       * \param name a name to identify this object
       * \param color_frgrnd an optional color for the current values of the tube
       * \param color_bckgrnd an optional color for the previous values of the tube, before any new contraction
       */
      void add_tube(const TubeVector *tube, const std::string& name, const std::string& color_frgrnd = DEFAULT_FRGRND_COLOR, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);

      /**
       * \brief Sets a new name for a tube vector
       *
       * \param tube the const pointer to the TubeVector object to be renamed
       * \param name a new name to identify this object
       */
      void set_tube_name(const TubeVector *tube, const std::string& name);

      /**
       * \brief Links a tube vector to its derivative for display purposes
       *
       * When available, a derivative tube \f$[\dot{\mathbf{x}}](\cdot)\f$ allows to draw slices of
       * \f$[\mathbf{x}](\cdot)\f$ as polygons, thus displaying a thinner envelope of the tube \f$[\mathbf{x}](\cdot)\f$. 
       *
       * \param tube the const pointer to the TubeVector object that will be displayed
       * \param derivative the const pointer its derivative set
       */
      void set_tube_derivative(const TubeVector *tube, const TubeVector *derivative);

      /**
       * \brief Sets color properties for a given tube vector
       *
       * \param tube the const pointer to the TubeVector object for which the colors will be set
       * \param color_frgrnd a color for the current values of the tube
       * \param color_bckgrnd a color for the previous values of the tube, before any new contraction (gray by default)
       */
      void set_tube_color(const TubeVector *tube, const std::string& color_frgrnd, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);

      /**
       * \brief Sets color properties for a given tube vector
       *
       * This method allows to change the display of slices, borders, gates, etc.
       *
       * \param tube the const pointer to the TubeVector object for which the colors will be set
       * \param color_type the `TubeColorType` key for which the value will be set
       * \param color the new color to be specified for the given type
       */
      void set_tube_color(const TubeVector *tube, TubeColorType color_type, const std::string& color);

      /**
       * \brief Reset the background of a given tube vector
       *
       * \todo automatically clear the figure (for now, the pointer is only deleted)
       *
       * \param tube the const pointer to the TubeVector object for which the background will be deleted
       */
      void reset_tube_background(const TubeVector *tube);

      /**
       * \brief Removes a tube vector from this figure
       *
       * \todo automatically clear the figure (for now, the tube is only removed
       *       from the list of objects to be shown afterwards)
       *
       * \note the object will not be deleted
       *
       * \param tube the const pointer to the TubeVector object to be removed
       */
      void remove_tube(const TubeVector *tube);

      /// @}
      /// \name Handling trajectories
      /// @{

      /**
       * \brief Adds a trajectory vector object to this figure
       *
       * \param traj a const pointer to a TrajectoryVector object to be displayed
       * \param name a name to identify this object
       * \param color an optional color to draw this trajectory
       */
      void add_trajectory(const TrajectoryVector *traj, const std::string& name, const std::string& color = DEFAULT_TRAJ_COLOR);

      /**
       * \brief Sets a new name for a trajectory vector
       *
       * \param traj the const pointer to the TrajectoryVector object to be renamed
       * \param name a new name to identify this object
       */
      void set_trajectory_name(const TrajectoryVector *traj, const std::string& name);

      /**
       * \brief Sets color properties for a given trajectory vector
       *
       * \param traj the const pointer to the TrajectoryVector object for which the color will be set
       * \param color a color to draw this trajectory
       */
      void set_trajectory_color(const TrajectoryVector *traj, const std::string& color);
      
      /**
       * \brief Sets points size for a given trajectory, and activates
       *        a display mode with points instead of a line for this trajectory
       *
       * \param traj the const pointer to the TrajectoryVector object for which the points size will be set
       * \param points_size size of the points (if 0, the display is done with a line)
       */
      void set_trajectory_points_size(const TrajectoryVector *traj, double points_size);

      /**
       * \brief Removes a trajectory vector from this figure
       *
       * \todo automatically clear the figure (for now, the trajectory is only removed
       *       from the list of objects to be shown afterwards)
       *
       * \note the object will not be deleted
       *
       * \param traj the const pointer to the TrajectoryVector object to be removed
       */
      void remove_trajectory(const TrajectoryVector *traj);

      /// @}
      /// \name Static methods (shortcuts for fast and simple use)
      /// \todo to be implemented
      /// @{

      //static void draw(const std::string& fig_name, int x = 0, int y = 0);
      //static void draw(const std::string& fig_name, const TubeVector *tubevector, int x = 0, int y = 0);
      //static void draw(const std::string& fig_name, const TrajectoryVector *trajvector, int x = 0, int y = 0);
      //static void draw(const std::string& fig_name, const TubeVector *tubevector, const TrajectoryVector *trajvector, int x = 0, int y = 0);
      //static void end_drawing();

      /// @}

    protected:

      /**
       * \brief Creates the subfigures related to the dimensions that will be displayed
       *
       * \param n
       */
      void create_subfigures(int n);

    protected:

      int m_n = 0; //!< dimension of this figure
      VIBesFigTube **m_v_figs = nullptr; //!< array of components (scalar figures)
      int m_start_index = -1, m_end_index = -1; //!< restriction on the dimensions that will be displayed
  };
}

#endif