.. _sec-tuto-cprob-lesson-a:

Lesson A: Static range-and-bearing localization
===============================================

In this lessson, we focus on the perception of landmarks clearly identified: each observation is related to a known position.
The problem amounts to estimate the feasible positions of a robot measuring distances and bearing from known landmarks.
The *bearing* corresponds to the angular position of the landmarks with respect to the heading of the robot.

We will see how to handle the related constraints. From this example, we will understand the principles of *decomposition of constraints* and the *polar equations*.


Formalism
---------

.. image:: img/loc_gonio_dist.png
  :align: right
  :width: 350px

For the moment, the map is made of one landmark. The localization problem corresponds to the following observation function:

.. math::

  \mathbf{y}=\mathbf{g}\big(\mathbf{x}\big)

where :math:`\mathbf{x}` is the unknown state vector: the robot is located at :math:`\left(x_{1},x_{2}\right)^\intercal` with a heading :math:`x_{3}`. The output measurement vector is :math:`\mathbf{y}`, made of the range :math:`y_{1}` and the bearing :math:`y_{2}`, as illustrated by the figure. It is assumed that the measurement is bounded: :math:`\mathbf{y}\in\left[\mathbf{y}\right]`, contrary to the state :math:`\mathbf{x}` that is completely unknown: :math:`\mathbf{x}\in[\mathbf{x}]=[-\infty,\infty]^3`. We say that :math:`[\mathbf{x}]` is an *unbounded* set.

Often, the observation equation also appears in the following implicit form:

.. math::

  \mathbf{g}\big(\mathbf{x},\mathbf{y}\big)=\mathbf{0}.

In our *range-and-bearing* case, the observation equation is then defined as:

.. math::

  \mathbf{g}(\mathbf{x},\mathbf{y})=\left(\begin{array}{c}
  x_{1}+y_{1}\cdot\cos\left(x_{3}+y_{2}\right)-m_1\\
  x_{2}+y_{1}\cdot\sin\left(x_{3}+y_{2}\right)-m_2
  \end{array}\right),

| with :math:`\mathbf{m}`, the 2d position of the landmark.

.. | Note that in a *range-only* case where the robot only measures the distance :math:`y` to the landmark, we obtain the following relation:

.. .. math::
  
..   g(\mathbf{x},y)=\left(x_{1}-m_1\right)^{2}+\left(x_{2}-m_2\right)^{2}-y^{2}.

| In the more general case, the observation function :math:`\mathbf{g}` is uncertain, or more precisely, it depends on some parameters that are not exactly known. For instance, the position of the landmark, denoted by the 2d vector :math:`\mathbf{m}`, may be uncertain. It can even be completely unknown if the landmark is not identified; this will be the case in the next lesson.
| The related observation equation can be written as:

.. math::

  \mathbf{g}\big(\mathbf{x},\mathbf{y},\mathbf{m}\big)=\mathbf{0}.

In a set-membership approach, the uncertainties related to the state :math:`\mathbf{x}`, the measurement :math:`\mathbf{y}` and the landmark :math:`\mathbf{m}` will be implicitly represented by interval-vectors (boxes): :math:`\mathbf{x}\in[\mathbf{x}]`, :math:`\mathbf{y}\in[\mathbf{y}]` and :math:`\mathbf{m}\in[\mathbf{m}]`.

We will solve this problem using the constraint programming approach.


Decomposition of constraints
----------------------------

Problems involving complex equations can be broken down into a set of **primitive constraints**. Here, *primitive* means that the constraints cannot be decomposed anymore. For instance, in a range-only localization problem, the observation constraint :math:`\mathcal{L}_{\textrm{dist}}` can be broken down into: 

.. math::

  \mathcal{L}_{\textrm{dist}}: y=\sqrt{\left(x_1-m_1\right)^2+\left(x_2-m_2\right)^2} \Longleftrightarrow \left\{\begin{array}{rcll}
    a & = & x_1-m_1 & \big(\mathcal{L}_{-}\big)\\
    b & = & x_2-m_2 & \big(\mathcal{L}_{-}\big)\\
    c & = & a^2 & \big(\mathcal{L}_{(\cdot)^2}\big)\\
    d & = & b^2 & \big(\mathcal{L}_{(\cdot)^2}\big)\\
    e & = & c+d & \big(\mathcal{L}_{+}\big)\\
    y & = & \sqrt{e} & \big(\mathcal{L}_{\sqrt{\cdot}}\big)
  \end{array}\right.

where :math:`a,b,\dots,e` are intermediate variables used for the decomposition. This constitutes a network made of the :math:`\mathcal{L}_{-}`, :math:`\mathcal{L}_{+}`, :math:`\mathcal{L}_{(\cdot)^2}`, and :math:`\mathcal{L}_{\sqrt{\cdot}}` elementary constraints.

| The library provides a way to automatically make this decomposition, select already existing **elementary contractors** such as :math:`\mathcal{C}_{+}`, :math:`\mathcal{C}_{(\cdot)^2}`, :math:`\mathcal{C}_{\sqrt{\cdot}}` and build the complex contractor modeling the constraint :math:`\mathcal{L}_{\textrm{dist}}`.
| This can be done implicitly with:

.. | This was implicitly done in the previous :ref:`Lesson A <sec-tuto-01>` with:

.. tabs::

  .. code-tab:: py

    # Symbolic variables:
    y = ScalarVar()
    x,m = VectorVar(2),VectorVar(2)

    # Analytic scalar function g(x,m,y) involved in the constraint:
    g = AnalyticFunction([x,y,m], sqrt(((x[0]-m[0])^2)+((x[1]-m[1])^2))-y)

    # Contractor associated with the constraint g(x,m,y)\in[u], with [u]=[0,0]
    ctc_g = CtcInverse(g, 0)

    # Now ctc_g can be called with the .contract(..) method to contract all domains:
    # Example:
    a = IntervalVector(2) # box for x
    b = IntervalVector([[2,3],[5,6]]) # box for m
    d = Interval(4.5,5) # interval for y
    a,d,b = ctc_g.contract(a,d,b)

  .. code-tab:: c++

    // Symbolic variables:
    ScalarVar y;
    VectorVar x(2), m(2);

    // Analytic scalar function g(x,m,y) involved in the constraint:
    AnalyticFunction g({x,y,m}, sqrt((x[0]-m[0])^2+(x[1]-m[1])^2)-y);

    // Contractor associated with the constraint g(x,m,y)\in[u], with [u]=[0,0]
    CtcInverse ctc_g(g, 0);

    // Now ctc_g can be called with the .contract(..) method to contract all domains:
    // Example:
    IntervalVector a(2); // box for x
    IntervalVector b({{2,3},{5,6}}); // box for m
    Interval d(4.5,5); // interval for y
    ctc_g.contract(a,d,b);


Optimality of contractors
-------------------------

Depending on the properties of the equation, the resulting contractor can be **optimal**. It means that the contracted box will perfectly enclose the set of feasible solutions **without pessimism**. This enables an efficient resolution.

In other cases, for instance because of dependencies between the variables, the resulting operator may not be optimal.
For instance, looking at the equation depicting the above problem of *range-and-bearing* localization, the formula

.. math::

  \mathbf{g}(\mathbf{x},\mathbf{y},\mathbf{m})=\left(\begin{array}{c}
  x_{1}+y_{1}\cdot\cos\left(x_{3}+y_{2}\right)-m_1\\
  x_{2}+y_{1}\cdot\sin\left(x_{3}+y_{2}\right)-m_2
  \end{array}\right).

could be implemented by

.. tabs::

  .. code-tab:: py

    # Symbolic variables:
    x,m,y = VectorVar(3),VectorVar(2),VectorVar(2)

    # Analytic vectorial function g(x,y,m) involved in the constraint:
    g = AnalyticFunction([x,y,m], [
      x[0]+y[0]*cos(x[2]+y[1])-m[0],
      x[1]+y[0]*sin(x[2]+y[1])-m[1]
    ])

    # Contractor associated with the constraint g(x,m,y)\in[u] with [u]=[[0,0],[0,0]]
    ctc_g = CtcInverse(g, [0,0]) # [0,0] is equivalent to Vector.zero(2)

  .. code-tab:: c++

    // Symbolic variables:
    VectorVar x(3), m(2), y(2);

    // Analytic vectorial function g(x,y,m) involved in the constraint:
    AnalyticFunction g({x,y,m}, {
      x[0]+y[0]*cos(x[2]+y[1])-m[0],
      x[1]+y[0]*sin(x[2]+y[1])-m[1]
    });

    // Contractor associated with the constraint g(x,m,y)\in[u] with [u]=[[0,0],[0,0]]
    CtcInverse ctc_g(g, {0,0}) // {0,0} is equivalent to Vector::zero(2)

However, this involves a multi-occurrence of variables which leads to pessimism. For instance, the sum :math:`(x_3+y_2)` appears twice in functions :math:`\cos` and :math:`\sin`, which is hardly handled by a classical decomposition.


We may instead use a **dedicated contractor** that deals with the **polar constraint**, appearing in the above function :math:`\mathbf{g}`. This will allow us to avoid multi-occurrences and then, pessimism.


The polar constraint
--------------------

The range-and-bearing problem is usually related to the *polar constraint* that links Cartesian coordinates to polar ones. This constraint is often encountered in robotics and it is important to be able to deal with it in an optimal way.

This geometrical constraint is expressed by:

.. math::

  \mathcal{L}_\textrm{polar}\big(d_1,d_2,\rho,\theta\big):~~~\mathbf{d}=\rho\cdot\left(\begin{array}{c}
  \cos(\theta)\\
  \sin(\theta)
  \end{array}\right),

| where :math:`\rho` and :math:`\theta` are respectively the radial and angular coordinates.
| We could deal with :math:`\mathcal{L}_\textrm{polar}` by decomposition, but we would lose optimality. Therefore, a dedicated contractor exists to deal with it without decomposition: the ``CtcPolar``.

This constraint appears in the expression of :math:`\mathbf{g}`.


.. admonition:: Exercise

  **A.1.** On a sheet of paper, write a decomposition of function :math:`\mathbf{g}` that involves :math:`\mathcal{L}_\textrm{polar}`, other constraints and intermediate variables.
  
  :download:`See the solution <doc/solution_a1.pdf>`


Initialization
--------------

A robot depicted by the state :math:`\mathbf{x}=\left(2,1,\pi/6\right)^\intercal` is perceiving a landmark :math:`\mathbf{m}=\left(5,6\right)^\intercal` at a range :math:`y_1=6` and a bearing :math:`y_2=\pi/6`. We assume that the position of the robot is not known and that the uncertainties related to the measurement and the landmark are:

* **Measurement.** For :math:`y_1`: :math:`[-0.3,0.3]`, for :math:`y_2`: :math:`[-0.1,0.1]`.
* **Landmark.** For each component of the position: :math:`[-0.2,0.2]`.

.. admonition:: Exercise

  **A.2.** In a new project, create the vectors ``x_truth``, ``y_truth``, ``m_truth`` representing the actual but unknown values of :math:`\mathbf{x}=\left(2,1,\pi/6\right)^\intercal`, :math:`\mathbf{y}=\left(6,\pi/6\right)^\intercal` and :math:`\mathbf{m}=\left(5,6\right)^\intercal`.

  .. tabs::

    .. code-tab:: py

      # We recall that you can use the Vector class for
      # representing mathematical vectors. For instance:

      x_truth = Vector([2,1,PI/6]) # actual state vector (pose = position + bearing)

    .. code-tab:: cpp

      // We recall that you can use the Vector class for
      // representing mathematical vectors. For instance:

      Vector x_truth = {2,1,PI/6}; // actual state vector (pose = position + bearing)


  **A.3.** Create the bounded sets related to the state, the measurement and the landmark position: :math:`[\mathbf{x}]\in\mathbb{IR}^3`, :math:`[\mathbf{y}]\in\mathbb{IR}^2`, :math:`[\mathbf{m}]\in\mathbb{IR}^2`. We can for instance use the ``.inflate(float radius)`` method on intervals or boxes.

  | In addition, the heading of the robot is assumed precisely known (for instance thanks to a compass):

  .. tabs::

    .. code-tab:: py

      x[2] &= x_truth[2] # the heading is assumed to be known

    .. code-tab:: cpp

      x[2] &= x_truth[2]; // the heading is assumed to be known

  **A.4.** Display the vehicle and the landmark with:

  .. tabs::

    .. code-tab:: py

      DefaultFigure.draw_tank(x_truth, size=1, style=[Color.black(),Color.yellow()])
      DefaultFigure.draw_box(m, Color.red())

    .. code-tab:: c++

      DefaultFigure::draw_tank(x_truth, 1, {Color::black(),Color::yellow()}); // robot's size is 1
      DefaultFigure::draw_box(m, Color::red());

  **A.5.** Display the range-and-bearing measurement with its uncertainties. For this, we will use the function ``.draw_pie(<x>, <y>, <[rho]>, <[theta]>)`` to display a portion of a ring :math:`[\rho]\times[\theta]` centered on :math:`(x,y)^\intercal`. Here, we must add in :math:`[\theta]` the robot heading :math:`x_3` and the bounded bearing :math:`[y_2]`.

  You should obtain this figure:

  .. figure:: img/first_result.gif
    :width: 250px

  On this figure, we may also draw the origin of the measurement (in light gray). This can be done with:

  .. tabs::

    .. code-tab:: py

      DefaultFigure.draw_pie(<x>, <y>, <[rho]> | 0, <[theta]>, Color.light_gray()) # with: <[rho]> | 0

    .. code-tab:: c++

      DefaultFigure::draw_pie(<x>, <y>, <[rho]> | 0, <[theta]>, Color::light_gray()); // with: <[rho]> | 0

As one can see, intervals are not limited to axis-aligned boxes: we sometimes perform rotational mapping to better fit the set to represent. This polar constraint is a case in point.


State estimation
----------------

We will implement the decomposition of Question **A.1** using contractors and a ``fixpoint`` function that will manage the constraint propagation for us.

.. admonition:: Exercise
  
  *The following must be made before the graphical part.*

  **A.6.** Create the contractors related to the decomposition of Question **A.1**.

  * | The contractor :math:`\mathcal{C}_\textrm{polar}` is provided by the class ``CtcPolar``. :ref:`See more <sec-ctc-geom-ctcdist>`.
  * The other contractors can be built with several ``CtcInverse`` objects. We recall that these constraints have to be expressed in the form :math:`f(\mathbf{x})=0`.

  | **A.7.**  Create the intermediate variables introduced in Question **A.1**. They are ``Interval`` and ``IntervalVector`` objects, as for the other variables.
  | Note that the intermediate variables do not have to be initialized with prior values. For ``IntervalVector`` objects, you only have to define their size.

The current problem can be solved in one step with a smart order of contractors. You may find this order and run the contractors without iterative loop. However, this order may not always exist (due to dependencies in the constraints) or, in a pure declarative paradigm, we may not want to think about it. Using fixed-point iteration can then simplify the algorithm. In Codac, this is allowed using the ``fixpoint(..)`` function that will execute some ``contractors_list`` function until a fixpoint is reached. The `fixpoint` corresponds to a point where the involved domains are not contracted anymore.

The following code illustrates how to implement such fixpoint:

.. tabs::

  .. code-tab:: py

    def contractors_list(<domains related to c1,c2,..>):
      <domains related to c1> = <c1>.contract(<domains related to c1>)
      <domains related to c2> = <c2>.contract(<domains related to c2>)
      etc.
      return <domains related to c1,c2,..>

    <domains related to c1,c2,..> = fixpoint(contractors_list, <domains related to c1,c2,..>)

  .. code-tab:: c++

    fixpoint([&]() // lambda function `contractors_list`
    {
      <c1>.contract(<domains related to c1>);
      <c2>.contract(<domains related to c2>);
      etc.
    }, <domains related to c1,c2,..>);

The ``fixpoint`` function will execute the content of the function ``contractors_list`` until there are no more contractions on the sets listed in ``<domains related to c1,c2,..>``.

.. admonition:: Exercise

  | **A.8.** Call the contractors at stake, possibly using a fixpoint procedure.

  | **A.9.**  Run your programm to solve the problem. You should obtain this figure:

  .. figure:: img/result_rangebearing.png
    :width: 250px

The black box :math:`[\mathbf{x}]` cumulates all the uncertainties of the problem:

* uncertainties of the measurement
* uncertainties of the position of the landmark

If we remove the uncertainties related to the measurement :math:`[\mathbf{y}]`, then the width of :math:`[\mathbf{x}]` should be exactly the same as the one of :math:`[\mathbf{m}]`, because we used optimal contractors. The width of a box is given by the ``.diam()`` method.