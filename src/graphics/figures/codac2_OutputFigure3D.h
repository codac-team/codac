/** 
 *  \file codac2_OutputFigure3D.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include "codac2_Figure3DInterface.h"

namespace codac2
{
  class Figure3D;

  /**
   * \class OutputFigure3D
   * \brief Base class for 3D output figures
   */
  class OutputFigure3D : public Figure3DInterface
  {
    public:
      using Figure3DInterface::draw_triangle;

      /**
       * \brief Creates a new OutputFigure3D object linked to a given figure
       * 
       * \param fig Figure3D to use
       */
      OutputFigure3D(const Figure3D& fig)
        : _fig(fig)
      { }

      virtual ~OutputFigure3D() = default;

      /**
       * \brief Accessor to the associated Figure3D
       */
      const Figure3D& fig() const { return _fig; }

      /**
       * \brief Updates the axes of the figure
       */
      virtual void update_axes() = 0;

      /**
       * \brief Clears the figure
       */
      virtual void clear() = 0;
      
      /**
       * \brief Saves the figure to a file
       * 
       * \param filename Name of the file to save the figure to
       */
      virtual void save(const std::string& filename) = 0;

      // Default implementations of compound drawing methods
      void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style = StyleProperties()) override;
      void draw_triangle(const Vector &p1, const Vector &p2, const Vector &p3, const StyleProperties& style = StyleProperties()) override;
      void draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& style = StyleProperties()) override;
      void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& style = StyleProperties()) override;
      void draw_tube(const SlicedTube<IntervalVector>& x, const StyleProperties& style = StyleProperties()) override;
      void draw_paving(const PavingOut& p, const StyleProperties& bound_style = StyleProperties()) override;
      void draw_paving(const PavingInOut& p, const StyleProperties& bound_style = StyleProperties(), const StyleProperties& in_style = StyleProperties()) override;

    protected:

      const Figure3D& _fig;
      std::vector<std::string> _layers;
  };
}
