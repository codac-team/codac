/** 
 *  \file codac2_Figure3D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <memory>
#include <fstream>
#include "codac2_StyleProperties.h"
#include "codac2_IntervalVector.h"
#include "codac2_Paving.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Ellipsoid.h"

namespace codac2
{

  /**
   * \class Figure3D
   * \brief Figure3D class, used for 3D figures
   * 
   * This class is used to display 3D figures. It generates an OBJ file that can be opened with a 3D viewer.
   */
  class Figure3D
  {
    public:

      /**
       * \brief Creates a new Figure3D object, with a given name
       * 
       * \param name Name of the figure
       */
      Figure3D(const std::string& name);

      /**
       * \brief Destructor for the Figure3D class
       */
      ~Figure3D();

      /**
       * \brief Getter for the name of the figure
       * 
       * \return The name of the figure
       */
      const std::string& name() const;


      // Geometric shapes

      /**
       * \brief Draws a triangle
       *
       * \param c translation
       * \param A scaling
       * \param p1 first point
       * \param p2 second point
       * \param p3 third point
       * \param s style
       */
      void draw_triangle(const Vector &c, const Matrix &A,
		const Vector &p1, const Vector &p2,
		const Vector &p3, 
		const StyleProperties& s = { Color::dark_gray(0.5) });
		 
      /**
       * \brief Draws a triangle, shorter version
       *
       * \param p1 first point
       * \param p2 second point
       * \param p3 third point
       * \param s style
       */
      void draw_triangle(const Vector &p1, const Vector &p2,
		const Vector &p3, 
		const StyleProperties& s = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a ``star-shaped'' polygon  as a sequence of adjacent
       * triangles (l[0],l[k],l[k+1]) with k>=1.
       *
       * \param c translation
       * \param A scaling
       * \param l points 
       * \param s style
       */
      void draw_polygon(const Vector &c, const Matrix &A,
		const std::vector<Vector> &l, 
		const StyleProperties& s = { Color::dark_gray(0.5) });

		 
      /**
       * \brief Draws a parallelogram c + A (p + [-1,1]*v1 + [-1,1]*v2) 
       *
       * \param c translation
       * \param A scaling
       * \param p base point
       * \param v1 vector
       * \param v2 vector
       * \param s style
       */
      void draw_parallelogram(const Vector &c, const Matrix &A,
	        const Vector &p, const Vector &v1, const Vector &v2,
		const StyleProperties& s = { Color::dark_gray(0.5) });
		 
      /**
       * \brief Draws a parallelepiped z+A*[-1,1]^3 on the figure
       * 
       * \param z Coordinates of the center of the parallelepiped
       * \param A Matrix of the parallelepiped
       * \param s Style of the parallelepiped (edge color)
       */
      void draw_parallelepiped(const Vector& z, const Matrix& A, const StyleProperties& s = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a zonotope z+sum_i [-1,1] A_i on the figure
       * 
       * \param z Coordinates of the center of the zonotope
       * \param A list of vectors 
       * \param s Style of the zonotope (edge color)
       */
      void draw_zonotope(const Vector& z, const std::vector<Vector>& A, const StyleProperties& s = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a box on the figure
       * 
       * \param x Box to draw
       * \param s Style of the box (edge color)
       */      
      void draw_box(const IntervalVector& x, const StyleProperties& s = { Color::dark_gray(0.5) });


      /**
       * \brief Draws an arrow (box c + A * ([0,1],[-0.01,0.01],[-0.01,0.01]) and a
       * tip at the end)
       * 
       * \param c start
       * \param A orientation (first column)
       * \param s Style (color)
       */
      void draw_arrow(const Vector &c, const Matrix& A, const StyleProperties& s = { Color::dark_gray(0.5) });


      /**
       * \brief Draws the (x,y,z) axes on the figure in red, green and blue
       * 
       * \param size Size of the axes
       */
      void draw_axes(double size = 1.0);

      /**
       * \brief Draws a parametric surface
       * 
       * \param c translation
       * \param A scaling
       * \param Ip1 bounds of p1
       * \param dp1 incrementation for p1
       * \param Ip2 bounds of p2
       * \param dp2 incrementation for p2
       * \param f function computing the values
       * \param s Style (color)
       */
      void draw_surface(const Vector &c, const Matrix &A,
		const Interval &Ip1, double dp1,
                const Interval &Ip2, double dp2,
		std::function<Vector(double,double)> f,
		const StyleProperties& s = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a sphere (ellipsoid)
       *
       * \param c translation
       * \param A scaling
       * \param s Style (color)
       */
      void draw_sphere(const Vector &c, const Matrix &A,
		const StyleProperties& s = { Color::dark_gray(0.5) });

      /**
       * \brief Draws an ellipsoid (from the Ellipsoid class)
       *
       * \param e Ellipsoid to draw
       * \param s Style (color)
       */
      void draw_ellipsoid(const Ellipsoid &e,
		const StyleProperties& s = { Color::dark_gray(0.5) });

      /** 
       * \brief Draws a car, with blue windscreen
       *
       * \param c ``center'' (low) of the car
       * \param A orientation
       * \param s Style (color)
       */
      void draw_car(const Vector &c, const Matrix &A,
		const StyleProperties& s = { Color::yellow(0.5) });
     
      /** 
       * \brief Draws a (paper) plane
       *
       * \param c ``center'' (low) of the car
       * \param A orientation
       * \param yaw_is_up yaw axis is up (default true)
       * \param s Style (color)
       */
      void draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up=true,
		const StyleProperties& s = { Color::dark_gray(0.8) });
     

      // Pavings

      /**
       * \brief Draws a paving on the figure (Only the boundary is drawn).
       * 
       * \param p PavingOut to draw (result of a paving with contractors). 
       * \param bound_s Style of the boundary of the paving
       */
      void draw_paving(const PavingOut& p,
        const StyleProperties& bound_s = { Color::yellow(0.5) });

      /**
       * \brief Draws a paving on the figure (Only the boundary and the inside is drawn).
       * 
       * \param p PavingInOut to draw (result of a paving with separators).
       * \param bound_s Style of the boundary of the paving
       * \param in_s Style of the inside of the paving
       */
      void draw_paving(const PavingInOut& p,
        const StyleProperties& bound_s = { Color::yellow(0.3), "paving_bound" },
        const StyleProperties& in_s = { Color::green(0.5) , "paving_in" });
        
      /**
       * \brief Draws a subpaving on the figure
       * 
       * \param p Subpaving to draw
       * \param s Style of the subpaving
       */
      template<typename P>
      inline void draw_subpaving(const Subpaving<P>& p, const StyleProperties& s = StyleProperties())
      {
        for(const auto& pi : p.boxes())
          draw_box(pi, s);
      }

    private:
      /** 
       * \brief describes a vertex (c+ A p). Increment the vertex_count
       *  
       * \param c center
       * \param A rotation 
       * \param p relative position
       * \return the vertex number
       **/ 
       size_t move_write_v(const Vector &c, const Matrix &A, const Vector &p);

      /**
       * \brief Sets the current color.
       * 
       * \param c color
       */
      void set_color_internal(const Color &c);
      
      /**
       * \brief Sets the style (layer+color), if lock_style is false.
       * Otherwise, do nothing. If the layer is empty of "alpha", use the
       * name of the figure.
       * 
       * \param s layer. If empty or "alpha", use the name of the figure.
       */
      void set_style_internal(const StyleProperties& s);

    protected:

      const std::string _name;
      std::ofstream _file;
      size_t vertex_count = 0;
      bool lock_style=false;
  };
}
