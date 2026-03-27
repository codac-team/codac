.. _sec-intervals-intervalvector-class:

The IntervalVector class
========================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

An :class:`~codac.IntervalVector` represents an axis-aligned *box* (Cartesian product of intervals) in :math:`\mathbb{R}^n`:

.. math::

   [\mathbf{x}] = [x_1]\times [x_2]\times \dots \times [x_n].

In C++, ``IntervalVector`` is an alias for an Eigen dynamic column vector whose scalar type is :ref:`Interval <sec-intervals-class>` (i.e., ``Eigen::Matrix<Interval,-1,1>``). This means you can use the usual Eigen vector API (size/resize/segment/concatenation patterns) while benefiting from Codac’s interval arithmetic and utilities. In Python and Matlab, some of these functions are available.

Creating IntervalVectors
------------------------

A box can be created from:

* a dimension :math:`n` (components initialized with the default interval :math:`[-\infty,\infty]`),
* a dimension :math:`n` and a common interval (a “cube”),
* a list of intervals or bounds pairs,
* a real vector (degenerate box wrapping a point).

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [intervalvector-class-1-beg]
      :end-before: [intervalvector-class-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [intervalvector-class-1-beg]
      :end-before: [intervalvector-class-1-end]
      :dedent: 4

.. note::

   Codac intentionally does **not** enable Eigen’s “initialize matrices by zero” option for interval-based vectors and matrices, because default-initializing an interval should produce :math:`[-\infty,\infty]` rather than ``0``.
   As a result, resizing an ``IntervalVector`` typically creates new components initialized as :math:`[-\infty,\infty]`.

Accessing and modifying components
----------------------------------

Components are intervals; indexing is 0-based in Python/C++ and 1-based in Matlab.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [intervalvector-class-2-beg]
      :end-before: [intervalvector-class-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [intervalvector-class-2-beg]
      :end-before: [intervalvector-class-2-end]
      :dedent: 4


Box properties
--------------

Most scalar properties of :ref:`Interval <sec-intervals-class>` have natural extensions to boxes:

* component-wise bounds: :math:`\mathbf{x}^-` and :math:`\mathbf{x}^+`,
* component-wise widths (diameters/radii),
* geometric metrics such as volume or max diameter.

Typical accessors you will use in practice:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [intervalvector-class-3-beg]
      :end-before: [intervalvector-class-3-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [intervalvector-class-3-beg]
      :end-before: [intervalvector-class-3-end]
      :dedent: 4

.. note::

   The exact set of box-wide metrics available (*e.g.*, ``max_diam()``, ``volume()``, ``argmax_diam()``) is provided by the interval-based Eigen extensions used in Codac. See the technical documentation section at the end of this page for the comprehensive list.

Testing boxes
-------------

Common predicates include:

- ``is_empty()``: tests whether the box represents the empty set (at least one empty component),
- ``is_degenerated()``: all components are degenerate,
- ``is_degenerated()``: at least one component is degenerate,
- ``is_unbounded()``: at least one bound is infinite,
- ``contains(p)``: inclusion test of a point/vector,
- ``intersects(y)``: non-empty intersection test with another box,
- ``is_subset(y)``, ``is_strict_subset(y)``, *etc.*

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [intervalvector-class-4-beg]
      :end-before: [intervalvector-class-4-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [intervalvector-class-4-beg]
      :end-before: [intervalvector-class-4-end]
      :dedent: 4

Other operations
----------------

As for intervals, typical operations on boxes include:

.. list-table:: Common advanced methods for a given box :math:`[\mathbf{x}]`
   :widths: 30 70
   :header-rows: 1

   * - Method
     - Description
   * - ``inflate(rad)``
     - Expands the box by ``±rad`` (scalar radius for common inflation, or vector radius for specifying different inflation on each component).
   * - ``bisect_largest([ratio])``
     - Splits the box into two sub-boxes along the widest dimension.
   * - ``bisect(i,[ratio])``
     - Splits the box into two sub-boxes along the :math:`i`-th dimension.
   * - ``rand()``
     - Returns a random sample ``Vector`` inside the box (when non-empty).
   * - ``init()``
     - Re-initializes to :math:`[-\infty,\infty]^n`.

Arithmetic and linear algebra
-----------------------------

Because ``IntervalVector`` is a vector of ``Interval``, standard arithmetic is naturally extended component-wise:

* box–box and box–scalar operations (``+``, ``-``, ``*``, ``/``),
* interactions with real vectors/matrices,
* matrix–vector products involving :class:`~codac.IntervalMatrix`.

  .. tabs::

    .. group-tab:: Python

      .. literalinclude:: src.py
        :language: py
        :start-after: [intervalvector-class-5-beg]
        :end-before: [intervalvector-class-5-end]
        :dedent: 4

    .. group-tab:: C++

      .. literalinclude:: src.cpp
        :language: c++
        :start-after: [intervalvector-class-5-beg]
        :end-before: [intervalvector-class-5-end]
        :dedent: 4


.. admonition:: Technical documentation

  See the `C++ API documentation of the IntervalVector class <../../api/html/codac2___interval_vector_8h.html>`_.