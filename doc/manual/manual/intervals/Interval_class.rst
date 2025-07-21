.. _sec-intervals-class:

The Interval class
==================

  Main author: `Simon Rohou <https://www.simon-rohou.fr/research/>`_

The ``Interval`` class represents closed and connected subsets of :math:`\mathbb{R}`.
It is based on the ``gaol::interval`` structure and extends it with many utility methods.


Creating Intervals
------------------

An interval :math:`[a,b]` can be created using the constructor ``Interval(a, b)``.
You can also create degenerate intervals, unbounded intervals, or intervals from collections.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [interval-class-1-beg]
      :end-before: [interval-class-1-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [interval-class-1-beg]
      :end-before: [interval-class-1-end]
      :dedent: 4

  .. code-tab:: matlab

    x = Interval(3, 5)                          % [3.0, 5.0]
    x = Interval(4.2)                           % [4.2, 4.2]
    x = Interval()                              % [-∞, ∞]
    x = Interval(-oo, -10)                      % [-∞, -10]
    x = Interval([2.0, -3.0])                   % [-3.0, 2.0]
    x = Interval([3.14])                        % [3.14, 3.14]


.. note::

  Note that in Codac, the value *positive infinity* is represented by the constant ``oo`` (and ``-oo`` for negative infinity). One possible implementation of this constant is ``std::numeric_limits<double>::infinity()`` in C++, or ``math.inf`` in Python. Therefore, the declarations ``Interval()`` and ``Interval(-oo,oo)`` are equivalent.


Predefined constants
--------------------

Some common intervals are predefined:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [interval-class-2-beg]
      :end-before: [interval-class-2-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [interval-class-2-beg]
      :end-before: [interval-class-2-end]
      :dedent: 4

  .. code-tab:: matlab

    x = Interval()                              % [-∞,∞] (default value)
    x = Interval().empty                        % ∅
    x = Interval().pi                           % [π]
    x = Interval().two_pi                       % [2π]
    x = Interval().half_pi                      % [π/2]
    x = Interval().zero                         % [0]
    x = Interval().one                          % [1]

.. note::

  Note that the constant :math:`[\pi]` is a reliable enclosure of the :math:`\pi` value, that cannot be exactly represented in a computer with a single floating-point value.

  .. tabs::
    
    .. group-tab:: Python

      .. literalinclude:: src.py
        :language: py
        :start-after: [interval-class-3-beg]
        :end-before: [interval-class-3-end]
        :dedent: 4

    .. group-tab:: C++

      .. literalinclude:: src.cpp
        :language: c++
        :start-after: [interval-class-3-beg]
        :end-before: [interval-class-3-end]
        :dedent: 4

    .. code-tab:: matlab
    
      x = Interval().pi                           % [π]
      % x = [3.14159, 3.1416]

  In case you need a non-reliable floating point value for :math:`\pi`, you can use the ``PI`` constant already available.


Interval properties
-------------------

You can access key interval properties:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [interval-class-4-beg]
      :end-before: [interval-class-4-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [interval-class-4-beg]
      :end-before: [interval-class-4-end]
      :dedent: 4

  .. code-tab:: matlab

    x.lb        % lower bound
    x.ub        % upper bound
    x.mid       % midpoint
    x.rad       % radius
    x.diam      % diameter
    x.mag       % magnitude
    x.mig       % mignitude
    x.size      % dimension (always 1)


Testing intervals
-----------------

Intervals support a wide range of predicates:

- ``is_empty()``: test if the interval is empty
- ``is_degenerated()``: test if it’s of the form [a,a]
- ``is_integer()``: test if it’s an integer singleton
- ``is_unbounded()``: test if any bound is infinite
- ``contains(x)``: test if it contains a real number
- ``intersects(y)``: test if it intersects with another interval
- ``is_subset(y)``: test if it is a subset of another
- ``is_strict_subset(y)``, ``is_interior_subset(y)``, *etc.*


Interval arithmetic
-------------------

All standard arithmetic operations are supported, both element-wise and with real numbers.

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [interval-class-5-beg]
      :end-before: [interval-class-5-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [interval-class-5-beg]
      :end-before: [interval-class-5-end]
      :dedent: 4

  .. code-tab:: matlab

    x = Interval(2,3)
    y = Interval(1,2)
    z = x + y              % [3, 5]
    z = x - 1              % [1, 2]
    z = 2 * x              % [4, 6]
    z = x / y              % [1, 3]


Advanced operations
-------------------

.. list-table:: Supported advanced methods for a given interval :math:`[x]`
   :widths: 30 70
   :header-rows: 1

   * - Method
     - Description
   * - ``inflate(rad)``
     - Expands the interval by `±rad`
   * - ``bisect([ratio])``
     - Splits into two intervals at given ratio (default is 0.49)
   * - ``complementary()``
     - Computes complement :math:`\mathbb{R} \setminus [x]`
   * - ``diff(y)``
     - Computes set difference :math:`[x] \setminus [y]`
   * - ``rand()``
     - Returns a random sample within the interval
   * - ``init()``
     - Re-initializes to [-∞, ∞]
   * - ``init_from_list(l)``
     - Initializes using the hull of a list of values


Unary and binary functions
--------------------------

Mathematical functions such as :math:`\sin`, :math:`\cos`, :math:`\exp`, :math:`\log`, :math:`\sqrt{\cdot}`, *etc.*, are overloaded to work with intervals. Some examples:

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [interval-class-6-beg]
      :end-before: [interval-class-6-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [interval-class-6-beg]
      :end-before: [interval-class-6-end]
      :dedent: 4

  .. code-tab:: matlab

    x = Interval.half_pi();
    x.self_union(0);        % x = [0, π/2]
    y = sin(x);             % y = [0, 1]
    z = exp(x);             % z = [1, exp(π/2)]

For a complete list of additional operations, see the page :ref:`sec-functions-analytic-operators`.


Literals
--------

In C++, a user-defined literal is provided in C++ to construct an interval directly from a floating-point number:

.. literalinclude:: src.cpp
  :language: c++
  :start-after: [interval-class-6b-beg]
  :end-before: [interval-class-6b-end]
  :dedent: 4


Floating-point adjacency
------------------------

The ``previous_float`` and ``next_float`` functions return the floating-point numbers directly adjacent to a given value.
These functions are useful in the context of interval arithmetic to tightly control rounding directions.

.. doxygenfunction:: codac2::previous_float
  :project: codac

.. tabs::

  .. group-tab:: Python

    .. literalinclude:: src.py
      :language: py
      :start-after: [interval-class-7-beg]
      :end-before: [interval-class-7-end]
      :dedent: 4

  .. group-tab:: C++

    .. literalinclude:: src.cpp
      :language: c++
      :start-after: [interval-class-7-beg]
      :end-before: [interval-class-7-end]
      :dedent: 4

  .. code-tab:: matlab

    x = previous_float(1.0);
    % x = 0.9999999999999999


.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac2_1_1_interval.html>`_.