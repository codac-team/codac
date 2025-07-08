/** 
 *  \file codac2_Figure2D_IPE.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <fstream>
#include "codac2_Figure2D.h"
#include "codac2_OutputFigure2D.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Ellipsoid.h"
#include "vibes.h"

namespace codac2
{
  /**
   * \class Figure2D_IPE
   * \brief IPE output class
   * 
   * This class is used to manage the IPE output. It generates an XML file that can be opened with the IPE editor.
   */
  class Figure2D_IPE : public OutputFigure2D
  {
    public:

      /**
       * \brief Creates a new Figure2D_IPE object linked to a given figure
       * 
       * \param fig Figure2D to use
       */
      Figure2D_IPE(const Figure2D& fig);

      /**
       * \brief Destructor for the Figure2D_IPE object
       */
      virtual ~Figure2D_IPE();

      /**
       * \brief Updates the axes of the figure
       */
      void update_axes();

      /**
       * \brief Updates the drawing properties : colors, line style, line width and layer
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
       * 
       * This function clears the figure and resets the drawing properties.
       */
      void clear();

      /**
       * \brief Begins a new path in the IPE file
       * 
       * \param style the style properties for the path (color, line width, etc.)
       * \param tip if true, the path will be drawn with a tip (default is false)
       */
      void begin_path(const StyleProperties& style,bool tip = false);

      /* For future doc:
      https://github.com/codac-team/codac/pull/126#discussion_r1829030491
      Pour les véhicules (draw_tank et draw_AUV) le header par défaut du begin_path n'est pas suffisant.
      J'ai donc ajouté cette fonction qui fait le même travail que le begin_path, avec en plus le champ
      "matrix" complété.
      Ce champ contient 6 valeurs : les 4 premières sont la matrice de transformation 2D, rotation et
      dilatation, "par colonne" (i.e. m11, m21, m12, m22) et les 2 autres valeurs sont la translation.
      Le tout permet de scale le véhicule, l'orienter et le déplacer au bon endroit.
      Cette fonction écrit dans le xml quelque chose dans le style :
         <path layer="alpha" 
         stroke="codac_color_000000" 
         fill="codac_color_ffd32a" 
         opacity="100%" 
         stroke-opacity="100%" 
         pen="heavier" 
         matrix="0.00948009 11.9048 -11.9047 0.00948009 166.667 166.667">
      */

     /**
      * \brief Begins a new path in the IPE file with a transformation matrix, used for vehicles
      * 
      * \param x center and orientation of the path (first two elements are the center, next element is the angle in radians)
      * \param length Length of the path
      * \param style Style properties for the path (color, line width, etc.)
      */
      void begin_path_with_matrix(const Vector& x, float length, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a text on the figure
       * 
       * \param c Coordinates of the text
       * \param r Rotation of the text (in radians)
       * \param text Text to draw
       * \param style Style properties for the text (font size, color, etc.)
       */
      void draw_text(const Vector& c, const Vector& r, const std::string& text, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws the axes of the figure
       */
      void draw_axes();

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

    protected:

      double scale_x(double x) const;
      double scale_y(double y) const;
      double scale_length(double y) const;
      void print_header_page();

      std::ofstream _f, _f_temp_content;
      const double _ipe_grid_size = 500.;
      Vector _ratio { 1., 1. };

      std::vector<double> _x_ticks;
      std::vector<double> _y_ticks;
      double _x_offset;
      double _y_offset;

      std::map<std::string,Color> _colors;
  };
}