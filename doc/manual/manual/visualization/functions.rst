.. _sec-graphics-functions:

2D drawing functions
====================

  Main author: `Maël Godard <https://godardma.github.io>`_

This page lists all the 2D drawing functions available in Codac. Note that the functions described here on a
:ref:`Figure2D <subsec-graphics-2d-figures-figure2d>` object also work on the :ref:`DefaultFigure <subsec-graphics-2d-figures-defaultfigure>`.

For example : 

.. tabs::

  .. code-tab:: py
    
    # On a figure
    fig.draw_box([[2.2,2.5],[2.2,2.5]])

    # On the DefaultFigure
    DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]])

  .. code-tab:: c++

    // On a figure
    fig.draw_box({{2.2,2.5},{2.2,2.5}});

    // On the DefaultFigure
    DefaultFigure::draw_box({{2.2,2.5},{2.2,2.5}});

In addition to the arguments described here for every function, an optionnal argument of type StyleProperties can be added to choose the edge
and fill color (by default black edge no fill). This object can be deduced from one or two Color objects, see :ref:`this page <sec-graphics-colors>` 
for more information.

.. tabs::

  .. code-tab:: py
    
    fig.draw_box([[2.2,2.5],[2.2,2.5]],Color.red()) # Red edge
    fig.draw_box([[2.2,2.5],[2.2,2.5]],[Color.red()]) # Red edge
    fig.draw_box([[2.2,2.5],[2.2,2.5]],[Color.red(),Color.blue()]) # Red edge and blue fill

  .. code-tab:: c++

    fig.draw_box({{2.2,2.5},{2.2,2.5}},Color.red()); // Red edge
    fig.draw_box({{2.2,2.5},{2.2,2.5}},{Color.red()}); // Red edge
    fig.draw_box({{2.2,2.5},{2.2,2.5}},{Color.red(),Color.blue()}); // Red edge and blue fill

A line style and a layer can be added to the StyleProperties object, see :ref:`the dedicated page <sec-graphics-colors>` for more information.


Overview of the functions
-------------------------

Below are the detailled available drawing functions. The shapes that can be drawn are:

Geometric shapes
  - Point
  - Box
  - Circle
  - Ring
  - Line
  - Arrow
  - Polyline
  - Polygone
  - Parallelepiped
  - Zonotope
  - Pie
  - Ellipse
  - Ellipsoid

Trajectories
  - Sampled Trajectory
  - Analytic Trajectory

Vehicles
  - Tank
  - AUV
  - Motor Boat

Paving
  - PavingOut (Paving with contractors)
  - PavingInOut (Paving with separators)
  - Subpaving

Geometric shapes
----------------

.. doxygenfunction:: codac2::Figure2D::draw_point(const Vector&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_box(const IntervalVector&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_circle(const Vector&, double, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_ring(const Vector&, const Interval&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_line(const Vector&, const Vector&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_line(const Segment&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_arrow(const Vector&, const Vector&, float, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_polyline(const std::vector<Vector>&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_polyline(const std::vector<Vector>&, float, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_polygon(const Polygon&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_parallelepiped(const Parallelepiped&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_zonotope(const Zonotope&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_pie(const Vector&, const Interval&, const Interval&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_ellipse(const Vector&, const Vector&, double, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_ellipsoid(const Ellipsoid&, const StyleProperties&)
  :project: codac

Trajectories and tubes
----------------------

.. doxygenfunction:: codac2::Figure2D::draw_trajectory(const SampledTraj<Vector>&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_trajectory(const AnalyticTraj<VectorType>&, const StyleProperties&)
  :project: codac

Trajectories can be drawn with a ColorMap instead of the classic StyleProperties. This can be done by passing a ColorMap object as the second argument.

.. doxygenfunction:: codac2::Figure2D::draw_trajectory(const SampledTraj<Vector>&, const StyleGradientProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_trajectory(const AnalyticTraj<VectorType>&, const StyleGradientProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_tube(const SlicedTube<IntervalVector>&, const StyleProperties&, int)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_tube(const SlicedTube<IntervalVector>&, const StyleGradientProperties&, int)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::plot_tube(const SlicedTube<Interval>&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::plot_tube(const SlicedTube<Interval>&, const SlicedTube<Interval>&, const StyleProperties&)
  :project: codac

Vehicles
--------

.. doxygenfunction:: codac2::Figure2D::draw_tank(const Vector&, float, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_AUV(const Vector&, float, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_motor_boat(const Vector&, float, const StyleProperties&)
  :project: codac

Miscellaneous
-------------

.. doxygenfunction:: codac2::Figure2D::draw_text(const std::string&, const Vector&, double, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_raster(const std::string&, const IntervalVector&, const StyleProperties&)
  :project: codac

Paving
------

.. doxygenfunction:: codac2::Figure2D::draw_paving(const PavingOut&, const PavingStyle&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_paving(const PavingOut&, const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>&, const PavingStyle&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_paving(const PavingInOut&, const PavingStyle&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_paving(const PavingInOut&, const std::function<void(Figure2D&,const IntervalVector&,const StyleProperties&)>&, const PavingStyle&)
  :project: codac

.. doxygenfunction:: codac2::Figure2D::draw_subpaving(const Subpaving<P>&, const StyleProperties&)
  :project: codac
