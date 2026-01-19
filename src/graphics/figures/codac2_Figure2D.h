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
#include "codac2_Parallelepiped.h"
#include "codac2_Zonotope.h"
#include "codac2_PavingStyle.h"
#include "codac2_Ellipsoid.h"
#include "codac2_Polygon.h"
#include "codac2_SlicedTube.h"
#include "codac2_Ctc.h"
#include "codac2_Sep.h"

#define DEFAULT_FIG_NAME "Codac - default figure"

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

  class DefaultFigure;
  class PavingOut;
  class PavingInOut;
  template<typename P>
  class Subpaving;

  /**
   * \class Figure2D
   * \brief Figure2D class, used for 2D display
   * 
   * This class is used to display 2D figures.
   * 
   * Currently, it can interact with VIBes and IPE.
   * 
   * For VIBes, the server must be launched before using this class.
   * 
   * For IPE, an xml file is generated and can be opened with the IPE editor.
   */
  class Figure2D : public std::enable_shared_from_this<Figure2D>
  {
    public:

      /**
       * \brief Creates a new Figure2D object, with a given name and output
       * 
       * \param name Name of the figure
       * \param o Output of the figure, can be VIBes or IPE (or both)
       * \param set_as_default (optionnal) If true, the figure is set as the default view, default is false
       */
      Figure2D(const std::string& name, GraphicOutput o, bool set_as_default = false);

      /**
       * \brief Returns ``OutputFigure2D`` objects rendering the current figure.
       * 
       * \return vector of pointers to the ``OutputFigure2D`` objects
       */
      std::vector<std::shared_ptr<OutputFigure2D>> output_figures();

      /**
       * \brief Getter for the name of the figure
       * 
       * \return The name of the figure
       */
      const std::string& name() const;
      
      /**
       * \brief Getter for the size of the figure
       * 
       * \return The size of the figure
       */
      Index size() const;

      /**
       * \brief Getter for the axes of the figure
       * 
       * \return A vector of the axes of the figure
       */
      const std::vector<FigureAxis>& axes() const;

      /**
       * \brief Setter for the axes of the figure
       * 
       * \param axis1 First axis (horizontal)
       * \param axis2 Second axis (vertical)
       */
      Figure2D& set_axes(const FigureAxis& axis1, const FigureAxis& axis2);

      /**
       * \brief Getter for the index of the horizontal axis
       * 
       * \return The index of the horizontal axis
       */
      const Index& i() const;

      /**
       * \brief Getter for the index of the vertical axis
       * 
       * \return The index of the vertical axis
       */
      const Index& j() const;

      /**
       * \brief Getter for the position of the figure
       * 
       * \return The position of the figure
       */
      const Vector& pos() const;

      /**
       * \brief getter for the size of the window
       * 
       * \return The size of the window
       */
      const Vector& window_size() const;

      /**
       * \brief Setter for the position and size of the window
       * 
       * \param pos Position of the window
       * \param size Size of the window
       */
      void set_window_properties(const Vector& pos, const Vector& size);

      /**
       * \brief VIBes only: center the viewbox on a given point with a given radius
       * 
       * \param c Center of the viewbox
       * \param r Radius of the viewbox
       */
      void center_viewbox(const Vector& c, const Vector& r);

      /**
       * \brief Clears the figure
       */
      void clear();

      /**
       * \brief Getter for the scaling factor of the figure
       * 
       * \return The scaling factor of the figure
       */
      double scaled_unit() const;

      /**
       * \brief VIBes only: auto scale the figure
       */
      void auto_scale();

      /**
       * \return True if the figure is the default view
       * 
       * \return True if the figure is the default view, false otherwise
       */
      bool is_default() const;

      /**
       * \brief Sets the figure as the default view
       */
      void set_as_default();

      /**
       * \brief Setter for the time domain of the figure
       * 
       * \param tdomain The new time domain
       */
      void set_tdomain(const Interval& tdomain);

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
       * \brief Draws a line on the figure
       * 
       * \param p1 First point of the line
       * \param p2 Second point of the line
       * \param style Style of the line (edge color)
       */
      void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a line on the figure
       * 
       * \param e segment to be drawn
       * \param style Style of the line (edge color)
       */
      void draw_line(const Segment& e, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws an arrow on the figure
       * 
       * \param p1 First point of the arrow
       * \param p2 Second point of the arrow
       * \param tip_length Length of the tip of the arrow
       * \param style Style of the arrow (edge color and fill color)
       */
      void draw_arrow(const Vector& p1, const Vector& p2, float tip_length, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a polyline on the figure
       * 
       * \param x Vector of the points of the polyline
       * \param style Style of the polyline (edge color)
       */
      void draw_polyline(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a polyline on the figure
       * 
       * \param x Vector of the points of the polyline
       * \param tip_length Length of the tip of the arrow
       * \param style Style of the polyline (edge color and fill color)
       */
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a ``Polygon`` object on the figure
       * 
       * \param x the ``Polygon`` object to be displayed
       * \param style Style of the polygone (edge color and fill color)
       */
      void draw_polygon(const Polygon& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a parallelepiped z+A*[-1,1]^2 on the figure
       * 
       * \param p Parallelepiped to draw (center and shape matrix)
       * \param style Style of the parallelepiped (edge color and fill color)
       */
      void draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a zonotope z+sum_i [-1,1] A_i on the figure
       * 
       * \param z Zonotope to draw (center and shape matrix)
       * \param style Style of the zonotope (edge color and fill color)
       */
      void draw_zonotope(const Zonotope& z, const StyleProperties& style = StyleProperties());

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

      /**
       * \brief Draws an ellipsoid on the figure
       * 
       * \param e Ellipsoid to draw
       * \param style Style of the ellipsoid (edge color and fill color)
       */
      void draw_ellipsoid(const Ellipsoid& e, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a trajectory on the figure
       * 
       * \param x SampledTraj to draw
       * \param style Style of the trajectory (edge color)
       */
      void draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a trajectory on the figure
       * 
       * \param x AnalyticTraj to draw
       * \param style Style of the trajectory (edge color)
       */
      void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a trajectory on the figure with a colormap
       * 
       * \param x SampledTraj to plot
       * \param style StyleGradientProperties to use
       */
      void draw_trajectory(const SampledTraj<Vector>& x, const StyleGradientProperties& style);

      /**
       * \brief Draws a trajectory on the figure with a colormap
       * 
       * \param x AnalyticTraj to plot
       * \param style StyleGradientProperties to use
       */
      void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleGradientProperties& style);

      /**
       * \brief Plots a trajectory on the figure (x-axis is the time)
       * 
       * \param x SampledTraj to plot
       * \param style Style of the trajectory (edge color)
       */
      void plot_trajectory(const SampledTraj<double>& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Plots a set of trajectories on the figure (x-axis is the time) with random colors
       * 
       * \param x SampledTraj<Vector> set of trajectories to plot
       */
      void plot_trajectories(const SampledTraj<Vector>& x);

      /**
       * \brief Plots a set of trajectories on the figure (x-axis is the time) with custom common color
       * 
       * \param x SampledTraj<Vector> set of trajectories to plot
       * \param style Style shared by all the trajectories (edge color)
       */
      void plot_trajectories(const SampledTraj<Vector>& x, const StyleProperties& style);

      /**
       * \brief Draws a tube of ``IntervalVector`` on the figure with some ``StyleProperties``.
       *
       * This function renders the slices (boxes) that compose a ``SlicedTube``.  
       * If the number of slices exceeds ``max_nb_slices_to_display``, consecutive 
       * rectangular slices are merged by groups into convex polygons.  
       * This reduces visual clutter and improves rendering efficiency while 
       * preserving the overall shape of the tube.
       *
       * \param x SlicedTube to draw.
       * \param style Style properties used for drawing the tube.
       * \param max_nb_slices_to_display Maximum number of slices to display before merging them into convex polygons.
       */
      void draw_tube(const SlicedTube<IntervalVector>& x, const StyleProperties& style, int max_nb_slices_to_display = 5000);

      /**
       * \brief Draws a tube of ``IntervalVector`` on the figure with a colormap.
       *
       * This function renders the slices (boxes) that compose a ``SlicedTube``.  
       * If the number of slices exceeds ``max_nb_slices_to_display``, consecutive 
       * rectangular slices are merged by groups into convex polygons.  
       * This reduces visual clutter and improves rendering efficiency while 
       * preserving the overall shape of the tube.
       *
       * \param x SlicedTube to draw.
       * \param style StyleGradientProperties to use
       * \param max_nb_slices_to_display Maximum number of slices to display before merging them into convex polygons.
       */
      void draw_tube(const SlicedTube<IntervalVector>& x, const StyleGradientProperties& style = StyleGradientProperties(ColorMap::blue_tube(), "z:-1"), int max_nb_slices_to_display = 5000);

      /**
       * \brief Plots a tube on the figure (x-axis is the time)
       * 
       * \param x SlicedTube to plot
       * \param style Style of the tube (edge color)
       */
      void plot_tube(const SlicedTube<Interval>& x, const StyleProperties& style = StyleProperties());

      /**
       * \brief Plots a tube on the figure (x-axis is the time), with derivative information: 
       *        slices are displayed as polygons.
       * 
       * \param x SlicedTube to plot
       * \param v derivative tube of the SlicedTube to plot
       * \param style Style of the tube (edge color)
       */
      void plot_tube(const SlicedTube<Interval>& x, const SlicedTube<Interval>& v, const StyleProperties& style = StyleProperties());

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

      // Miscellaneous

      /**
       * \brief Draws text on the figure
       * 
       * \param text Text to display
       * \param ul Position of the top-left corner of the text
       * \param scale Scaling of the text (VIBes only)
       * \param style Style of the text (color, layer)
       */
      void draw_text(const std::string& text, const Vector& ul, double scale, const StyleProperties& style = StyleProperties());

      /**
       * \brief Draws a raster on the figure in VIBes only, only the bounding box is drawn in IPE
       * 
       * \param filename The name of the file, the path is relative to the VIBes' server folder
       * \param bbox The bounding box of the raster
       * \param style Style of the raster (only the layer is used)
       */
      void draw_raster(const std::string& filename, const IntervalVector& bbox, const StyleProperties& style = StyleProperties());

      // Pavings

      /**
       * \brief Draws a previously computed paving (outer approximation) on the figure
       * 
       * \param p PavingOut to draw (result of a paving with contractors)
       * \param style ``PavingStyle`` for the drawing
       */
      void draw_paving(const PavingOut& p,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a previously computed paving (outer approximation) on the figure
       * 
       * \param p PavingOut to draw (result of a paving with contractors)
       * \param draw_box Custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */
      void draw_paving(const PavingOut& p,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a previously computed paving (inner/outer approximation) on the figure
       * 
       * \param p PavingInOut to draw (result of a paving with separators)
       * \param style ``PavingStyle`` for the drawing
       */
      void draw_paving(const PavingInOut& p,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a previously computed paving (inner/outer approximation) on the figure
       * 
       * \param p PavingInOut to draw (result of a paving with separators)
       * \param draw_box Custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */
      void draw_paving(const PavingInOut& p,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a paving from a contractor while it is being computed
       * 
       * \param x0 Initial box
       * \param c Contractor to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      void pave(const IntervalVector& x0, const C& c, double eps,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a paving from a contractor while it is being computed
       * 
       * \param x0 Initial box
       * \param c Contractor to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param draw_box Optional custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      void pave(const IntervalVector& x0, const C& c, double eps,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a paving from a separator while it is being computed
       * 
       * \param x0 Initial box
       * \param s Separator to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename S>
        requires IsSepBaseOrPtr<S>
      void pave(const IntervalVector& x0, const S& s, double eps,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a paving from a separator while it is being computed
       * 
       * \param x0 Initial box
       * \param s Separator to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param draw_box Optional custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename S>
        requires IsSepBaseOrPtr<S>
      void pave(const IntervalVector& x0, const S& s, double eps,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style());

      /**
       * \brief Draws a subpaving on the figure
       * 
       * \param p Subpaving to draw
       * \param style Style of the subpaving
       */
      template<typename P>
      void draw_subpaving(const Subpaving<P>& p, const StyleProperties& style = StyleProperties())
      {
        for(const auto& pi : p.boxes())
          draw_box(pi, style);
      }

    protected:

      const std::string _name;
      Vector _pos {50,50}, _window_size {700,700};
      std::vector<FigureAxis> _axes { axis(0,{0,1}), axis(1,{0,1}) };
      std::vector<std::shared_ptr<OutputFigure2D>> _output_figures;
      Interval _tdomain;

      friend DefaultFigure;
  };

  /**
   * \class DefaultFigure
   * \brief Default view class, used to manage the default figure
   * 
   * This class is used to manage the default figure. By default it only interacts with VIBes.
   * 
   * Any figure can be set as the default view.
   */
  class DefaultFigure
  {
    public:

      /**
       * \brief Getter for the default figure
       * 
       * \return The default figure
       */
      static std::shared_ptr<Figure2D> selected_fig()
      {
        if(_selected_fig == nullptr)
          _selected_fig = _default_fig;
        return _selected_fig;
      }

      /**
       * \brief Setter for the default figure
       * 
       * \param fig The new default figure
       */
      static void set(std::shared_ptr<Figure2D> fig)
      {
        _selected_fig = fig;
      }
      
      /**
       * \brief Setter for the axes of the figure
       * 
       * \param axis1 First axis (horizontal)
       * \param axis2 Second axis (vertical)
       */
      static Figure2D& set_axes(const FigureAxis& axis1, const FigureAxis& axis2)
      {
        auto_init();
        return selected_fig()->set_axes(axis1,axis2);
      }
      
      /**
       * \brief Setter for the position and size of the window
       * 
       * \param pos Position of the window
       * \param size Size of the window
       */
      static void set_window_properties(const Vector& pos, const Vector& size)
      {
        auto_init();
        selected_fig()->set_window_properties(pos,size);
      }

      /**
       * \brief VIBes only: auto scale the figure
       */
      static void auto_scale()
      {
        auto_init();
        selected_fig()->auto_scale();
      }

      /**
       * \brief Clear the figure
       */
      static void clear()
      {
        auto_init();
        selected_fig()->clear();
      }

      // Geometric shapes

      /**
       * \brief Draws a point on the figure
       * 
       * \param c Coordinates of the point
       * \param style Style of the point (edge color and fill color)
       */
      static void draw_point(const Vector& c, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_point(c,style);
      }

      /**
       * \brief Draws a box on the figure
       * 
       * \param x Box to draw
       * \param style Style of the box (edge color and fill color)
       */
      static void draw_box(const IntervalVector& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_box(x,style);
      }

      /**
       * \brief Draws a circle on the figure
       * 
       * \param c Center of the circle
       * \param r Radius of the circle
       * \param style Style of the circle (edge color and fill color)
       */
      static void draw_circle(const Vector& c, double r, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_circle(c,r,style);
      }

      /**
       * \brief Draws a ring on the figure
       * 
       * \param c Center of the ring
       * \param r Inner and outer radius of the ring
       * \param style Style of the ring (edge color and fill color)
       */
      static void draw_ring(const Vector& c, const Interval& r, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_ring(c,r,style);
      }

      /**
       * \brief Draws a line on the figure
       * 
       * \param p1 First point of the line
       * \param p2 Second point of the line
       * \param style Style of the line (edge color)
       */
      static void draw_line(const Vector& p1, const Vector& p2, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_line(p1,p2,style);
      }

      /**
       * \brief Draws a line on the figure
       * 
       * \param e segment to be drawn
       * \param style Style of the line (edge color)
       */
      static void draw_line(const Segment& e, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_line(e,style);
      }

      /**
       * \brief Draws an arrow on the figure
       * 
       * \param p1 First point of the arrow
       * \param p2 Second point of the arrow
       * \param tip_length Length of the tip of the arrow
       * \param style Style of the arrow (edge color and fill color)
       */
      static void draw_arrow(const Vector& p1, const Vector& p2, float tip_length, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_arrow(p1,p2,tip_length,style);
      }

      /**
       * \brief Draws a polyline on the figure
       * 
       * \param x Vector of the points of the polyline
       * \param style Style of the polyline (edge color)
       */
      static void draw_polyline(const std::vector<Vector>& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_polyline(x,style);
      }

      /**
       * \brief Draws a polyline on the figure
       * 
       * \param x Vector of the points of the polyline
       * \param tip_length Length of the tip of the arrow
       * \param style Style of the polyline (edge color and fill color)
       */
      static void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_polyline(x,tip_length,style);
      }

      /**
       * \brief Draws a ``Polygon`` object on the figure
       * 
       * \param x the ``Polygon`` object to be drawn
       * \param style Style of the polygone (edge color and fill color)
       */
      static void draw_polygon(const Polygon& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_polygon(x,style);
      }

      /**
       * \brief Draws a parallelepiped z+A*[-1,1]^2 on the figure
       * 
       * \param p Parallelepiped to draw (center and shape matrix)
       * \param style Style of the parallelepiped (edge color and fill color)
       */
      static void draw_parallelepiped(const Parallelepiped& p, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_parallelepiped(p,style);
      }

      /**
       * \brief Draws a zonotope z+sum_i [-1,1] A_i on the figure
       * 
       * \param z Zonotope to draw (center and shape matrix)
       * \param style Style of the zonotope (edge color and fill color)
       */
      static void draw_zonotope(const Zonotope& z, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_zonotope(z,style);
      }

      /**
       * \brief Draws a pie on the figure
       * 
       * \param c Center of the pie
       * \param r Inner and outer radius of the pie
       * \param theta Start and end angle of the pie (in radians)
       * \param style Style of the pie (edge color and fill color)
       */
      static void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_pie(c,r,theta,style);
      }

      /**
       * \brief Draws an ellipse on the figure
       * 
       * \param c Center of the ellipse
       * \param ab Half-lengths of the ellipse
       * \param theta Rotation angle of the ellipse (in radians)
       * \param style Style of the ellipse (edge color and fill color)
       */
      static void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_ellipse(c,ab,theta,style);
      }

      /**
       * \brief Draws an ellipsoid on the figure
       * 
       * \param e Ellipsoid to draw
       * \param style Style of the ellipsoid (edge color and fill color)
       */
      static void draw_ellipsoid(const Ellipsoid& e, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_ellipsoid(e,style);
      }

      /**
       * \brief Draws a trajectory on the figure
       * 
       * \param x Trajectory to draw
       * \param style Style of the trajectory (edge color)
       */
      static void draw_trajectory(const SampledTraj<Vector>& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_trajectory(x,style);
      }

      /**
       * \brief Draws a trajectory on the figure
       * 
       * \param x AnalyticTraj to draw
       * \param style Style of the trajectory (edge color)
       */
      static void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_trajectory(x,style);
      }

      /**
       * \brief Draws a trajectory on the figure with a colormap
       * 
       * \param x SampledTraj to draw
       * \param style StyleGradientProperties to use
       */
      static void draw_trajectory(const SampledTraj<Vector>& x, const StyleGradientProperties& style)
      {
        auto_init();
        selected_fig()->draw_trajectory(x,style);
      }

      /**
       * \brief Draws a trajectory on the figure with a colormap
       * 
       * \param x AnalyticTraj to draw
       * \param style StyleGradientProperties to use
       */
      static void draw_trajectory(const AnalyticTraj<VectorType>& x, const StyleGradientProperties& style)
      {
        auto_init();
        selected_fig()->draw_trajectory(x,style);
      }

      /**
       * \brief Plots a trajectory on the figure (x-axis is the time)
       * 
       * \param x SampledTraj to plot
       * \param style Style of the trajectory (edge color)
       */
      static void plot_trajectory(const SampledTraj<double>& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->plot_trajectory(x,style);
      }

      /**
       * \brief Plots a set of trajectories on the figure (x-axis is the time)
       * 
       * \param x SampledTraj<Vector> set of trajectories to plot
       * \param style Style of the trajectory (edge color)
       */
      static void plot_trajectories(const SampledTraj<Vector>& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->plot_trajectories(x,style);
      }

      /**
       * \brief Draws a tube of `IntervalVector` on the figure
       * 
       * \param x SlicedTube to draw
       * \param style Style of the tube
       */
      static void draw_tube(const SlicedTube<IntervalVector>& x, const StyleProperties& style)
      {
        auto_init();
        selected_fig()->draw_tube(x,style);
      }

      /**
       * \brief Draws a tube of `IntervalVector` on the figure with a colormap
       * 
       * \param x SlicedTube to draw
       * \param style StyleGradientProperties to use
       */
      static void draw_tube(const SlicedTube<IntervalVector>& x, const StyleGradientProperties& style = StyleGradientProperties(ColorMap::blue_tube(), "z:-1"))
      {
        auto_init();
        selected_fig()->draw_tube(x,style);
      }

      /**
       * \brief Plots a tube on the figure (x-axis is the time)
       * 
       * \param x SlicedTube to plot
       * \param style Style of the tube (edge color)
       */
      static void plot_tube(const SlicedTube<Interval>& x, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->plot_tube(x,style);
      }

      /**
       * \brief Plots a tube on the figure (x-axis is the time), with derivative information: 
       *        slices are displayed as polygons.
       * 
       * \param x SlicedTube to plot
       * \param v derivative tube of the SlicedTube to plot
       * \param style Style of the tube (edge color)
       */
      static void plot_tube(const SlicedTube<Interval>& x, const SlicedTube<Interval>& v, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->plot_tube(x,v,style);
      }

      // Robots

      /**
       * \brief Draws a tank on the figure
       * 
       * \param x Coordinates of the tank
       * \param size Size of the tank
       * \param style Style of the tank (edge color and fill color)
       */
      static void draw_tank(const Vector& x, float size, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_tank(x,size,style);
      }

      /**
       * \brief Draws an AUV on the figure
       * 
       * \param x Coordinates of the AUV
       * \param size Size of the AUV
       * \param style Style of the AUV (edge color and fill color)
       */
      static void draw_AUV(const Vector& x, float size, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_AUV(x,size,style);
      }

      /**
       * \brief Draws a motor boat on the figure
       * 
       * \param x Coordinates of the motor boat
       * \param size Size of the motor boat
       * \param style Style of the motor boat (edge color and fill color)
       */
      static void draw_motor_boat(const Vector& x, float size, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_motor_boat(x,size,style);
      }

      // Miscellaneous

      /**
       * \brief Draws text on the figure
       * 
       * \param text Text to display
       * \param ul Position of the top-left corner of the text
       * \param scale Scaling of the text (VIBes only)
       * \param style Style of the text (edge color, layer)
       */
      static void draw_text(const std::string& text, const Vector& ul, double scale, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_text(text,ul,scale,style);
      }

      /**
       * \brief Draws a raster on the figure in VIBes only, only the bounding box is drawn in IPE
       * 
       * \param filename The name of the file, the path is relative to the VIBes' server folder
       * \param bbox The bounding box of the raster
       * \param style Style of the raster (only the layer is used)
       */
      static void draw_raster(const std::string& filename, const IntervalVector& bbox, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_raster(filename,bbox,style);
      }

      // Pavings

    protected:

      static void init_axes_paving(const IntervalVector& x)
      {
        _default_fig->set_window_properties({100,100}, {800,800});
        _default_fig->set_axes(axis(0,x[0],"x_1"), axis(1,x[1],"x_2"));
        _default_fig->auto_scale();
      }

    public:

      /**
       * \brief Draws a previously computed paving (outer approximation) on the figure
       * 
       * \param p PavingOut to draw (result of a paving with contractors)
       * \param style ``PavingStyle`` for the drawing
       */
      static void draw_paving(const PavingOut& p,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(p.tree()->hull());
        selected_fig()->draw_paving(p, style);
      }

      /**
       * \brief Draws a previously computed paving (outer approximation) on the figure
       * 
       * \param p PavingOut to draw (result of a paving with contractors)
       * \param draw_box Custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */
      static void draw_paving(const PavingOut& p,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(p.tree()->hull());
        selected_fig()->draw_paving(p, draw_box, style);
      }

      /**
       * \brief Draws a previously computed paving (inner/outer approximation) on the figure
       * 
       * \param p PavingInOut to draw (result of a paving with separators)
       * \param style ``PavingStyle`` for the drawing
       */
      static void draw_paving(const PavingInOut& p,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(p.tree()->hull());
        selected_fig()->draw_paving(p, style);
      }

      /**
       * \brief Draws a previously computed paving (inner/outer approximation) on the figure
       * 
       * \param p PavingInOut to draw (result of a paving with separators)
       * \param draw_box Custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */
      static void draw_paving(const PavingInOut& p,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(p.tree()->hull());
        selected_fig()->draw_paving(p, draw_box, style);
      }

      /**
       * \brief Draws a paving from a contractor while it is being computed
       * 
       * \param x0 Initial box
       * \param c Contractor to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param style ``PavingStyle`` for the drawing
       */
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      static void pave(const IntervalVector& x0, const C& c, double eps,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(x0);
        selected_fig()->pave(x0, c, eps, style);
      }

      /**
       * \brief Draws a paving from a contractor while it is being computed
       * 
       * \param x0 Initial box
       * \param c Contractor to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param draw_box Optional custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename C>
        requires IsCtcBaseOrPtr<C,IntervalVector>
      static void pave(const IntervalVector& x0, const C& c, double eps,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(x0);
        selected_fig()->pave(x0, c, eps, draw_box, style);
      }

      /**
       * \brief Draws a paving from a separator while it is being computed
       * 
       * \param x0 Initial box
       * \param s Separator to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename S>
        requires IsSepBaseOrPtr<S>
      static void pave(const IntervalVector& x0, const S& s, double eps,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(x0);
        selected_fig()->pave(x0, s, eps, style);
      }

      /**
       * \brief Draws a paving from a separator while it is being computed
       * 
       * \param x0 Initial box
       * \param s Separator to be paved
       * \param eps Accuracy of the paving algorithm (the undefined boxes will have their max_diam <= eps)
       * \param draw_box Optional custom drawing function (for instance, if one wants to draw in polar coordinates)
       * \param style ``PavingStyle`` for the drawing
       */  
      template<typename S>
        requires IsSepBaseOrPtr<S>
      static void pave(const IntervalVector& x0, const S& s, double eps,
        const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>& draw_box,
        const PavingStyle& style = PavingStyle::default_style())
      {
        if(auto_init())
          init_axes_paving(x0);
        selected_fig()->pave(x0, s, eps, draw_box, style);
      }

      /**
       * \brief Draws a subpaving on the figure
       * 
       * \param p Subpaving to draw
       * \param style Style of the subpaving
       */
      template<typename P>
      static void draw_subpaving(const Subpaving<P>& p, const StyleProperties& style = StyleProperties())
      {
        auto_init();
        selected_fig()->draw_subpaving(p, style);
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

#include "codac2_Figure2D_pave.h"