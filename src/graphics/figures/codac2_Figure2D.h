/** 
 *  \file codac2_Figure2D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "codac2_Index.h"
#include "codac2_Figure2DInterface.h"
#include "codac2_OutputFigure2D.h"
#include "codac2_Paving.h"
#include "codac2_ColorMap.h"
#include "codac2_Ellipsoid.h"

#define DEFAULT_FIG_NAME "Codac - default view"

namespace codac2
{
  enum class GraphicOutput
  {
    VIBES = 0x01,
    IPE = 0x02
  };

  constexpr int operator&(GraphicOutput a, GraphicOutput b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }

  constexpr GraphicOutput operator|(GraphicOutput a, GraphicOutput b)
  { return static_cast<GraphicOutput>(static_cast<int>(a) | static_cast<int>(b)); }

  struct FigureAxis
  {
    Index dim_id;
    Interval limits;
    std::string label;
  };

  inline FigureAxis axis(Index dim_id, const Interval& limits, const std::string& label = "")
  {
    assert_release(dim_id >= 0);
    //assert_release(!limits.is_empty());

    std::string axis_label = label;
    if(axis_label.empty())
      axis_label = "x_" + std::to_string(dim_id);

    return { dim_id, limits, axis_label };
  }

  class DefaultView;
  class PavingOut;
  class PavingInOut;
  template<typename P>
  class Subpaving;

  class Figure2D : public Figure2DInterface, public std::enable_shared_from_this<Figure2D>
  {
    public:

      Figure2D(const std::string& name, GraphicOutput o, bool set_as_default = false);

      const std::string& name() const;
      Index size() const;

      const std::vector<FigureAxis>& axes() const;
      void set_axes(const FigureAxis& axis1, const FigureAxis& axis2);

      const Index& i() const;
      const Index& j() const;

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
      void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& s = StyleProperties());
      void draw_arrow(const Vector& p1, const Vector& p2, float tip_length, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties());
      void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties());
      void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s = StyleProperties());
      void draw_ellipsoid(const Ellipsoid& e, const StyleProperties& s = StyleProperties());
      void draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& s = StyleProperties());
      void draw_trajectory(const SampledTraj<Vector>& x, const ColorMap& cmap);
      void draw_trajectory(const AnalyticTraj<VectorType>& x, const ColorMap& cmap);

      // Robots
      void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties());
      void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties());

      // Pavings
      void draw_paving(const PavingOut& p,
        const StyleProperties& bound_s = StyleProperties::boundary(),
        const StyleProperties& out_s = StyleProperties::outside());
      void draw_paving(const PavingInOut& p,
        const StyleProperties& bound_s = StyleProperties::boundary(),
        const StyleProperties& out_s = StyleProperties::outside(),
        const StyleProperties& in_s = StyleProperties::inside());

      template<typename P>
      void draw_subpaving(const Subpaving<P>& p, const StyleProperties& s = StyleProperties())
      {
        for(const auto& pi : p.boxes())
          draw_box(pi, s);
      }

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

      static std::shared_ptr<Figure2D> selected_fig()
      {
        if(_selected_fig == nullptr)
          _selected_fig = _default_fig;
        return _selected_fig;
      }

      static void set(std::shared_ptr<Figure2D> fig)
      {
        _selected_fig = fig;
      }
      
      static void set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
      {
        auto_init();
        selected_fig()->set_axes(axis1,axis2);
      }
      
      static void set_window_properties(const Vector& pos, const Vector& size)
      {
        auto_init();
        selected_fig()->set_window_properties(pos,size);
      }

      // Geometric shapes

      static void draw_point(const Vector& c, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_point(c,s);
      }

      static void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_box(x,s);
      }

      static void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_circle(c,r,s);
      }

      static void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_ring(c,r,s);
      }

      static void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_line(p1,p2,s);
      }

      static void draw_arrow(const Vector& p1, const Vector& p2, float tip_length, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_arrow(p1,p2,tip_length,s);
      }

      static void draw_polyline(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_polyline(x,s);
      }

      static void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_polyline(x,tip_length,s);
      }

      static void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_polygone(x,s);
      }

      static void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_pie(c,r,theta,s);
      }

      static void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_ellipse(c,ab,theta,s);
      }

      static void draw_ellipsoid(const Ellipsoid& e, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_ellipsoid(e,s);
      }

      static void draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_trajectory(x,s);
      }

      static void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_trajectory(x,s);
      }

      static void draw_trajectory(const SampledTraj<Vector>& x, const ColorMap& cmap)
      {
        auto_init();
        selected_fig()->draw_trajectory(x,cmap);
      }

      static void draw_trajectory(const AnalyticTraj<VectorType>& x, const ColorMap& cmap)
      {
        auto_init();
        selected_fig()->draw_trajectory(x,cmap);
      }

      // Robots

      static void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_tank(x,size,s);
      }

      static void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_AUV(x,size,s);
      }

      // Pavings

      static void draw_paving(const PavingOut& p,
        const StyleProperties& boundary_style = StyleProperties::boundary(),
        const StyleProperties& outside_style = StyleProperties::outside())
      {
        if(auto_init())
        {
          double rx = p.tree()->hull()[0].diam(), ry = p.tree()->hull()[1].diam();
          _default_fig->set_window_properties({20.,20.}, 
            rx > ry ? Vector({800.,800.*ry/rx}) : Vector({800.*rx/ry,800.}));
          _default_fig->set_axes(axis(0,p.tree()->hull()[0]),axis(1,p.tree()->hull()[1]));
        }

        selected_fig()->draw_paving(p, boundary_style, outside_style);
      }

      static void draw_paving(const PavingInOut& p,
        const StyleProperties& boundary_style = StyleProperties::boundary(),
        const StyleProperties& outside_style = StyleProperties::outside(),
        const StyleProperties& inside_style = StyleProperties::inside())
      {
        if(auto_init())
        {
          double rx = p.tree()->hull()[0].diam(), ry = p.tree()->hull()[1].diam();
          _default_fig->set_window_properties({20.,20.}, 
            rx > ry ? Vector({800.,800.*ry/rx}) : Vector({800.*rx/ry,800.}));
          _default_fig->set_axes(axis(0,p.tree()->hull()[0]),axis(1,p.tree()->hull()[1]));
        }

        selected_fig()->draw_paving(p, boundary_style, outside_style, inside_style);
      }

      static void draw_subpaving(const PavingInOut& p,
        const StyleProperties& boundary_style = StyleProperties::boundary(),
        const StyleProperties& outside_style = StyleProperties::outside(),
        const StyleProperties& inside_style = StyleProperties::inside())
      {
        auto_init();
        selected_fig()->draw_paving(p, boundary_style, outside_style, inside_style);
      }

      template<typename P>
      static void draw_subpaving(const Subpaving<P>& p, const StyleProperties& s = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_subpaving(p, s);
      }


    protected:

      static bool auto_init()
      {
        if(!_default_fig && !_selected_fig)
        {
          _default_fig = std::make_shared<Figure2D>(DEFAULT_FIG_NAME, GraphicOutput::VIBES);
          _default_fig->set_window_properties({20.,20.}, {800.,800.});
          _default_fig->set_axes(axis(0,{-10,10}),axis(1,{-10,10}));
          _selected_fig = _default_fig;
          return true;
        }

        return false;
      }

      friend Figure2D;

      static std::shared_ptr<Figure2D> _default_fig;
      static std::shared_ptr<Figure2D> _selected_fig;
  };
}