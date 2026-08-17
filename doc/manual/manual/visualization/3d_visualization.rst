.. _sec-graphics-3d:

The 3D Figure class
===================

  Main author: `Maël Godard <https://godardma.github.io>`_, `Damien Massé <https://labsticc.fr/fr/annuaire/masse-damien>`_

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

  .. code-tab:: matlab

    fig = Figure3D("my_object") % for the object file my_object.obj

Drawing functions
-----------------

Below are the detailled available drawing functions. The shapes that can be drawn are:

Geometric shapes
  - Box
  - Sphere
  - Arrow
  - Zonotope
  - Parallelepiped
  - Parallelogram
  - Triangle
  - Star-shaped polygon
  - Parametric surface

Vehicles
  - Car
  - Plane

Paving
  - PavingOut (Paving with contractors)
  - PavingInOut (Paving with separators)
  - Subpaving



Note that only the stroke color is used in all of the supported drawing functions.

In addition, a function ``draw_axes`` is available to draw the three axes of the 3D space. It can take two arguments :

- float : the size of the axes
- Vector : the origin of the axes


Geometric shapes
----------------

All the drawable geometric objects can take a last optionnal argument to set up their stroke color. 
For further details, refer to :ref:`subsec-graphics-colors-style-properties`. 

The geometric shapes that can be drawn and their arguments are listed below :

- draw_box

  - IntervalVector : the box to draw

- draw_sphere
  
  - Vector : the center of the sphere
  - Matrix : the scaling matrix

- draw_arrow

  - Vector : start of the arrow
  - Matrix : orientation of the arrow (first column)

- draw_zonotope

  - Zonotope: the zonotope to draw

- draw_parallelepiped

  - Parallelepiped : the parallelepiped to draw

- draw_parallelogram (defined by :math:`c + A (p + [-1,1]*v1 + [-1,1]*v2)`)

  - Vector : c, the translation
  - Matrix : A, the scaling
  - Vector : p, base point
  - Vector : v1, first generator
  - Vector : v2, second generator

- draw_triangle

  - Vector : first point
  - Vector : second point
  - Vector : third point

- draw_triangle

  - Vector : translation applied to the triangle
  - Matrix : scaling matrix applied to the triangle
  - Vector : first point
  - Vector : second point
  - Vector : third point

The ``draw_polygon`` can be used to draw a `star-shaped polygon <https://en.wikipedia.org/wiki/Star-shaped_polygon>`_ when the vectors are coplanar, and more
generally a sequence of adjacent triangles sharing a same vertex.

- draw_polygon

  - Vector : translation applied to the polygon
  - Matrix : scaling matrix applied to the polygon
  - vector<Vector> : vector where each element is a point of the polygon to draw

- draw_surface

  - Vector : the translation applied to the surface
  - Matrix : scaling applied to the surface
  - Interval : bounds of p1
  - double : incrementation for p1
  - Interval : bounds of p2
  - double : incrementation for p2
  - function<double, double> -\> Vector : the function of the surface, linking each (p1,p2) to a 3D point

Vehicles
--------

All the drawable vehicles can take a last optionnal argument to set up their stroke color. 
For further details, refer to :ref:`subsec-graphics-colors-style-properties`. 

The vehicles that can be drawn and their arguments are listed below :

- draw_car

  - Vector : center of the car
  - Matrix : orientation of the car

- draw_plane

  - Vector : center of the plane
  - Matrix : orientation of the plane
  - bool : (optionnal) defines if the yaw axis is up, default to true

Paving
------

When a paving is drawn, only the inside and boundary boxes are drawn. This is done to avoid outside boxes masking them.

If only one type of paving is drawn (for example a paving with contractors), only one :ref:`subsec-graphics-colors-style-properties` can be defined to choose its edge color.
If two types are drawn (boundary and inside), two :ref:`subsec-graphics-colors-style-properties` can be passed to select both colors.

The paving that can be drawn and their arguments are listed below :

- draw_paving

  - PavingOut | PavingInOut : the paving to draw

- draw_subpaving

  - Subpaving : the subpaving to draw