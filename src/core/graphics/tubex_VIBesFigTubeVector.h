/** 
 *  \file
 *  VIBesFigTubeVector class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_VIBESFIGTUBEVECTOR_H__
#define __TUBEX_VIBESFIGTUBEVECTOR_H__

#include "tubex_VIBesFigTube.h"
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"

namespace tubex
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
       * The dimension of the figure (number of windows) is based on the `tubevector` parameter.
       *
       * \param fig_name name of the figure as displayed in the window title
       * \param tubevector a const pointer to the tube vector to be displayed
       * \param trajvector an optional const pointer to a trajectory to be displayed (`NULL` by default)
       */
      VIBesFigTubeVector(const std::string& fig_name, const TubeVector *tubevector, const TrajectoryVector *trajvector = NULL);

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
       * \param t position on the temporal domain
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
       * \param domain the temporal domain related to the box
       * \param box the \f$n\f$d IntervalVector to be displayed
       * \param params VIBes parameters related to the box
       */
      void draw_box(const ibex::Interval& domain, const ibex::IntervalVector& box, const vibes::Params& params);

      /**
       * \brief Draws a multi-dimensional box
       *
       * \param domain the temporal domain related to the box
       * \param box the \f$n\f$d IntervalVector to be displayed
       * \param color the optional color of the box (black by default) 
       * \param params VIBes parameters related to the box (none by default)
       */
      void draw_box(const ibex::Interval& domain, const ibex::IntervalVector& box, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /// @}
      /// \name Handling tubes
      /// @{

      /**
       * \brief Adds a tube vector object to this figure
       *
       * \param tubevector a const pointer to a TubeVector object to be displayed
       * \param name a name to identify this object
       * \param color_frgrnd an optional color for the current values of the tube
       * \param color_bckgrnd an optional color for the previous values of the tube, before any new contraction
       */
      void add_tubevector(const TubeVector *tubevector, const std::string& name, const std::string& color_frgrnd = DEFAULT_FRGRND_COLOR, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);

      /**
       * \brief Sets a new name for a tube vector
       *
       * \param tubevector the const pointer to the TubeVector object to be renamed
       * \param name a new name to identify this object
       */
      void set_tubevector_name(const TubeVector *tubevector, const std::string& name);

      /**
       * \brief Links a tube vector to its derivative for display purposes
       *
       * When available, a derivative tube \f$[\dot{\mathbf{x}}](\cdot)\f$ allows to draw slices of
       * \f$[\mathbf{x}](\cdot)\f$ as polygons, thus displaying a thinner envelope of the tube \f$[\mathbf{x}](\cdot)\f$. 
       *
       * \param tubevector the const pointer to the TubeVector object that will be displayed
       * \param derivative the const pointer its derivative set
       */
      void set_tubevector_derivative(const TubeVector *tubevector, const TubeVector *derivative);

      /**
       * \brief Sets color properties for a given tube vector
       *
       * \param tubevector the const pointer to the TubeVector object for which the colors will be set
       * \param color_frgrnd a color for the current values of the tube
       * \param color_bckgrnd a color for the previous values of the tube, before any new contraction (gray by default)
       */
      void set_tubevector_color(const TubeVector *tubevector, const std::string& color_frgrnd, const std::string& color_bckgrnd = DEFAULT_BCKGRND_COLOR);

      /**
       * \brief Sets color properties for a given tube vector
       *
       * This method allows to change the display of slices, borders, gates, etc.
       *
       * \param tubevector the const pointer to the TubeVector object for which the colors will be set
       * \param color_type the `TubeColorType` key for which the value will be set
       * \param color the new color to be specified for the given type
       */
      void set_tubevector_color(const TubeVector *tubevector, int color_type, const std::string& color);

      /**
       * \brief Removes a tube vector from this figure
       *
       * \todo automatically clear the figure (for now, the tube is only removed
       *       from the list of objects to be shown afterwards)
       *
       * \note the object will not be deleted
       *
       * \param tubevector the const pointer to the TubeVector object to be removed
       */
      void remove_tubevector(const TubeVector *tubevector);

      /// @}
      /// \name Handling trajectories
      /// @{

      /**
       * \brief Adds a trajectory vector object to this figure
       *
       * \param trajvector a const pointer to a TrajectoryVector object to be displayed
       * \param name a name to identify this object
       * \param color an optional color to draw this trajectory
       */
      void add_trajectoryvector(const TrajectoryVector *trajvector, const std::string& name, const std::string& color = DEFAULT_TRAJ_COLOR);

      /**
       * \brief Sets a new name for a trajectory vector
       *
       * \param trajvector the const pointer to the TrajectoryVector object to be renamed
       * \param name a new name to identify this object
       */
      void set_trajectoryvector_name(const TrajectoryVector *trajvector, const std::string& name);

      /**
       * \brief Sets color properties for a given trajectory vector
       *
       * \param trajvector the const pointer to the TrajectoryVector object for which the color will be set
       * \param color a color to draw this trajectory
       */
      void set_trajectoryvector_color(const TrajectoryVector *trajvector, const std::string& color);

      /**
       * \brief Removes a trajectory vector from this figure
       *
       * \todo automatically clear the figure (for now, the trajectory is only removed
       *       from the list of objects to be shown afterwards)
       *
       * \note the object will not be deleted
       *
       * \param trajvector the const pointer to the TrajectoryVector object to be removed
       */
      void remove_trajectoryvector(const TrajectoryVector *trajvector);

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
      VIBesFigTube **m_v_figs = NULL; //!< array of components (scalar figures)
      int m_start_index = -1, m_end_index = -1; //!< restriction on the dimensions that will be displayed
  };
}

#endif