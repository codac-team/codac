.. _sec-graphics-3d:

The 3D Figure class
===================

  Main author: `Maël Godard <https://godardma.github.io>`_

This page describes the class used in Codac for 3D visualization.

Figure3D
--------

The basic class for 3D visualization is Figure3D. It is used to generate a ``.obj`` file that can be imported in any 3D visualization software.

For online visualization, the free website `3dviewer <https://3dviewer.net/>`_ can be used.

The constructor takes one arguments: the name of the object file.

.. tabs::

  .. code-tab:: py

    fig = Figure3D("my_object") # for the object file my_object.obj

  .. code-tab:: c++

    Figure3D fig ("my_object"); // for the object file my_object.obj

Drawing functions
-----------------

Below are the detailled available drawing functions. The shapes that can be drawn are:

Geometric shapes
  - Box
  - Parallelepiped

Paving
  - PavingOut (Paving with contractors)
  - PavingInOut (Paving with separators)
  - Subpaving

In addition, a function ``draw_axes`` is available to draw the three axes of the 3D space.

.. doxygenfunction:: codac2::Figure3D::draw_axes(double)
  :project: codac

Note that only the stroke color is used in all of the supported drawing functions.

Geometric shapes
----------------

.. doxygenfunction:: codac2::Figure3D::draw_box(const IntervalVector&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure3D::draw_parallelepiped(const Vector&, const Matrix&, const StyleProperties&)
  :project: codac

Paving
------

.. doxygenfunction:: codac2::Figure3D::draw_paving(const PavingOut& p, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure3D::draw_paving(const PavingInOut& p, const StyleProperties&, const StyleProperties&)
  :project: codac

.. doxygenfunction:: codac2::Figure3D::draw_subpaving(const Subpaving<P>&, const StyleProperties&)
  :project: codac
