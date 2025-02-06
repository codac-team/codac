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

  class Figure3D
  {
    public:

      Figure3D(const std::string& name);
      ~Figure3D();

      const std::string& name() const;

      void draw_axes(double size = 1.0);


      // Geometric shapes
      void draw_box(const IntervalVector& x, const StyleProperties& s = { Color::dark_gray(0.5) });
      void draw_parallelepiped(const Vector& z, const Matrix& A, const StyleProperties& s = { Color::dark_gray(0.5) });

      // Pavings
      void draw_paving(const PavingOut& p,
        const StyleProperties& bound_s = { Color::yellow(0.5) });
      void draw_paving(const PavingInOut& p,
        const StyleProperties& bound_s = { Color::yellow(0.3) },
        const StyleProperties& in_s = { Color::green(0.5) });

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