.. _sec-manual-ctclohner:

************************************************************************
CtcLohner: :math:`\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t)\big)`
************************************************************************

.. contents::


Definition
----------

The Lohner contractor uses Lohner's guaranteed integration algorithm to contract a tube according to a differential
equation. This algorithm performs two main steps:

- an estimation of a global enclosure of the system's trajectories over an integration step (*i.e.* the domain of a slice),
  which corresponds to the codomain of that slice;
- using this estimation and the input gate (or output gate in the backward mode), the corresponding output gate (or input gate
  in the backward mode) is then estimated;
- over a single slice, these steps can be iterated a few times to obtain tighter enclosures for both the gate and the codomain of the slice.

This contractor is supposed to yield better results than :ref:`the Picard contractor <sec-manual-ctcpicard>`, as long as the tubes are "thin enough".

.. important::
    
  .. math::

    \left.\begin{array}{r}\dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t)\big)\end{array}\right. \longrightarrow \mathcal{C}_{\textrm{Lohner}}\big([\mathbf{x}](\cdot)\big)

  .. tabs::

    .. code-tab:: py

      ctc_lohner = CtcLohner(Function("<var1>", "<var2...>", "<exp>"))
      ctc_lohner.contract(x)

    .. code-tab:: c++

      CtcLohner ctc_lohner(Function("<var1>", "<var2...>", "<exp>"));
      ctc_lohner.contract(x);

.. important::

    The contractor might throw a runtime error when it cannot find a global enclosure over a specific time step. This usually
    happens when the time step is too large, and may therefore be avoided by reducing the discretisation frequency of the tube.

    Note that this behaviour may be tackled using automatic step adjustment. However, such a feature is not yet implemented.


Simple example of use
---------------------

In the following example, we consider the two-dimensional system described by:

.. math::

  \left(\begin{array}{c}\dot{x}_1\\\dot{x}_2\end{array}\right) = \left(\begin{array}{c}-x_1\\-\sin({x}_2)\end{array}\right).

The Lohner contractor is used for obtaining a two-dimensional tube, considering an initial condition :math:`\mathbf{x}_0\in[0.9,1.1]\times[0.9,1.1]`. The contraction is tested on two tubes :math:`[\mathbf{a}](\cdot)` and :math:`[\mathbf{b}](\cdot)` of different resolutions (width of slices).

.. tabs::

  .. code-tab:: py

    x0 = IntervalVector(2, [1,1]) # the box [1,1]×[1,1]..
    x0.inflate(0.1)               # ..becomes [0.9,1.1]×[0.9,1.1]

    # 2d tubes:
    a = TubeVector(Interval(0,10), 0.2, 2) # low resolution
    a.set(x0, 0.) # setting initial value

    b = TubeVector(Interval(0,10), 0.01, 2) # high resolution
    b.set(x0, 0.) # setting initial value

    # Defining Lohner contractor from f
    f = Function("x[2]", "(-x[0] ; -sin(x[1]))")
    ctc_lohner = CtcLohner(f)

    # Contracting the tubes
    ctc_lohner.contract(a)
    ctc_lohner.contract(b)

    # Graphics
    beginDrawing()
    fig = VIBesFigTubeVector("Lohner")
    fig.set_properties(100, 100, 600, 300)
    fig.add_tube(a, "a")
    fig.add_tube(b, "b")
    fig.show(True) # displaying slices
    endDrawing()

  .. code-tab:: c++

    IntervalVector x0(2, Interval(1,1)); // the box [1,1]×[1,1]..
    x0.inflate(0.1);                     // ..becomes [0.9,1.1]×[0.9,1.1]

    // 2d tubes:
    TubeVector a(Interval(0,10), 0.2, 2); // low resolution
    a.set(x0, 0.); // setting initial value

    TubeVector b(Interval(0,10), 0.01, 2); // high resolution
    b.set(x0, 0.); // setting initial value

    // Defining Lohner contractor from f
    Function f("x[2]", "(-x[0] ; -sin(x[1]))");
    CtcLohner ctc_lohner(f);

    // Contracting the tubes
    ctc_lohner.contract(a);
    ctc_lohner.contract(b);

    // Graphics
    vibes::beginDrawing();
    VIBesFigTubeVector fig("Lohner");
    fig.set_properties(100, 100, 600, 300);
    fig.add_tube(&a, "a");
    fig.add_tube(&b, "b");
    fig.show(true); // displaying slices
    vibes::endDrawing();


The above code yields the following result for :math:`[a_1](\cdot)` (in gray, large slices) and :math:`[b_1](\cdot)` (in blue, thin slices):

.. figure:: img/clohner.png




Related content
---------------

.. admonition:: Mathematical documentation

  See `Auguste Bourgois' thesis <https://www.ensta-bretagne.fr/jaulin/thesis_auguste.pdf>`_. In particular, check Chapter 4.

.. admonition:: Technical documentation

  See the `C++ API documentation of this class <../../../api/html/classcodac_1_1_ctc_lohner.html>`_.
