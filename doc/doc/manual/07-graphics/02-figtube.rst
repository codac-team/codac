.. _sec-manual-figtube:

.. warning::
  
  This part of the documentation is deprecated. Several changes are currently performed on the library.
  A new stable version of Tubex will be released in the coming weeks.

*******
FigTube
*******

.. contents::


**More content coming soon..**


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


Produces:

.. figure:: img/fig_tube.png