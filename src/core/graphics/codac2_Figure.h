/** 
 *  \file
 *  Figure class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "codac2_FigureInterface.h"
#include "codac2_OutputFigure.h"

namespace codac2
{
  enum class GraphicOutputMode
  {
    VIBES = 0x01,
    IPE = 0x02
  };

  inline int operator&(GraphicOutputMode a, GraphicOutputMode b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }

  inline GraphicOutputMode operator|(GraphicOutputMode a, GraphicOutputMode b)
  { return static_cast<GraphicOutputMode>(static_cast<int>(a) | static_cast<int>(b)); }

  struct FigureAxis
  {
    size_t dim_id;
    Interval limits;
    std::string label;
  };

  inline FigureAxis axis(size_t dim_id, const Interval& limits, const std::string& label = "")
  {
    assert(dim_id >= 0);
    assert(!limits.is_empty());

    std::string axis_label = label;
    if(axis_label.empty())
      axis_label = "x_" + std::to_string(dim_id);

    return { dim_id, limits, axis_label };
  }

  class Figure : public FigureInterface
  {
    public:

      Figure(const std::string& name, GraphicOutputMode o);

      size_t size() const;
      const Vector& pos() const;
      const Vector& window_size() const;
      const std::string& name() const;
      const std::vector<FigureAxis>& axes() const;

      void set_as_default();
      void set_axes(const FigureAxis& axis1, const FigureAxis& axis2);
      void set_window_properties(const Vector& pos, const Vector& size);
      void center_viewbox(const Vector& c, const Vector& r);

      void draw_box(const IntervalVector& x, const std::string& color = "black");

    protected:

      const std::string _name;
      Vector _pos {50,50}, _window_size {500,500};
      std::vector<FigureAxis> _axes { axis(0,{0,1}), axis(1,{0,1}) };
      std::vector<std::shared_ptr<OutputFigure>> _output_figures;
  };

  class DefaultFigure
  {
    public:

      static void set(Figure *fig)
      {
        _default_figure = fig;
      }

      static void draw_box(const IntervalVector& x, const std::string& color = "black")
      {
        if(_default_figure)
          _default_figure->draw_box(x,color);
      }

    protected:

      static Figure *_default_figure;
  };
}