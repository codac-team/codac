.. _sec-polytope:

Polytope
========

  Main author: `Damien Massé <https://labsticc.fr/fr/annuaire/masse-damien>`_

The Polytope class represent convex polytopes (or, more generally, 
convex polyhedra).
Polytopes are internally represented as intersections of linear constraints
(and a bounding box), in a class name CollectFacets.
A double-description algorithm is used to compute generators for operations
requiring them (union, projections...). Due to the imprecise nature of
floating-point computations, the algorithm is designed such that the
convex hull of the "generators" encloses the polytope. However, no guarantee
is given that each vertice of the polytope is associated to a generator.
As a result,
going back to the hyperplane representation may return a larger polytope.

.. toctree::
  :maxdepth: 1

  facets.rst
  polytope_class.rst
  

Building a polytope
-------------------
Polytopes can be defined from a set of facets (linear inequalities 
or equalities), or a set of vertices. 

.. tabs::

  .. group-tab:: C++

    .. literalinclude:: src_polytope.cpp
      :language: c++
      :start-after: [polytope-1-beg]
      :end-before: [polytope-1-end]
      :dedent: 2

Output of a polytope
--------------------
Polytopes are displayed as a set of linear constraints and a bounding box.
To display the set of vertices, one can use the 
vertices() method.

.. tabs::

  .. group-tab:: C++

    .. literalinclude:: src_polytope.cpp
      :language: c++
      :start-after: [polytope-2-beg]
      :end-before: [polytope-2-end]
      :dedent: 2

Operations on polytopes
-----------------------

