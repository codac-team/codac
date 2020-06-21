.. _sec-tuto-05:

Lesson E: Dynamic localization with asynchronous measurements
=============================================================

Mobile robot motion is usually described by an evolution function :math:`\mathbf{f}` involved in a **differential equation**. The interval state estimation then amounts to estimating the set of **feasible trajectories** that are solutions of this differential equation. The difficulty is to propagate the uncertainties in time, and to consider observations from asynchronous measurements in the same time. We will see that these difficulties are easily handled with constraint propagations on **tubes**.


.. contents:: Content of this lesson


Formalism
---------

We now address the following problem:

.. math:: 

  \left\{ \begin{array}{lll}
  \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),\mathbf{u}(t)\big) &  & \textrm{(evolution equation)}\\
  \mathbf{y}^{i}=\mathbf{g}\big(\mathbf{x}(t_{i})\big) &  & \textrm{(observation equation)}
  \end{array}\right.

We recall that

* :math:`\mathbf{x}\in\mathbb{R}^n` is the state vector
* :math:`\mathbf{u}\in\mathbb{R}^m` is some input vector
* :math:`\mathbf{y}^i\in\mathbb{R}^p` is a measurement made at time :math:`t_i`
* and :math:`\mathbf{f}`, :math:`\mathbf{g}` are respectively *evolution* and *observation* functions, possibly non-linear or uncertain

The :math:`\mathbf{x}` and :math:`\mathbf{u}` values :underline:`continuously` evolve with time. We will now handle them as **trajectories**.


.. rubric:: What is a trajectory

A trajectory :math:`x(\cdot):[t_0,t_f]\to\mathbb{R}` is a set of real values defined over some temporal domain :math:`[t_0,t_f]`, called *t*-domain.
They are aimed at **representing temporal evolutions**. We implement them with the ``Trajectory`` class (or the ``TrajectoryVector`` class for trajectories :math:`\mathbf{x}(\cdot):[t_0,t_f]\to\mathbb{R}^n`).

These trajectories correspond to a new kind of variable in the constraint programming approach. We will consider them together with other values such as reals of vectors, as we did in the previous lessons.


.. rubric:: New type of variables :math:`\implies` new type of domain

Because we will deal with a new kind of variable (trajectories :math:`x(\cdot)`), we also define a new type of domain to handle them with contractors: **tubes** :math:`[x](\cdot)`.

A tube :math:`[x](\cdot)` is defined over a temporal *t*-domain :math:`[t_0,t_f]` as an envelope of trajectories that are defined over the same *t*-domain. We speak about an *envelope* as it may exist trajectories enclosed in the tube that are not solutions of our problem. A trajectory :math:`x(\cdot)` belongs to the tube :math:`\left[x\right](\cdot)` if :math:`\forall t\in[t_0,t_f], x\left(t\right)\in\left[x\right]\left(t\right)`. 

.. figure:: ../../manual/03-domains/img/tube_def.png

  Illustration a one-dimensional tube enclosing a trajectory :math:`x^*(\cdot)` plotted in orange. The figure shows two interval evaluations: :math:`[x]([t_1])` and :math:`[x](t_2)`.

.. note::

  **Important:** we assume that all the tubes and/or the trajectories involved in a given resolution process share the same *t*-domain :math:`[t_0,t_f]`.

The ``Tube`` class allows to create a tube.


Using trajectories
------------------

Let us consider a robot following a `Lissajous curve <https://en.wikipedia.org/wiki/Lissajous_curve>`_ from :math:`t_0=0` to :math:`t_f=5`:

.. math::

  \mathbf{x}^*(t) = \left(\begin{array}{c}2\cos(t)\\\sin(2t)\end{array}\right)

The following code creates the actual (but unknown) trajectory :math:`\mathbf{x}^*(\cdot)`:

.. tabs::

  .. code-tab:: py

    tdomain = Interval(0,5) # temporal domain [t0,tf]
    actual_x = TrajectoryVector(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), 0.01)

  .. code-tab:: c++

    Interval tdomain(0.,5.); // temporal domain [t0,tf]
    TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), 0.01);

.. figure:: ../../manual/02-variables/img/lissajous.png

  Top view. The yellow robot follows a Lissajous curve forming an :math:`\infty` symbol.

| The ``TFunction`` object depicts a temporal function. It is similar to ``Function`` objects we have seen previously, but the ``t`` variable is already defined. In this example, the output of the function is two-dimensional (defined with parenthesis).
| The last argument ``0.01`` converts the analytical function into a discretized signal with :math:`\delta=0.01`.


.. admonition:: Exercise

  **E.1.** Create a new project with the above trajectory :math:`\mathbf{x}^*(\cdot)`.

  **E.2.** Create a landmark object represented by the vector :math:`\mathbf{b}=(0.5,1)`.

  **E.3.** In a ``VIBesFigMap`` figure, project the 2d trajectory in the view with the ``.add_trajectory()`` method.
  The arguments respectively refer to the object to display, its name in the view, the x-index component and the y-index component:

  .. tabs::

    .. code-tab:: py

      fig_map = VIBesFigMap("Map")
      fig_map.set_properties(100, 100, 600, 300)
      fig_map.add_trajectory(actual_x, "x*", 0, 1)
      fig_map.add_beacon(b, 0.1)                    # 0.1: landmark width
      fig_map.axis_limits(-2.5,2.5,-0.1,0.1, True)
      fig_map.show(0.25)                            # argument is robot size

    .. code-tab:: cpp
    
      VIBesFigMap = fig_map("Map");
      fig_map.set_properties(100, 100, 600, 300);
      fig_map.add_trajectory(actual_x, "x*", 0, 1);
      fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
      fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
      fig_map.show(0.25);                           // argument is robot size

  **E.4.** The robot continuously measures its distance to the landmark :math:`\mathbf{b}`.

  Classical mathematical functions such as :math:`+`, :math:`\cos`, :math:`\exp`, :math:`\sqrt{\cdot}`, can be used on trajectories.
  Compute the distance signal ``dist`` between the robot and the landmark as one ``Trajectory object``.

  **E.5.** Display the ``dist`` trajectory in another dedicated view with a ``VIBesFigTube`` object:

  .. tabs::

    .. code-tab:: py

      fig_dist = VIBesFigTube("Distance to the landmark")
      fig_dist.set_properties(100, 100, 600, 300)
      fig_dist.add_trajectory(dist, "dist")
      fig_dist.show()

    .. code-tab:: cpp
    
      VIBesFigTube fig_dist("Distance to the landmark");
      fig_dist.set_properties(100, 100, 600, 300);
      fig_dist.add_trajectory(&dist, "dist");
      fig_dist.show();

  You should obtain this figure:

  .. figure:: ../../manual/02-variables/img/distances.png

    Result of simulated range measurements: the ``dist`` trajectory object.



Enclosing the feasible trajectories of a robot
----------------------------------------------




Contractors for differential equations
--------------------------------------



Dead-reckoning
--------------