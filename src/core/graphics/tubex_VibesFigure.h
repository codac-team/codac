/* ============================================================================
 *  tubex-lib - VibesFigure class
 * ============================================================================
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

#include <string>
#include "ibex_IntervalVector.h"
#include "vibes.h"
#include "tubex_Figure.h"
#include "tubex_Polygon.h"

namespace tubex
{
  class VibesFigure : public Figure
  {
    public:

      /**
       * \brief Create an object view: abstract class.
       *
       * \param figure_name a reference to the figure that will be displayed in the window's title
       */
      VibesFigure(const std::string& figure_name);

      /**
       * \brief Delete this figure.
       */
      ~VibesFigure() {};
      
      /**
       * \brief Set figure's properties: position and dimensions.
       *
       * \param x x-position
       * \param y y-position
       * \param width width value (px)
       * \param height height value (px)
       */
      void set_properties(int x, int y, int width, int height);

      /**
       * \brief Return figure's view box.
       *
       * \return IntervalVector
       */
      const ibex::IntervalVector& view_box() const;

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

      /**
       * \brief Set figure's axis limits.
       *
       * same_ratio view box is set to fit with the params, without changing its ratio (false by default)
       */
      const ibex::IntervalVector& axis_limits(double x_min, double x_max, double y_min, double y_max, bool same_ratio = false, float margin = 0.);
      const ibex::IntervalVector& axis_limits(const ibex::IntervalVector& viewbox, bool same_ratio = false, float margin = 0.);
      
      /**
       * \brief Save vibes-figure in SVG/PNG/... format.
       *
       * A file named {figure_name}{suffix}.svg is created in the current directory.
       *
       * \param suffix optional part name that can be added to figure_name, empty by default
       * \param extension optional part to specify image type, ".svg" by default
       */
      void save_image(const std::string& suffix = "", const std::string& extension = ".svg") const;

      /**
       * \brief Display the figure.
       *
       * This is a virtual method to overload.
       */
      //virtual void show() = 0;
      void show() {};

      void clear();

    protected:

      ibex::IntervalVector m_view_box;
  };
}

#endif