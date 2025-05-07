/** 
 *  \file codac2_Figure2DInterface.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <string>
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_StyleProperties.h"
#include "codac2_Ellipsoid.h"
#include "codac2_SampledTraj.h"
#include "codac2_AnalyticTraj.h"

namespace codac2
{
  /**
   * \class Figure2DInterface
   * \brief Interface for 2D figures
   * 
   * This class is used to display 2D figures. It defines the basic drawing functions.
   * 
   * This class is purely virtual and should not be used directly. It is used as an interface for the Figure2D class.
   * 
   * This interface contains the “low-level” methods that need to be overloaded
   * to implement graphic primitives in the output view. The Figure2D class may
   * contain methods not listed here: these methods are more “high-level” and do
   * not require a specific implementation related to the graphic tool.
   * For example, Figure2D::draw_line calls Figure2D::draw_polyline (a line is a
   * special case of a two-point polyline), so only draw_polyline needs to be
   * implemented in this interface.
  */
  class Figure2DInterface
  {
    public:

      // Geometric shapes

      /**
       * \brief Draws a point on the figure
       * 
       * \param c Coordinates of the point
       * \param s Style of the point (edge color and fill color)
       */
      virtual void draw_point(const Vector& c, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a box on the figure
       * 
       * \param x Box to draw
       * \param s Style of the box (edge color and fill color)
       */
      virtual void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a circle on the figure
       * 
       * \param c Center of the circle
       * \param r Radius of the circle
       * \param s Style of the circle (edge color and fill color)
       */
      virtual void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a ring on the figure
       * 
       * \param c Center of the ring
       * \param r Inner and outer radius of the ring
       * \param s Style of the ring (edge color and fill color)
       */
      virtual void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a polyline on the figure
       * 
       * \param x Vector of the points of the polyline
       * \param tip_length Length of the tip of the arrow
       * \param s Style of the polyline (edge color and fill color)
       */
      virtual void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a polygone on the figure
       * 
       * \param x Vector of the points of the polygone
       * \param s Style of the polygone (edge color and fill color)
       */
      virtual void draw_polygon(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a pie on the figure
       * 
       * \param c Center of the pie
       * \param r Inner and outer radius of the pie
       * \param theta Start and end angle of the pie (in radians)
       * \param s Style of the pie (edge color and fill color)
       */
      virtual void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws an ellipse on the figure
       * 
       * \param c Center of the ellipse
       * \param ab Half-lengths of the ellipse
       * \param theta Rotation angle of the ellipse (in radians)
       * \param s Style of the ellipse (edge color and fill color)
       */
      virtual void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s = StyleProperties()) = 0;

      // Robots

      /**
       * \brief Draws a tank on the figure
       * 
       * \param x Coordinates of the tank
       * \param size Size of the tank
       * \param s Style of the tank (edge color and fill color)
       */
      virtual void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws an AUV on the figure
       * 
       * \param x Coordinates of the AUV
       * \param size Size of the AUV
       * \param s Style of the AUV (edge color and fill color)
       */
      virtual void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties()) = 0;

      /**
       * \brief Draws a motor boat on the figure
       * 
       * \param x Coordinates of the motor boat
       * \param size Size of the motor boat
       * \param s Style of the motor boat (edge color and fill color)
       */
      virtual void draw_motor_boat(const Vector& x, float size, const StyleProperties& s = StyleProperties()) = 0;
  };
}