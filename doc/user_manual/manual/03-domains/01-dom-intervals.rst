.. _sec-manual-intervals:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*******************
Intervals and boxes
*******************

We will first only focus on *static* items that are not evolving with time.
The vectors :math:`\mathbf{x}` and matrices :math:`\mathbf{X}`, presented :ref:`in the previous pages <sec-manual-varstatic>`, have their interval counterpart. This page provides the main uses of these sets. They will be involved afterwards for solving problems.

.. contents::

.. note::

  .. Figure:: ../../img/ibex_logo.jpg
    :align: right
  .. Figure:: ../../img/pyibex_logo.jpg
    :align: right
  
  These interval variables come from the `IBEX library <http://www.ibex-lib.org>`_. They are briefly presented here for the sake of consistency. For more information, please refer to the `IBEX documentation <http://www.ibex-lib.org/doc/interval.html#intervals-vectors-and-matrices>`_ for C++ use or to the `pyIbex manual <http://benensta.github.io/pyIbex/sphinx/quickstart.html>`_ for Python usage.


.. _sec-manual-intervals-domains:

Intervals, boxes and interval matrices
--------------------------------------

* | ``Interval x(double lb, double ub)`` will define the interval :math:`[x]`.
  | It is made of its lower and upper bounds :math:`[x^{-},x^{+}]`.

  .. tabs::

    .. code-tab:: c++

      Interval x(0, 10);                          // [0,10]
      Interval x(1, POS_INFINITY);                // [1,∞]
    
    .. code-tab:: py

      # todo

  Some pre-defined values are also at hand:

  .. tabs::

    .. code-tab:: c++

      Interval x;                                 // [-∞,∞] (default value)
      Interval x = Interval::all_reals();         // [-∞,∞]
      Interval x = Interval::pos_reals();         // [0,∞]
      Interval x = Interval::neg_reals();         // [-∞,0]
      Interval x = Interval::empty_set();         // ∅
      Interval x = Interval::pi();                // [π]
      Interval x = Interval::two_pi();            // [2π]
      Interval x = Interval::half_pi();           // [π/2]
    
    .. code-tab:: py

      # todo

* | ``IntervalVector x(int n)`` is used for vectors of intervals, also called *boxes*.
  | For instance:

  .. tabs::

    .. code-tab:: c++

      IntervalVector x(2, Interval(-1,3));        // creates [x]=[-1,3]x[-1,3]=[-1,3]^2
      IntervalVector y{{3,4},{4,6}};              // creates [y]= [3,4]x[4,6]
      IntervalVector z(3, Interval::pos_reals()); // creates [z]=[0,∞]^3
    
    .. code-tab:: py

      # todo
  
  One can access vector components as we do for ``Vector`` objects:

  .. tabs::

    .. code-tab:: c++

      x[1] = Interval(0,10);                      // updates to [x]=[-1,3]x[0,10]
    
    .. code-tab:: py

      # todo


.. _sec-manual-intervals-empty-set:

The empty set
-------------


.. _sec-manual-intervals-operations:

Set operations
--------------



.. _sec-manual-intervals-arithmetic:

Interval arithmetic
-------------------

One can compute the following simple operations on intervals:

.. tabs::

  .. code-tab:: c++

    Interval a = Interval(-2,4) * Interval(1,3);                    // a = [-6,12]
    Interval b = Interval(-2,4) & Interval(6,7);                    // b = [empty] (intersection)
    Interval c = max(Interval(2,7), Interval(1,9));                 // c = [2,9]
    Interval d = max(Interval::EMPTY_SET,Interval(1,2));            // d = [empty]
    Interval e = Interval(-1,3) / Interval::POS_REALS;              // e = [-oo,oo]
    Interval f = (Interval(1,2) * Interval(-1,3))
               + max(Interval(1,3) & Interval(6,7), Interval(1,2)); // f = [4,9]
  
  .. code-tab:: py

    # todo
