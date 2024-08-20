/** 
 *  \file codac2_Figure2D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "codac2_Figure2DInterface.h"
#include "codac2_OutputFigure2D.h"

namespace codac2
{
  enum class GraphicOutput
  {
    VIBES = 0x01,
    IPE = 0x02
  };

  inline int operator&(GraphicOutput a, GraphicOutput b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }

  inline GraphicOutput operator|(GraphicOutput a, GraphicOutput b)
  { return static_cast<GraphicOutput>(static_cast<int>(a) | static_cast<int>(b)); }

  struct FigureAxis
  {
    size_t dim_id;
    Interval limits;
    std::string label;
  };

  inline FigureAxis axis(size_t dim_id, const Interval& limits, const std::string& label = "")
  {
    assert_release(dim_id >= 0);
    //assert_release(!limits.is_empty());

    std::string axis_label = label;
    if(axis_label.empty())
      axis_label = "x_" + std::to_string(dim_id);

    return { dim_id, limits, axis_label };
  }

  class DefaultView;

  class Figure2D : public Figure2DInterface
  {
    public:

      Figure2D(const std::string& name, GraphicOutput o, bool set_as_default = false);

      const std::string& name() const;
      size_t size() const;

      const std::vector<FigureAxis>& axes() const;
      void set_axes(const FigureAxis& axis1, const FigureAxis& axis2);

      const Vector& pos() const;
      const Vector& window_size() const;
      void set_window_properties(const Vector& pos, const Vector& size);

      void center_viewbox(const Vector& c, const Vector& r);
      double scaled_unit() const;

      bool is_default() const;
      void set_as_default();

      // Geometric shapes
      void draw_point(const Vector& c, const StyleProperties& s = StyleProperties());
      void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties());
      void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties());
      void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties());
      void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties());

      // Robots
      void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties());
      void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties());

    protected:

      const std::string _name;
      Vector _pos {50,50}, _window_size {500,500};
      std::vector<FigureAxis> _axes { axis(0,{0,1}), axis(1,{0,1}) };
      std::vector<std::shared_ptr<OutputFigure2D>> _output_figures;

      friend DefaultView;
  };

  class DefaultView
  {
    public:

      static Figure2D* selected_fig()
      {
        if(_selected_fig == nullptr && _default_fig.get() == nullptr)
        {
          _default_fig = std::make_shared<Figure2D>("Codac - default view", GraphicOutput::VIBES);
          _default_fig->set_window_properties({20.,20.}, {800.,800.});
          _default_fig->set_axes(axis(0,{-10,10}),axis(1,{-10,10}));
          _selected_fig = _default_fig.get();
        }
        
        return _selected_fig;
      }

      static void set(Figure2D *fig)
      {
        _selected_fig = fig;
      }
      
      static void set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
      {
        selected_fig()->set_axes(axis1,axis2);
      }
      
      static void set_window_properties(const Vector& pos, const Vector& size)
      {
        selected_fig()->set_window_properties(pos,size);
      }

      // Geometric shapes

      static void draw_point(const Vector& c, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_point(c,s);
      }

      static void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_box(x,s);
      }

      static void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_circle(c,r,s);
      }

      static void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_ring(c,r,s);
      }

      static void draw_polyline(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_polyline(x,s);
      }

      static void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_polyline(x,tip_length,s);
      }

      static void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_polygone(x,s);
      }

      static void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_pie(c,r,theta,s);
      }

      // Robots

      static void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_tank(x,size,s);
      }

      static void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties())
      {
        selected_fig()->draw_AUV(x,size,s);
      }


    protected:

      friend Figure2D;

      static std::shared_ptr<Figure2D> _default_fig;
      static Figure2D *_selected_fig;
  };
}