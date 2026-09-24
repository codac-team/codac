/** 
 *  \file codac2_Figure3D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard, Damien Massé, Quentin Brateau
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "codac2_GraphicOutput.h"
#include "codac2_StyleProperties.h"
#include "codac2_PavingStyle.h"
#include "codac2_IntervalVector.h"
#include "codac2_Paving.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_Ellipsoid.h"
#include "codac2_Parallelepiped.h"
#include "codac2_SampledTraj.h"
#include "codac2_AnalyticTraj.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  class OutputFigure3D;
  template<typename P>
  class Subpaving;

  /**
   * \class Figure3D
   * \brief Figure3D class, used for 3D figures
   * 
   * This class is used to display 3D figures. It can interact with OBJ and Rerun.
   */
  class Figure3D : public std::enable_shared_from_this<Figure3D>
  {
    public:

      /**
       * \brief Creates a new Figure3D object, with a given name and output backend
       * 
       * \param name Name of the figure
       * \param o Graphic output backend (OBJ, RERUN, or combination)
       */
      Figure3D(const std::string& name, GraphicOutput o = GraphicOutput::OBJ | GraphicOutput::RERUN);

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

      /**
       * \brief Returns OutputFigure3D objects rendering the current figure
       */
      std::vector<std::shared_ptr<OutputFigure3D>> output_figures();

      /**
       * \brief Clears the figure
       */
      void clear();

      /**
       * \brief Saves the figure to a file
       * 
       * \param filename Name of the file to save the figure to
       */
      void save(const std::string& filename);


      // Geometric shapes

      /**
       * \brief Draws a 3D point on the figure
       * 
       * \param c Coordinates of the 3D point (Vector of dimension >= 3)
       * \param style Style properties (color, line width, layer)
       */
      void draw_point(const Vector& c, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a line between two 3D points
       * 
       * \param p1 Starting 3D point
       * \param p2 Ending 3D point
       * \param style Style properties (stroke color, line width, layer)
       */
      void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a 3D polyline connecting a sequence of points
       * 
       * \param x Vector of 3D points forming the polyline
       * \param style Style properties (stroke color, line width, layer)
       */
      void draw_polyline(const std::vector<Vector>& x, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a triangle with coordinate transformation
       *
       * \param c Translation vector (center offset)
       * \param A Linear transformation matrix (3x3)
       * \param p1 First local vertex
       * \param p2 Second local vertex
       * \param p3 Third local vertex
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_triangle(const Vector &c, const Matrix &A,
        const Vector &p1, const Vector &p2, const Vector &p3, 
        const StyleProperties& style = { Color::dark_gray(0.5) });
     
      /**
       * \brief Draws a triangle with direct global vertices
       *
       * \param p1 First vertex
       * \param p2 Second vertex
       * \param p3 Third vertex
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_triangle(const Vector &p1, const Vector &p2, const Vector &p3, 
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a star-shaped polygon as a sequence of adjacent
       * triangles (l[0],l[k],l[k+1]) with k>=1.
       *
       * \param c Translation vector (center offset)
       * \param A Linear transformation matrix (3x3)
       * \param l Vector of vertices in local coordinates
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_polygon(const Vector &c, const Matrix &A, const std::vector<Vector> &l, 
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a parallelogram c + A * (p + [-1,1]*v1 + [-1,1]*v2)
       *
       * \param c Translation vector (center offset)
       * \param A Linear transformation matrix (3x3)
       * \param p Base offset point
       * \param v1 First direction vector
       * \param v2 Second direction vector
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_parallelogram(const Vector &c, const Matrix &A,
        const Vector &p, const Vector &v1, const Vector &v2,
        const StyleProperties& style = { Color::dark_gray(0.5) });
     
      /**
       * \brief Draws a parallelepiped c + A * [-1,1]^3 on the figure
       * 
       * \param p Parallelepiped to draw (center c and generator matrix A)
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a zonotope c + sum_i [-1,1] * A_i on the figure
       * 
       * \param z Zonotope to draw (center and generator matrix columns)
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_zonotope(const Zonotope& z, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws an axis-aligned box (IntervalVector) on the figure
       * 
       * \param x 3D interval box to draw
       * \param style Style properties (fill/stroke color, layer)
       */      
      void draw_box(const IntervalVector& x, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a 3D oriented arrow
       * 
       * \param c Starting origin position of the arrow
       * \param A Orientation/direction matrix (first column is the direction vector)
       * \param style Style properties (stroke color, line width, layer)
       */
      void draw_arrow(const Vector &c, const Matrix& A, const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws the (x,y,z) coordinate axes on the figure in red, green and blue
       * 
       * \param size Length of the coordinate axis arrows
       * \param origin Origin position of the axes (default: (0,0,0))
       */
      void draw_axes(double size = 1.0, const Vector& origin = Vector::Zero(3));

      /**
       * \brief Draws a parametric 3D surface
       * 
       * \param c Translation vector (center offset)
       * \param A Linear transformation matrix (3x3)
       * \param Ip1 Parameter 1 interval domain [min, max]
       * \param dp1 Discretization step size for parameter 1
       * \param Ip2 Parameter 2 interval domain [min, max]
       * \param dp2 Discretization step size for parameter 2
       * \param f Parametric mapping function (p1, p2) -> Vector (3D coordinates)
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_surface(const Vector &c, const Matrix &A,
        const Interval &Ip1, double dp1,
        const Interval &Ip2, double dp2,
        std::function<Vector(double,double)> f,
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a transformed sphere / ellipsoid mesh
       *
       * \param c Center position of the sphere
       * \param A Scaling and orientation transformation matrix (3x3)
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_sphere(const Vector &c, const Matrix &A,
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws an ellipsoid from an Ellipsoid domain object
       *
       * \param e Ellipsoid object (mean center vector and transformation matrix)
       * \param style Style properties (fill/stroke color, layer)
       */
      void draw_ellipsoid(const Ellipsoid &e,
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /** 
       * \brief Draws a 3D car model with windscreen
       *
       * \param c Center position of the car
       * \param A Scaling and orientation transformation matrix (3x3)
       * \param style Style properties (body color, layer)
       */
      void draw_car(const Vector &c, const Matrix &A,
        const StyleProperties& style = { Color::yellow(0.5) });
     
      /** 
       * \brief Draws a 3D (paper) airplane model
       *
       * \param c Center position of the plane
       * \param A Scaling and orientation transformation matrix (3x3)
       * \param yaw_is_up Whether the vertical yaw axis is pointing upwards (default: true)
       * \param style Style properties (body color, layer)
       */
      void draw_plane(const Vector &c, const Matrix &A, bool yaw_is_up=true,
        const StyleProperties& style = { Color::dark_gray(0.8) });

      /**
       * \brief Draws a 3D Autonomous Underwater Vehicle (AUV / submarine) model
       * 
       * \param c Center position of the AUV
       * \param A Scaling and orientation transformation matrix (3x3)
       * \param style Style properties (body color, layer)
       */
      void draw_AUV(const Vector &c, const Matrix &A,
        const StyleProperties& style = { Color::yellow(0.5) });

      // Pavings

      /**
       * \brief Draws a paving on the figure (boundary boxes from contractor paving)
       * 
       * \param p PavingOut object to draw (result of a paving with contractors)
       * \param bound_style Style properties of the boundary boxes (color, layer)
       */
      void draw_paving(const PavingOut& p,
        const StyleProperties& bound_style = { Color::yellow(0.5), "paving_bound" });

      /**
       * \brief Draws a paving on the figure with a PavingStyle object (boundary only)
       * 
       * \param p PavingOut object to draw
       * \param style PavingStyle properties controlling boundary style and layer
       */
      void draw_paving(const PavingOut& p,
        const PavingStyle& style);

      /**
       * \brief Draws a paving on the figure (boundary and inside boxes from separator paving)
       * 
       * \param p PavingInOut object to draw (result of a paving with separators)
       * \param bound_style Style properties of the boundary boxes (color, layer)
       * \param in_style Style properties of the inside/interior boxes (color, layer)
       */
      void draw_paving(const PavingInOut& p,
        const StyleProperties& bound_style = { Color::yellow(0.3), "paving_bound" },
        const StyleProperties& in_style = { Color::green(0.5) , "paving_in" });

      /**
       * \brief Draws a paving on the figure with a PavingStyle object (boundary and inside)
       * 
       * \param p PavingInOut object to draw
       * \param style PavingStyle properties controlling boundary and inside styles and layers
       */
      void draw_paving(const PavingInOut& p,
        const PavingStyle& style);
        
      /**
       * \brief Draws a subpaving on the figure
       * 
       * \param p Subpaving object containing the collection of boxes
       * \param style Style properties for the boxes (color, layer)
       */
      template<typename P>
      inline void draw_subpaving(const Subpaving<P>& p, const StyleProperties& style = StyleProperties())
      {
        for(const auto& pi : p.boxes())
          draw_box(pi, style);
      }

      // Trajectories & Tubes

      /**
       * \brief Draws a sampled trajectory in 3D
       * 
       * \param x Sampled trajectory object containing time-indexed 3D points
       * \param style Style properties (stroke color, line width, layer)
       */
      void draw_trajectory(const SampledTraj<Vector>& x,
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws an analytic trajectory in 3D
       * 
       * \param x Analytic trajectory object evaluated over a time domain
       * \param style Style properties (stroke color, line width, layer)
       */
      void draw_trajectory(const AnalyticTraj<VectorType>& x,
        const StyleProperties& style = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a tube of IntervalVector in 3D
       * 
       * \param x SlicedTube object containing 3D interval box slices over time
       * \param style Style properties (box stroke/fill color, layer)
       */
      void draw_tube(const SlicedTube<IntervalVector>& x,
        const StyleProperties& style = { Color::dark_gray(0.5) });

    protected:

      const std::string _name;
      std::vector<std::shared_ptr<OutputFigure3D>> _output_figures;
  };
}
