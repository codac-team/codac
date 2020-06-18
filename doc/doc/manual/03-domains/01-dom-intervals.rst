.. _sec-manual-intervals:

*******************
Intervals and boxes
*******************

We will first only focus on *static* items that are not evolving with time.
The vectors :math:`\mathbf{x}` and matrices :math:`\mathbf{X}`, presented :ref:`in the previous pages <sec-manual-varstatic>`, have their interval counterpart. This page provides the main uses of these sets. They will be involved afterwards for solving problems.

.. contents::

.. note::

  .. Figure:: ../../img/logo_ibex.jpg
    :align: right
  .. Figure:: ../../img/logo_pyibex.jpg
    :align: right
  
  These interval variables come from the `IBEX library <http://www.ibex-lib.org>`_. They are briefly presented here for the sake of consistency. For more information, please refer to the `IBEX documentation <http://www.ibex-lib.org/doc/interval.html#intervals-vectors-and-matrices>`_ for C++ use or to the `pyIbex manual <http://benensta.github.io/pyIbex/sphinx/quickstart.html>`_ for Python usage.


.. _sec-manual-intervals-domains:

Intervals, boxes and interval matrices
--------------------------------------

* | ``Interval x(double lb, double ub)`` will define the interval :math:`[x]`.
  | It is made of its lower and upper bounds :math:`[x^{-},x^{+}]`.

  .. tabs::
    
    .. code-tab:: py

      x = Interval(0, 10)                         # [0,10]
      x = Interval(1, oo)                         # [1,∞]
      x = Interval(-oo, -10)                      # [-∞,-10]

    .. code-tab:: c++

      Interval x(0, 10);                          // [0,10]
      Interval x(1, oo);                          // [1,∞]
      Interval x(-oo, -10);                       // [-∞,-10]

  Some pre-defined values are also at hand:

  .. tabs::
    
    .. code-tab:: py

      x = Interval()                              # [-∞,∞] (default value)
      x = Interval.EMPTY_SET                      # ∅
      x = Interval.PI                             # [π]
      x = Interval.TWO_PI                         # [2π]
      x = Interval.HALF_PI                        # [π/2]

    .. code-tab:: c++

      Interval x;                                 // [-∞,∞] (default value)
      Interval x = Interval::EMPTY_SET;           // ∅
      Interval x = Interval::PI;                  // [π]
      Interval x = Interval::TWO_PI;              // [2π]
      Interval x = Interval::HALF_PI;             // [π/2]

  .. _sec-manual-intervals-pi:

  Note that the constant :math:`[\pi]` is a reliable enclosure of the :math:`\pi` value, that cannot be exactly represented in a computer with a single floating-point value.

  .. tabs::
    
    .. code-tab:: py

      x = Interval.PI                             # [π]
      # x = [3.141592653589793, 3.141592653589794]

    .. code-tab:: c++

      Interval x = Interval::PI;                  // [π]
      // x = [3.141592653589793, 3.141592653589794]

  
.. _sec-manual-intervals-boxes:

* | ``IntervalVector x(int n)`` is used for :math:`n`-d vectors of intervals, also called *boxes*.
  | For instance:

  .. tabs::
    
    .. code-tab:: py

      x = IntervalVector(2, [-1,3])               # creates [x]=[-1,3]×[-1,3]=[-1,3]^2
      y = IntervalVector([[3,4],[4,6]])           # creates [y]= [3,4]×[4,6]
      z = IntervalVector(3, Interval(0,oo))       # creates [z]=[0,∞]^3
      w = IntervalVector(y)                       # creates a copy: [w]=[y]

      v = (0.42,0.42,0.42)                        # one vector (0.42;0.42;0.42)
      iv = IntervalVector(v)                      # creates one box that wraps v:
                                                  #   [0.42,0.42]×[0.42,0.42]×[0.42,0.42]

    .. code-tab:: c++

      IntervalVector x(2, Interval(-1,3));        // creates [x]=[-1,3]×[-1,3]=[-1,3]^2
      IntervalVector y{{3,4},{4,6}};              // creates [y]= [3,4]×[4,6]
      IntervalVector z(3, Interval(0,oo));        // creates [z]=[0,∞]^3
      IntervalVector w(y);                        // creates a copy: [w]=[y]

      Vector v(3, 0.42);                          // one vector (0.42;0.42;0.42)
      IntervalVector iv(v);                       // creates one box that wraps v:
                                                  //   [0.42,0.42]×[0.42,0.42]×[0.42,0.42]
  
  One can access vector components as we do for ``Vector`` objects:

  .. tabs::
    
    .. code-tab:: py

      x[1] = Interval(0,10)                       # updates to [x]=[-1,3]×[0,10]

    .. code-tab:: c++

      x[1] = Interval(0,10);                      // updates to [x]=[-1,3]×[0,10]

  The vector operations to handle ``Vector`` objects can also be used for boxes:

  .. tabs::

    .. code-tab:: py

      n = x.size()                  # box dimension (number of components): 2
      x.resize(5)                   # updates [x] to [-1,3]×[0,10]×[-∞,∞]×[-∞,∞]×[-∞,∞]
      m = x.subvector(1,2)          # creates [m]=[0,10]×[-∞,∞]
      x.put(2,y)                    # updates [x] to [-1,3]×[0,10]×[3,4]×[4,6]×[-∞,∞]

    .. code-tab:: c++

      int n = x.size();             // box dimension (number of components): 2
      x.resize(5);                  // updates [x] to [-1,3]×[0,10]×[-∞,∞]×[-∞,∞]×[-∞,∞]
      IntervalVector m = x.subvector(1,2); // creates [m]=[0,10]×[-∞,∞]
      x.put(2,y);                   // updates [x] to [-1,3]×[0,10]×[3,4]×[4,6]×[-∞,∞]

  .. _sec-manual-intervals-concatenate:

  Lastly, the concatenation of two ``IntervalVector`` can be done with the ``cart_prod`` function:

  .. tabs::
    
    .. code-tab:: py

      a = IntervalVector([[0,1],[2,3]])
      b = IntervalVector([[4,5],[6,7]])
      c = cart_prod(a,b)
      # c: ([0, 1] ; [2, 3] ; [4, 5] ; [6, 7])

    .. code-tab:: c++

      IntervalVector a({{0,1},{2,3}});
      IntervalVector b({{4,5},{6,7}});
      IntervalVector c = cart_prod(a,b);
      // c: ([0, 1] ; [2, 3] ; [4, 5] ; [6, 7])


.. _sec-manual-intervals-matrices:

* | ``IntervalMatrix`` is also available.
  | One can refer to the `documentation of IBEX <http://www.ibex-lib.org/doc/interval.html#matrices-and-array-of-matrices>`_ for more information.


.. _sec-manual-intervals-empty-set:

The empty set
-------------

In mathematics, the empty set is the unique set having no elements; it corresponds to one entity while in Tubex (as in IBEX/pyIbex) there exists one empty set representation for each class of domain.

.. note::

  In our framework, empty sets correspond to domains that do not contain feasible solutions. This may be the result of a **too restrictive definition** of the problem, for instance due to some errors in the model or because of **outliers in the dataset**.

The empty set of an ``Interval`` object is given by:

  .. tabs::
    
    .. code-tab:: py

      x = Interval.EMPTY_SET                      # ∅

    .. code-tab:: c++

      Interval x = Interval::EMPTY_SET;           // ∅

For boxes (interval vectors), we have to specify their dimension even in case of empty set. This differs from mathematical definitions, but allows simple operations when programming with boxes.

  .. tabs::
    
    .. code-tab:: py

      x = IntervalVector.empty(3) # ∅×∅×∅

    .. code-tab:: c++

      IntervalVector x = IntervalVector::empty(3); // ∅×∅×∅


.. _sec-manual-intervals-operations:

Set operations
--------------

Set operations are available for ``Interval`` and ``IntervalVector`` objects (see the `official reference <http://www.ibex-lib.org/doc/interval.html#set-membership-operations>`_). In the following table, if :math:`[x]` is an interval object, :math:`d` is a real value.

====================================  =======================================================
Code                                  Meaning
====================================  =======================================================
``x==y``                              :math:`[x]=[y]`
``x!=y``                              :math:`[x]\neq [y]`
``x.is_empty()``                      :math:`[x]=\varnothing`
``x.is_unbounded()``                  true iff :math:`[x]` has one of its bounds infinite
``x.is_subset(y)``                    :math:`[x]\subseteq [y]`
``x.is_strict_subset(y)``             :math:`[x]\subseteq [y]\wedge [x]\neq [y]`
``x.is_superset(y)``                  :math:`[x]\supseteq [y]`
``x.is_strict_superset(y)``           :math:`[x]\supseteq [y]\wedge [x]\neq [y]`
``x.contains(p)``                     :math:`d\in [x]`
``x.intersects(y)``                   :math:`[x]\cap [y]\neq\varnothing`
``x.is_disjoint(y)``                  :math:`[x]\cap [y]=\varnothing`
``x.overlaps(y)``                     :math:`\mathring{[x]}\cap \mathring{[y]}\neq\varnothing`
====================================  =======================================================

| Where :math:`\mathring{[x]}` denotes the interior of :math:`[x]`.
| In addition of these test functions, operations on sets are available:

====================================  =======================================================
Code                                  Meaning
====================================  =======================================================
``x&y``                               :math:`[x]\cap [y]`
``x|y``                               :math:`[x]\sqcup[y]`
``x.set_empty()``                     :math:`[x]\leftarrow \varnothing`
``x=y``                               :math:`[x]\leftarrow [y]`
``x&=y``                              :math:`[x]\leftarrow ([x]\cap [y])`
``x|=y``                              :math:`[x]\leftarrow ([x]\sqcup[y])`
====================================  =======================================================

Finally, one can also access properties of the sets. First for ``Interval``:

==================  =========================  ==================================================================
Return type         Code                       Meaning
==================  =========================  ==================================================================
``double``          ``x.lb()``                 :math:`\underline{x}`, the lower (left) bound of :math:`[x]`
``double``          ``x.ub()``                 :math:`\overline{x}`, the upper (right) bound of :math:`[x]`
``double``          ``x.diam()``               diameter, :math:`|\overline{x}-\underline{x}|`
``double``          ``x.rad()``                radius, half of the diameter
``double``          ``x.mid()``                the midpoint, (:math:`(\underline{x}+\overline{x})/2`)
``Interval``        ``x.inflate(eps)``         an interval with the same midpoint and radius increased by ``eps``
==================  =========================  ==================================================================

Then for ``IntervalVector``:

==================  =========================  ==================================================================
Return type         Code                       Meaning
==================  =========================  ==================================================================
``Vector``          ``x.lb()``                 lower-left corner (vector of  lower bounds of :math:`[x]`)
``Vector``          ``x.ub()``                 upper-right corner (vector of  upper bounds of :math:`[x]`)
``Vector``          ``x.diam()``               vector of diameters, :math:`|\overline{x_i}-\underline{x_i}|`
``double``          ``x.min_diam()``           minimal diameter, among all components of [x]
``double``          ``x.max_diam()``           maximal diameter, among all components of [x]
``Vector``          ``x.rad()``                vector of radii  (halves of diameters)
``Vector``          ``x.mid()``                the midpoint, (:math:`(\underline{x}+\overline{x})/2`)
``double``          ``x.volume()``             the volume of the box
``bool``            ``x.is_flat()``            true if the volume is null (one dimension is degenerated)
``IntervalVector``  ``x.inflate(eps)``         new box: same midpoint and each radius increased by ``eps``
``bool``            ``x.is_unbounded()``       true iff :math:`[x]` has one of its bounds infinite
==================  =========================  ==================================================================


.. _sec-manual-intervals-arithmetic:

Interval arithmetic
-------------------

Interval analysis is based on the extension of all classical real arithmetic operators.
Consider two intervals :math:`[x]` and :math:`[y]` and an operator :math:`\diamond\in\left\{+,-,\cdot,/\right\}`. We define :math:`[x]\diamond[y]` as the smallest interval containing all feasible values for :math:`x\diamond y`, assuming that :math:`x\in[x]` and :math:`y\in[y]`.

.. math::

  [x]\diamond[y]&=&\left[\left\{x\diamond y\in\mathbb{R} \mid x\in[x],y\in[y]\right\}\right],\\
  \left[x\right]\diamond\varnothing&=&\varnothing.

Dealing with closed intervals, most of the operations can rely on their bounds. It is for instance the case of addition, difference, union, *etc.*:

.. math::

  \begin{eqnarray}
    [x]+[y]&=&\left[\underline{x}+\underline{y},\overline{x}+\overline{y}\right],\\
    \left[x\right]-\left[y\right]& = &\left[\underline{x}-\overline{y},\overline{x}-\underline{y}\right],\\
    \left[x\right]\sqcup\left[y\right]& = &\left[\min\left(\underline{x},\underline{y}\right),\max\left(\overline{x},\overline{y}\right)\right],\\
    \left[x\right]\cap\left[y\right]& = &\left[\max\left(\underline{x},\underline{y}\right),\min\left(\overline{x},\overline{y}\right)\right] \textrm{if} \max\left\{\underline{x},\underline{y}\right\}\leqslant\min\left\{\overline{x},\overline{y}\right\},\nonumber\\
    ~ & = & \varnothing \textrm{ otherwise}.
  \end{eqnarray}

Low-level libraries upon which Tubex has been built provide functionalities for computing arithmetic on intervals, involving basic operations as well as non-linear functions. The following functions can be used:

=========================  ==================================================================
Code                       Meaning
=========================  ==================================================================
``sqr(x)``                 :math:`[x]^2`
``sqrt(x)``                :math:`\sqrt{[x]}`
``pow(x,n)``               :math:`[x]^n`
``pow(x,y)``               :math:`[x]^{[y]} = e^{[y]\log([x])}`
``root(x,n)``              :math:`\sqrt[n]{[x]}`
``exp(x)``                 :math:`\exp([x])`
``log(x)``                 :math:`\log([x])`
``cos(x)``                 :math:`\cos([x])`
``sin(x)``                 :math:`\sin([x])`
``tan(x)``                 :math:`\tan([x])`
``acos(x)``                :math:`\textrm{acos}([x])`
``asin(x)``                :math:`\textrm{asin}([x])`
``atan(x)``                :math:`\textrm{atan}([x])`
``atan2(y,x)``             :math:`\textrm{atan2}([y],[x])`
=========================  ==================================================================

The use on intervals is transparent:

.. tabs::
  
  .. code-tab:: py

    a = Interval(-2,4) * Interval(1,3)                              # a = [-6,12]
    b = Interval(-2,4) & Interval(6,7)                              # b = [empty] (intersection)
    c = max(Interval(2,7), Interval(1,9))                           # c = [2,9]
    d = max(Interval.EMPTY_SET, Interval(1,2))                      # d = [empty]
    e = Interval(-1,3) / Interval(0,oo)                             # e = [-oo,oo]
    f = (Interval(1,2) * Interval(-1,3)) \
      + max(Interval(1,3) & Interval(6,7), Interval(1,2))           # f = [4,9]

  .. code-tab:: c++

    Interval a = Interval(-2,4) * Interval(1,3);                    // a = [-6,12]
    Interval b = Interval(-2,4) & Interval(6,7);                    // b = [empty] (intersection)
    Interval c = max(Interval(2,7), Interval(1,9));                 // c = [2,9]
    Interval d = max(Interval::EMPTY_SET, Interval(1,2));           // d = [empty]
    Interval e = Interval(-1,3) / Interval(0,oo);                   // e = [-oo,oo]
    Interval f = (Interval(1,2) * Interval(-1,3))
               + max(Interval(1,3) & Interval(6,7), Interval(1,2)); // f = [4,9]


------------------------------------------------------

If intervals and boxes are used to handle static variables, tubes provide a way to deal with trajectories.