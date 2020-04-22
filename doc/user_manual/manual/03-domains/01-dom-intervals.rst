.. _sec-manual-intervals-label:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently perfomed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*******************
Intervals and boxes
*******************

.. contents::

We will first only focus on *static* variables that are not evolving with time.
These variables are reals of :math:`\mathbb{R}` or vectors.
Tubex is using C++ objects [#f1]_ to represent their domains: intervals and boxes.

* | ``Interval x(double lb, double ub)`` will define the interval :math:`[x]`.
  | It is made of its lower and upper bounds :math:`[x^{-},x^{+}]`.

  .. code-block:: c++

    Interval x(0, 10);                // [0,10]
    Interval x(1, POS_INFINITY);      // [0,oo]

  Some pre-defined values are also at hand:

  .. code-block:: c++

    Interval x;                       // [-oo,oo] (default value)
    Interval x = Interval::ALL_REALS; // [-oo,oo]
    Interval x = Interval::POS_REALS; // [0,oo]
    Interval x = Interval::EMPTY_SET;
    // etc.


* | ``IntervalVector x(int n)`` is used for boxes.
  | For instance, ``IntervalVector x(2, Interval(-1,3))`` creates the box :math:`[\mathbf{x}]=[-1,3]\times[-1,3]=[-1,3]^2` and ``IntervalVector y{{3,4},{4,6}}`` creates the box :math:`[\mathbf{y}]=[3,4]\times[4,6]`.
  | One can access vector components as we do for common vector sets: ``x[1] = Interval(0,10);``

In your new project, you can compute the following simple operations on intervals:

.. code-block:: c++

  Interval a = Interval(-2,4) * Interval(1,3);                    // a = [-6,12]
  Interval b = Interval(-2,4) & Interval(6,7);                    // b = [empty] (intersection)
  Interval c = max(Interval(2,7), Interval(1,9));                 // c = [2,9]
  Interval d = max(Interval::EMPTY_SET,Interval(1,2));            // d = [empty]
  Interval e = Interval(-1,3) / Interval::POS_REALS;              // e = [-oo,oo]
  Interval f = (Interval(1,2) * Interval(-1,3))
             + max(Interval(1,3) & Interval(6,7), Interval(1,2)); // f = [4,9]


.. admonition:: Example

  todo



.. rubric:: Footnotes
.. [#f1] The static objects, that are not time-related, are the ones defined in `the IBEX library <http://www.ibex-lib.org/>`_