/** 
 *  \file
 *  VIBesFig class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_VIBESFIG_H__
#define __TUBEX_VIBESFIG_H__

#include "tubex_Figure.h"
#include "tubex_Point.h"
#include "tubex_Polygon.h"
#include "tubex_ConvexPolygon.h"
#include "tubex_ColorMap.h"
#include "vibes.h"

namespace tubex
{
  /**
   * \class VIBesFig
   * \brief Two-dimensional graphical item based on the VIBes viewer
   */
  class VIBesFig : public Figure
  {
    public:

      /// \name Definition and properties
      /// @{

      /**
       * \brief Creates a VIBesFig
       *
       * \param fig_name a reference to the figure that will be displayed in the window's title
       */
      VIBesFig(const std::string& fig_name);

      /**
       * \brief VIBesFig destructor
       */
      ~VIBesFig();

      /**
       * \brief Closes this figure
       */
      void close();
      
      /**
       * \brief Sets the properties (coordinates and dimensions) of this figure
       *
       * \param x horizontal center coordinate (in pixels)
       * \param y vertical center coordinate (in pixels)
       * \param width width value (in pixels)
       * \param height height value (in pixels)
       */
      void set_properties(int x, int y, int width, int height);
      
      /**
       * \brief Sets the background color of this figure
       *
       * \param bg_color color of the background
       */
      void set_background(const std::string& bg_color);

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
       * \param margin adds a custom margin to the view box (none by default, ratio of max diam)
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
       * \param margin adds a custom margin to the view box (none by default, ratio of max diam)
       * \return the updated view box of this figure
       */
      const ibex::IntervalVector& axis_limits(const ibex::IntervalVector& viewbox, bool same_ratio = false, float margin = 0.);

      /// @}
      /// \name Saving this figure
      /// @{

      /**
       * \brief Saves the figure in SVG/PNG/... format
       *
       * A file named {path}/{figure_name}{suffix}.{extension} will be created in the current directory.
       *
       * \param suffix optional part name that can be added to the figure name (none by default)
       * \param extension optional part to specify the type of the image ("svg" by default)
       * \param path optional path to a different directory ("." by default)
       */
      void save_image(const std::string& suffix = "", const std::string& extension = "svg", const std::string& path = ".") const;

      /// @}
      /// \name Figure's content
      /// @{

      /**
       * \brief Displays this figure
       */
      void show() {};

      /**
       * \brief Clears this figure by removing displayed items
       */
      void clear();
      
      /// @}
      /// \name Displaying objects
      /// @{

      /**
       * \brief Draws a box
       *
       * \param box the 2d IntervalVector to be displayed
       * \param params VIBes parameters related to the box
       */
      void draw_box(const ibex::IntervalVector& box, const vibes::Params& params);

      /**
       * \brief Draws a box
       *
       * \param box the 2d IntervalVector to be displayed
       * \param color the optional color of the box (black by default) 
       * \param params VIBes parameters related to the box (none by default)
       */
      void draw_box(const ibex::IntervalVector& box, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a set of boxes
       *
       * \param v_boxes vector of 2d IntervalVector to be displayed
       * \param params VIBes parameters related to the boxes
       */
      void draw_boxes(const std::vector<ibex::IntervalVector>& v_boxes, const vibes::Params& params);

      /**
       * \brief Draws a set of boxes
       *
       * \param v_boxes vector of 2d IntervalVector to be displayed
       * \param color the optional color of the boxes (black by default) 
       * \param params VIBes parameters related to the boxes (none by default)
       */
      void draw_boxes(const std::vector<ibex::IntervalVector>& v_boxes, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a line of points
       *
       * \param v_x vector of horizontal center coordinates
       * \param v_y vector of vertical center coordinates
       * \param params VIBes parameters related to the line
       */
      void draw_line(const std::vector<double>& v_x, const std::vector<double>& v_y, const vibes::Params& params);

      /**
       * \brief Draws a line of points
       *
       * \param v_x vector of horizontal center coordinates
       * \param v_y vector of vertical center coordinates
       * \param color the optional color of the line (black by default) 
       * \param params VIBes parameters related to the line (none by default)
       */
      void draw_line(const std::vector<double>& v_x, const std::vector<double>& v_y, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a line of points
       *
       * \param v_pts vector of points that are expressed as 2d std::vector of coordinates
       * \param params VIBes parameters related to the line
       */
      void draw_line(const std::vector<std::vector<double> >& v_pts, const vibes::Params& params);

      /**
       * \brief Draws a line of points
       *
       * \param v_pts vector of points that are expressed as 2d std::vector of coordinates
       * \param color the optional color of the line (black by default) 
       * \param params VIBes parameters related to the line (none by default)
       */
      void draw_line(const std::vector<std::vector<double> >& v_pts, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a circle
       *
       * \param x horizontal center coordinate
       * \param y vertical center coordinate
       * \param r radius
       * \param params VIBes parameters related to the circle
       */
      void draw_circle(double x, double y, double r, const vibes::Params& params);

      /**
       * \brief Draws a circle
       *
       * \param x horizontal center coordinate
       * \param y vertical center coordinate
       * \param r radius
       * \param color the optional color of the circle (black by default) 
       * \param params VIBes parameters related to the circle (none by default)
       */
      void draw_circle(double x, double y, double r, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a ring
       *
       * \param x horizontal center coordinate
       * \param y vertical center coordinate
       * \param r interval radius
       * \param params VIBes parameters related to the ring
       */
      void draw_ring(double x, double y, const ibex::Interval& r, const vibes::Params& params);

      /**
       * \brief Draws a ring
       *
       * \param x horizontal center coordinate
       * \param y vertical center coordinate
       * \param r interval radius
       * \param color the optional color of the ring (black by default) 
       * \param params VIBes parameters related to the ring (none by default)
       */
      void draw_ring(double x, double y, const ibex::Interval& r, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a pie: radial portion of a ring
       *
       * \param x horizontal center coordinate
       * \param y vertical center coordinate
       * \param r interval radius
       * \param theta interval angle (in radian)
       * \param params VIBes parameters related to the ring
       */
      void draw_pie(double x, double y, const ibex::Interval& r, const ibex::Interval& theta, const vibes::Params& params);

      /**
       * \brief Draws a pie: radial portion of a ring
       *
       * \param x horizontal center coordinate
       * \param y vertical center coordinate
       * \param r interval radius
       * \param theta interval angle (in radian)
       * \param color the optional color of the ring (black by default) 
       * \param params VIBes parameters related to the ring (none by default)
       */
      void draw_pie(double x, double y, const ibex::Interval& r, const ibex::Interval& theta, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws an edge
       *
       * \param e edge
       * \param params VIBes parameters related to the edge (none by default)
       */
      void draw_edge(const Edge& e, const vibes::Params& params);

      /**
       * \brief Draws an edge
       *
       * \param e edge
       * \param color the optional color of the edge (black by default) 
       * \param params VIBes parameters related to the edge (none by default)
       */
      void draw_edge(const Edge& e, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a polygon
       *
       * \param p polygon
       * \param params VIBes parameters related to the polygon (none by default)
       */
      void draw_polygon(const Polygon& p, const vibes::Params& params);

      /**
       * \brief Draws a polygon
       *
       * \param p polygon
       * \param color the optional color of the polygon (black by default) 
       * \param params VIBes parameters related to the polygon (none by default)
       */
      void draw_polygon(const Polygon& p, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a set of polygons
       *
       * \param v_p vector of polygons
       * \param params VIBes parameters related to the polygons (none by default)
       */
      void draw_polygons(const std::vector<ConvexPolygon>& v_p, const vibes::Params& params);

      /**
       * \brief Draws a set of polygons
       *
       * \param v_p vector of polygons
       * \param color the optional color of the polygons (black by default) 
       * \param params VIBes parameters related to the polygons (none by default)
       */
      void draw_polygons(const std::vector<ConvexPolygon>& v_p, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a set of polygons
       *
       * \param v_p vector of polygons
       * \param color_map color map to draw polygons with a map of colors
       * \param params VIBes parameters related to the polygons (none by default)
       */
      void draw_polygons(const std::vector<ConvexPolygon>& v_p, const ColorMap& color_map, const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a point
       *
       * \param p the 2d Point to be displayed
       * \param params VIBes parameters related to the point
       */
      void draw_point(const Point& p, const vibes::Params& params);

      /**
       * \brief Draws a point
       *
       * \param p the 2d Point to be displayed
       * \param color the optional color of the point (black by default) 
       * \param params VIBes parameters related to the point (none by default)
       */
      void draw_point(const Point& p, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a point
       *
       * \param p the 2d Point to be displayed
       * \param size display size of the points
       * \param params VIBes parameters related to the point
       */
      void draw_point(const Point& p, float size, const vibes::Params& params);

      /**
       * \brief Draws a point
       *
       * \param p the 2d Point to be displayed
       * \param size display size of the points
       * \param color the optional color of the point (black by default) 
       * \param params VIBes parameters related to the point (none by default)
       */
      void draw_point(const Point& p, float size, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /**
       * \brief Draws a set of points
       *
       * \param v_pts vector of Point objects
       * \param size display size of the points
       * \param params VIBes parameters related to the set (none by default)
       */
      void draw_points(const std::vector<Point>& v_pts, float size, const vibes::Params& params);

      /**
       * \brief Draws a set of points
       *
       * \param v_pts vector of Point objects
       * \param size display size of the points
       * \param color the optional color of the set (black by default) 
       * \param params VIBes parameters related to the set (none by default)
       */
      void draw_points(const std::vector<Point>& v_pts, float size, const std::string& color = "", const vibes::Params& params = vibes::Params());

      /// @}
  };
}

#endif