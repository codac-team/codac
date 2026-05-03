.. _sec-geom:

Geometry
========

Codac provides a set of utility functions for basic geometric calculations.

Several basic geometric types are available under the form of classes representing enclosures of these types. For instance, a 2d point if represented by an ``IntervalVector`` enclosing it, a segment between two 2d points is implemented by the ``Segment`` class, the endpoints of which are ``IntervalVector`` objects. Furthermore, ``Polygon`` and ``ConvexPolygon`` are also available. Operations between these structure will reliably meet the uncertainties associated with the enclosure of points (vertices), as well as floating-point calculations.

Because computations are based on interval arithmetic, all these functions provide reliable results (unless the function name indicates otherwise). Therefore, the outputs of these functions are also enclosures of geometric types. For tests functions, such as ``.contains()``, ``.intersects()``, ``.is_subset()``, *etc.*, outputs will be interval enumerations such as ``BoolInterval`` or ``OrientationInterval`` types, thus allowing inconclusive returns in case of too much uncertainty.

.. toctree::
  :maxdepth: 1

  utils.rst
  segment.rst
  polygon.rst
  zonotope.rst
  polytope.rst

Related interval enumerations
-----------------------------

We recall the ``BoolInterval`` enumeration:

.. doxygenenum:: codac2::BoolInterval
  :project: codac

In addition, the ``OrientationInterval`` enumeration is dedicated to the geometrical functions.

.. doxygenenum:: codac2::OrientationInterval
  :project: codac
