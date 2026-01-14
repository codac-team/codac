/** 
 *  \file codac2_Figure2D_VIBes.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Figure2D.h"
#include "codac2_OutputFigure2D.h"
#include "codac2_IntervalVector.h"
#include "vibes.h"

namespace codac2
{
  /**
   * \class Figure2D_VIBes
   * \brief VIBes output class
   * 
   * This class is used to manage the VIBes output. It is meant to be used with the VIBes server.
   */
  class Figure2D_VIBes : public OutputFigure2D
  {
    public:

      /**
       * \brief Creates a new Figure2D_VIBes object linked to a given figure
       * 
       * \param fig Figure2D to use
       */
      Figure2D_VIBes(const Figure2D& fig);

      /**
       * \brief Destructor for the Figure2D_VIBes object
       */
      virtual ~Figure2D_VIBes();

      /**
       * \brief Updates the axes of the figure
       */
      void update_axes();

      /**
       * \brief Updates the drawing properties : colors, line style, line width and layer
       */
      void update_drawing_properties(const StyleProperties& s);

      /**
       * \brief Updates the window properties of the figure
       */
      void update_window_properties();

      /**
       * \brief Centers the viewbox on a given point with a given radius
       * 
       * \param c Center of the viewbox
       * \param r Radius of the viewbox
       */
      void center_viewbox(const Vector& c, const Vector& r);

      /**
       * \brief Clears the figure
       */
      void clear();
      
      // Geometric shapes

      /**
       * \brief Draws a point on the figure
       * 
       * \param c Coordinates of the point
       * \param style Style of the point (edge color and fill color)
       */
      void draw_point(const Vector& c, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a box on the figure
       * 
       * \param x Box to draw
       * \param style Style of the box (edge color and fill color)
       */
      void draw_box(const IntervalVector& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a circle on the figure
       * 
       * \param c Center of the circle
       * \param r Radius of the circle
       * \param style Style of the circle (edge color and fill color)
       */
      void draw_circle(const Vector& c, double r, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a ring on the figure
       * 
       * \param c Center of the ring
       * \param r Inner and outer radius of the ring
       * \param style Style of the ring (edge color and fill color)
       */
      void draw_ring(const Vector& c, const Interval& r, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a polyline on the figure
       * 
       * \param x Vector of the points of the polyline
       * \param tip_length Length of the tip of the arrow
       * \param style Style of the polyline (edge color and fill color)
       */
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a polygone on the figure
       * 
       * \param x Vector of the points of the polygone
       * \param style Style of the polygone (edge color and fill color)
       */
      void draw_polygon(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a pie on the figure
       * 
       * \param c Center of the pie
       * \param r Inner and outer radius of the pie
       * \param theta Start and end angle of the pie (in radians)
       * \param style Style of the pie (edge color and fill color)
       */
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws an ellipse on the figure
       * 
       * \param c Center of the ellipse
       * \param ab Half-lengths of the ellipse
       * \param theta Rotation angle of the ellipse (in radians)
       * \param style Style of the ellipse (edge color and fill color)
       */
      void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& style = StyleProperties());

      // Robots

      /**
       * \brief Draws a tank on the figure
       * 
       * \param x Coordinates of the tank
       * \param size Size of the tank
       * \param style Style of the tank (edge color and fill color)
       */
      void draw_tank(const Vector& x, float size, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws an AUV on the figure
       * 
       * \param x Coordinates of the AUV
       * \param size Size of the AUV
       * \param style Style of the AUV (edge color and fill color)
       */
      void draw_AUV(const Vector& x, float size, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a motor boat on the figure
       * 
       * \param x Coordinates of the motor boat
       * \param size Size of the motor boat
       * \param style Style of the motor boat (edge color and fill color)
       */
      void draw_motor_boat(const Vector& x, float size, const StyleProperties& style = StyleProperties());

      // Miscellaneous

      /**
       * \brief Draws text on the figure
       * 
       * \param text Text to display
       * \param ul Position of the top-left corner of the text
       * \param scale Scaling of the text (VIBes only)
       * \param style Style of the text (color, layer)
       */
      void draw_text(const std::string& text, const Vector& ul, double scale, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a raster on the figure
       * 
       * \param filename The name of the file, the path is relative to the VIBes' server folder
       * \param bbox The bounding box of the raster
       * \param style Style of the raster (only the layer is used)
       */
      void draw_raster(const std::string& filename, const IntervalVector& bbox, const StyleProperties& style = StyleProperties());



      /**
       * \brief Converts a StyleProperties object to a VIBes style string
       * 
       * \param style StyleProperties object to convert
       * 
       * \return VIBes style string
       */
      static std::string to_vibes_style(const StyleProperties& style);

    protected:

      static int _has_been_initialized;
      vibes::Params _params;
  };
}