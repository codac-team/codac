/** 
 *  \file codac2_Figure3D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard
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

      /**
       * \brief Draws the (x,y,z) axes on the figure in red, green and blue
       * 
       * \param size Size of the axes
       */
      void draw_axes(double size = 1.0);


      // Geometric shapes

      /**
       * \brief Draws a box on the figure
       * 
       * \param x Box to draw
       * \param s Style of the box (edge color)
       */      
      void draw_box(const IntervalVector& x, const StyleProperties& s = { Color::dark_gray(0.5) });

      /**
       * \brief Draws a parallelepiped z+A*[-1,1]^3 on the figure
       * 
       * \param z Coordinates of the center of the parallelepiped
       * \param A Matrix of the parallelepiped
       * \param s Style of the parallelepiped (edge color)
       */
      void draw_parallelepiped(const Vector& z, const Matrix& A, const StyleProperties& s = { Color::dark_gray(0.5) });

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
        const StyleProperties& bound_s = { Color::yellow(0.3) },
        const StyleProperties& in_s = { Color::green(0.5) });
        
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

    protected:

      const std::string _name;
      std::ofstream _file;
      size_t vertex_count = 0;
  };
}