.. _sec-manual-figtube:

******************************************
Display tubes and trajectories w.r.t. time
******************************************

This page presents a tool for displaying tubes and trajectories objects with respect to time.

.. contents::



The class ``VIBesFigTube``
--------------------------

| ``VIBesFigTube`` creates a figure for displaying temporal objects such as trajectories and tubes. The *x*-axis of this 2d figure is time.
| This class inherits from ``VIBesFig``: the methods provided in ``VIBesFig`` can be used here.

To create and show a :math:`600\times300` figure located at :math:`100,100`:

.. tabs::
      
  .. code-tab:: py

    fig = VIBesFigTube("Tube")
    fig.set_properties(100, 100, 600, 300)

    # add graphical items here ...

    fig.show()

  .. code-tab:: c++

    VIBesFigTube fig("Tube");
    fig.set_properties(100, 100, 600, 300);

    // add graphical items here ...

    fig.show();

To add temporal objects (before the method ``.show()``), use the methods ``.add_tube()`` or ``.add_trajectory()`` with three arguments:

* the first argument refers to the object to be shown
* the second one is the name of the object in the view
* (optional) the third argument defines the color of the object (``edge_color[fill_color]``)

.. tabs::
      
  .. code-tab:: py

    fig.add_tube(tube_x, "x", "blue[yellow]")
    fig.add_trajectory(traj_x, "x*", "red")
    # Where tube_x and traj_x are respectively Tube and Trajectory objects

  .. code-tab:: c++

    fig.add_tube(&tube_x, "x", "blue[yellow]");
    fig.add_trajectory(&traj_x, "x*", "red");
    // Where tube_x and traj_x are respectively Tube and Trajectory objects
    // In C++, the first argument is a pointer to the object to display


.. rubric:: Draw slices

The ``.show()`` method accepts a boolean argument. If set to true, then the slices of the tubes will be displayed (instead of a polygon envelope).
For instance:

.. tabs::
      
  .. code-tab:: py

    dt = 0.1
    tdomain = Interval(0,10)
     
    traj = TrajectoryVector(tdomain, TFunction("(sin(t) ; cos(t) ; cos(t)+t/10)"))
    y = Tube(traj[0], dt)
    x = Tube(traj[1], traj[2], dt)
     
    beginDrawing()
     
    fig = VIBesFigTube("Tube")
    fig.set_properties(100, 100, 600, 300)
    fig.add_tube(x, "x", "#376D7C[lightGray]")
    fig.add_tube(y, "y", "#7C4837[lightGray]")
    fig.add_trajectories(traj, "trajs")
    fig.show(True)
     
    endDrawing()

  .. code-tab:: cpp

    float dt = 0.1;
    Interval tdomain(0,10);
  
    TrajectoryVector traj(tdomain, TFunction("(sin(t) ; cos(t) ; cos(t)+t/10)"));
    Tube y(traj[0], dt);
    Tube x(traj[1], traj[2], dt);
  
    vibes::beginDrawing();
  
    VIBesFigTube fig("Tube");
    fig.set_properties(100, 100, 600, 300);
    fig.add_tube(&x, "x", "#376D7C[lightGray]");
    fig.add_tube(&y, "y", "#7C4837[lightGray]");
    fig.add_trajectories(&traj, "trajs");
    fig.show(true);
  
    vibes::endDrawing();

which produces:

.. figure:: img/fig_tube_slices.png


.. rubric:: Draw a set of objects on the same figure

Several objects can be drawn on the same figure with successive calls to the ``.add_...()`` methods. It is also possible to project all components of a vector object on the same figure with ``.add_tubes()`` or ``.add_trajectories()``.

The following code:

.. tabs::
      
  .. code-tab:: py

    dt = 0.001
    tdomain = Interval(0,10)

    f = TFunction("(cos(t) ; cos(t)+t/10 ; sin(t)+t/10 ; sin(t))") # 4d temporal function
    traj = TrajectoryVector(tdomain, f) # 4d trajectory defined over [0,10]

    # 1d tube [x](·) defined as a union of the 4 trajectories
    x = Tube(traj[0], dt) | traj[1] | traj[2] | traj[3]

    beginDrawing()

    fig = VIBesFigTube("Tube")
    fig.set_properties(100, 100, 600, 300)
    fig.add_tube(x, "x", "#376D7C[lightGray]")
    fig.add_trajectories(traj, "trajs")
    fig.show()

    endDrawing()

  .. code-tab:: c++

    float dt = 0.001;
    Interval tdomain(0.,10.);

    TFunction f("(cos(t) ; cos(t)+t/10 ; sin(t)+t/10 ; sin(t))"); // 4d temporal function
    TrajectoryVector traj(tdomain, f); // 4d trajectory defined over [0,10]

    // 1d tube [x](·) defined as a union of the 4 trajectories
    Tube x = Tube(traj[0], dt) | traj[1] | traj[2] | traj[3];

    vibes::beginDrawing();

    VIBesFigTube fig("Tube");
    fig.set_properties(100, 100, 600, 300);
    fig.add_tube(&x, "x", "#376D7C[lightGray]");
    fig.add_trajectories(&traj, "trajs");
    fig.show();

    vibes::endDrawing();


produces:

.. figure:: img/fig_tube.png


.. admonition:: Technical documentation

  See the C++ API documentation of this class:

  * `VIBesFigTube <../../../api/html/classcodac_1_1_v_i_bes_fig_tube.html>`_


The class ``VIBesFigTubeVector``
--------------------------------

**More content coming soon.**