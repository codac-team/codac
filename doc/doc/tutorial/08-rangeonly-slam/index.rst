.. _sec-tuto-08:

Lesson H: Range-only SLAM
=========================

This lesson is a summary of all the lessons of this tutorial. We will apply the concepts of constraints and interval analysis on a concrete SLAM problem, and see how an online SLAM can be solved.


.. contents:: Content of this lesson


.. tip::

  This exercise comes from IAMOOC: another MOOC related to Interval Analysis with applications to parameter estimation and robot localization. It provides complementary concepts and may be of interest to you. https://www.ensta-bretagne.fr/jaulin/iamooc.html

  This lesson is an adaptation of the Exercise 11 of IAMOOC. The difference is that we will now consider a continuous-time state equation.


Robot motion
------------

Consider a robot at position :math:`(x_1,x_2)` moving in an unknown environment.
Its motion is described by the state equations:

.. math::

  \left\{ \begin{array}{rcl}
    \dot{x}_1&=&10\cos(x_3) \\
    \dot{x}_2&=&10\sin(x_3) \\
    \dot{x}_3&=&u + n_u
  \end{array}\right.

where :math:`x_3` is the heading of the vehicle, :math:`u` is the desired rotational speed and :math:`n_u` is a noise.
The simulation runs from :math:`t_0=0` to :math:`t_f=15`.

Contrary to the previous lesson, we assume that we know the initial state :math:`\mathbf{x}_0=(0,0,2)^\intercal`. This is common in SLAM problems.

The desired input :math:`u(t)` is chosen as:

.. math::

  u(t) = 3(\sin(t)^2)+\frac{t}{100}.

We assume that the heading is measured (for instance by using a compass) with a small error:

.. math::

  x_3^m(t) = x_3^*(t) + n_{x_3}(t).

At any time, we assume that the errors :math:`n_u(t)` and :math:`n_{x_3}(t)` are bounded by :math:`[-0.03,0.03]`.

.. admonition:: Exercise

  **H.1.** Simulate the system using a uniform random noise (or a ``RandTrajectory``) and draw the tube enclosing the trajectory :math:`\mathbf{x}^*(\cdot)`. We will use :math:`\delta` = ``dt`` = :math:`0.05` for implementing trajectories and tubes.

  The tube of derivatives :math:`[\mathbf{v}](\cdot)` (containing the solutions of :math:`\dot{\mathbf{x}}(\cdot)`) can be computed with arithmetic functions. Furthermore, in this dead-reckoning situation (where only the initial state and the motion are known), the tube :math:`[\mathbf{x}](\cdot)` can be obtained as the primitive of :math:`[\mathbf{v}](\cdot)`:

  .. tabs::

    .. code-tab:: py

      x = v.primitive() + IntervalVector(x0) # dead reckoning

    .. code-tab:: cpp

      TubeVector x = v.primitive() + x0; // dead reckoning

You should obtain a result similar to:

.. figure:: img/slam_deadrecko.png


Perceiving landmarks
--------------------

The environment is made of 4 landmarks. Their coordinates are given in the following table.

  =========  =============================
  :math:`i`  Landmark :math:`\mathbf{b}_i`
  =========  =============================
  :math:`0`  :math:`(6,12)^\intercal`
  :math:`1`  :math:`(-2,-5)^\intercal`
  :math:`2`  :math:`(-3,20)^\intercal`
  :math:`3`  :math:`(3,4)^\intercal`
  =========  =============================

The robot does not know these coordinates. Each :math:`t=2k\delta`, the robot is able to measure the distance to one of these landmarks (taken randomly), with an accuracy of :math:`\pm0.03`.

.. admonition:: Exercise

  **H.2.** Using a Contractor Network, improve the localization of the robot while simultaneously estimating the position of the landmarks by enclosing them into boxes.


You should obtain a result similar to:

.. figure:: img/slam_final.png


Online SLAM
-----------

These computations were made offline, assuming that all the data were collected before running the solver.

We could use this approach **online** and make the solver run during the evolution of the robot. For this, we will use the ``.contract_during(ctc_dt)`` method instead of ``.contract()``. This way, we will let the solver contract as much as possible the domains **during a given amount of time** ``ctc_dt``. Remaining contractions will be done during the next call to ``.contract_during()``.

Hence, for real-time SLAM, we can use the following temporal loop:

.. tabs::

  .. code-tab:: py

    import time # used for time.sleep

    dt = 0.05
    iteration_dt = 0.2 # elapsed animation time between each dt
    tdomain = Interval(0,15) # [t0,tf]

    # ...

    # Create tubes defined over [t0,tf]
    # Add already known constraints, such as motion equations

    t = tdomain.lb()
    prev_t_obs = t

    while t < tdomain.ub(): # run the simulation from t0 to tf

      if t-prev_t_obs > 2*dt: # new observation each 2*dt
      
        # Creating new observation to a random landmark

          ...

        # Adding related observation constraints to the network

          ...

      contraction_dt = cn.contract_during(iteration_dt)
      if iteration_dt>contraction_dt: # pause for the animation
        time.sleep(iteration_dt-contraction_dt) # iteration delay

      # Display the current slice [x](t)
      fig_map.draw_box(x(t).subvector(0,1))

      t+=dt

  .. code-tab:: cpp

    double dt = 0.05;
    double iteration_dt = 0.2; // elapsed animation time between each dt
    Interval tdomain(0,15); // [t0,tf]

    // ...

    // Create tubes defined over [t0,tf]
    // Add already known constraints, such as motion equations

    double prev_t_obs = tdomain.lb();
    for(double t = tdomain.lb() ; t < tdomain.ub() ; t+=dt)
    {
      if(t - prev_t_obs > 2*dt) // new observation each 2*dt
      {
        // Creating new observation to a random landmark

          ...

        // Adding related observation constraints to the network

          ...
      }

      double contraction_dt = cn.contract_during(iteration_dt);
      usleep(max(0.,iteration_dt-contraction_dt)*1e6); // pause for the animation

      // Display the current slice [x](t)
      fig_map.draw_box(x(max(0.,ibex::previous_float(t))).subvector(0,1));
    }

You should obtain an animation that looks like this:

.. figure:: img/slam_online.png
