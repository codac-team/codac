/** 
 *  @file
 *  VIBesFigure class
 * ================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGURE_H__
#define __TUBEX_VIBESFIGURE_H__

#include "tubex_Figure.h"
#include "tubex_Polygon.h"
#include "vibes.h"

namespace tubex
{
  /**
   * \class VIBesFigure
   * \brief Two-dimensional graphical item based on the VIBes viewer
   */

  class VIBesFigure : public Figure
  {
    public:

      /**
       * \brief Creates a VIBesFigure
       *
       * \param figure_name a reference to the figure that will be displayed in the window's title
       */
      VIBesFigure(const std::string& figure_name);

      /**
       * \brief VIBesFigure destructor
       */
      ~VIBesFigure() {};
      
      /**
       * \brief Sets the properties (position and dimensions) of this figure
       *
       * \param x horizontal position (in pixels)
       * \param y vertical position (in pixels)
       * \param width width value (in pixels)
       * \param height height value (in pixels)
       */
      void set_properties(int x, int y, int width, int height);

      /**
       * \brief Sets the axis limits of this figure
       *
       * The function updates the viewbox and applies the changes.
       *
       * \param x_min lower horizontal value to be displayed
       * \param x_max upper horizontal value to be displayed
       * \param y_min lower vertical value to be displayed
       * \param y_max upper vertical value to be displayed
       * \param same_ratio if `true`, will compute the min/max values so
       *        that the previous ratio will be preserved (false by default)
       * \param margin adds a custom margin to the view box (none by default)
       * \return the updated view box of this figure
       */
      const ibex::IntervalVector& axis_limits(double x_min, double x_max, double y_min, double y_max, bool same_ratio = false, float margin = 0.);

      /**
       * \brief Sets the axis limits of this figure
       *
       * The function updates the viewbox and applies the changes.
       *
       * \param viewbox the 2d box defining lower/upper horizontal/vertical values
       * \param same_ratio if `true`, will compute the min/max values so
       *        that the previous ratio will be preserved (false by default)
       * \param margin adds a custom margin to the view box (none by default)
       * \return the updated view box of this figure
       */
      const ibex::IntervalVector& axis_limits(const ibex::IntervalVector& viewbox, bool same_ratio = false, float margin = 0.);

      /**
       * \brief Saves the figure in SVG/PNG/... format
       *
       * A file named {figure_name}{suffix}.{extension} will be created in the current directory.
       *
       * \param suffix optional part name that can be added to the figure name (none by default)
       * \param extension optional part to specify the type of the image ("svg" by default)
       */
      void save_image(const std::string& suffix = "", const std::string& extension = "svg") const;

      /**
       * \brief Displays this figure
       */
      void show() {};

      /**
       * \brief Clears this figure by removing displayed items
       */
      void clear();
      
      void draw_box(const ibex::IntervalVector& box, const vibes::Params& params);
      void draw_box(const ibex::IntervalVector& box, const std::string& color = "", const vibes::Params& params = vibes::Params());
      void draw_line(const std::vector<double>& v_x, const std::vector<double>& v_y, const vibes::Params& params);
      void draw_line(const std::vector<double>& v_x, const std::vector<double>& v_y, const std::string& color = "", const vibes::Params& params = vibes::Params());
      void draw_circle(double x, double y, double r, const vibes::Params& params);
      void draw_circle(double x, double y, double r, const std::string& color = "", const vibes::Params& params = vibes::Params());
      void draw_polygon(const Polygon& p, const vibes::Params& params);
      void draw_polygon(const Polygon& p, const std::string& color = "", const vibes::Params& params = vibes::Params());
      void draw_points(const std::vector<Point>& v_pts, float size, const vibes::Params& params);
      void draw_points(const std::vector<Point>& v_pts, float size, const std::string& color = "", const vibes::Params& params = vibes::Params());

  };
}

#endif