.. _sec-tuto-02:

Static range-only localization
==============================

We now have all the material to compute a solver for state estimation. We deal with the problem of localizing a robot of state :math:`\mathbf{x}\in\mathbb{R}^2` that measures distances :math:`\rho^{(k)}` from three landmarks :math:`\mathbf{b}^{(k)}`, :math:`k\in\{1,2,3\}`. We consider uncertainties on both the measurements and the location of the landmarks: :math:`\rho^{(k)}\in[\rho^{(k)}]` and :math:`\mathbf{b}^{(k)}\in[\mathbf{b}^{(k)}]`. This corresponds to the constraint network presented in the slides of this lesson:

.. math::

  \left\{
  \begin{array}{l}
  \textrm{Variables:}~~ \mathbf{x}, \mathbf{b}^{(1)}, \mathbf{b}^{(2)}, \mathbf{b}^{(3)}, \rho^{(1)}, \rho^{(2)}, \rho^{(3)}\\
  \textrm{Constraints:}~~ \\
  -~ \mathcal{L}_{g}^{(1)}\left(\mathbf{x},\mathbf{b}^{(1)},\rho^{(1)}\right) \\
  -~ \mathcal{L}_{g}^{(2)}\left(\mathbf{x},\mathbf{b}^{(2)},\rho^{(2)}\right) \\
  -~ \mathcal{L}_{g}^{(3)}\left(\mathbf{x},\mathbf{b}^{(3)},\rho^{(3)}\right) \\
  \textrm{Domains:}~~ [\mathbf{x}], [\mathbf{b}^{(1)}], [\mathbf{b}^{(2)}], [\mathbf{b}^{(3)}], [\rho^{(1)}], [\rho^{(2)}], [\rho^{(3)}]
  \end{array}\right.

Where :math:`\mathcal{L}_{g}^{(k)}` is a distance constraint from a landmark :math:`\mathbf{b}^{(k)}`, that links a measurement :math:`\rho^{(k)}` to the state :math:`\mathbf{x}`:

.. math::

  \mathcal{L}_{g}^{(k)}:~\rho^{(k)}=\sqrt{\left(x_1-\mathbf{b}_1^{(k)}\right)^2+\left(x_2-\mathbf{b}_2^{(k)}\right)^2}.

The following code provides a simulation of random landmarks and related range-only measurements:

.. tabs::

  .. code-tab:: c++

    // Truth (unknown pose)
    Vector x_truth({0.,0.,M_PI/6.}); // (x,y,heading)

    // Creating random map of landmarks
    int nb_landmarks = 3;
    IntervalVector map_area(2, Interval(-8.,8.));
    vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

    // The following function generates a set of [range]x[bearing] values
    vector<IntervalVector> v_obs = DataLoader::generate_static_observations(x_truth, v_map, false);

    // Generating range-only observations of these landmarks
    vector<Interval> v_range;
    for(auto& obs : v_obs)
      v_range.push_back(obs[0].inflate(0.1)); // adding uncertainties

  .. code-tab:: py

    # Truth (unknown pose)
    x_truth = [0,0,math.pi/6] # (x,y,heading)

    # Creating random map of landmarks
    map_area = IntervalVector(2, [-8,8])
    v_map = DataLoader.generate_landmarks_boxes(map_area, nb_landmarks = 3)

    # The following function generates a set of [range]x[bearing] values
    v_obs = DataLoader.generate_static_observations(x_truth, v_map, False)

    # Generating range-only observations of these landmarks
    v_range = []
    for obs in v_obs:
      r = obs[0].inflate(0.1) # adding uncertainties
      v_range.append(r)


Finally, the graphical functions are given by:

.. tabs::

  .. code-tab:: c++

    vibes::beginDrawing();

    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 600, 600);

    for(const auto& iv : v_map)
      fig.add_beacon(iv.mid(), 0.2);

    for(int i = 0 ; i < nb_landmarks ; i++)
      fig.draw_ring(v_map[i][0].mid(), v_map[i][1].mid(), v_range[i], "gray");

    fig.draw_vehicle(x_truth, 0.5); // last param: vehicle size
    fig.draw_box(x); // estimated position
    fig.show();

    vibes::endDrawing();

  .. code-tab:: py

    beginDrawing()

    fig = VIBesFigMap("Map")
    fig.set_properties(50, 50, 600, 600)

    for iv in v_map:
      fig.add_beacon(iv.mid(), 0.2)

    for i in range(0,len(v_range)):
      fig.draw_ring(v_map[i][0].mid(), v_map[i][1].mid(), v_range[i], "gray")

    fig.draw_vehicle(x_truth, size=0.5)
    fig.draw_box(x) # estimated position
    fig.show()

    endDrawing()


.. admonition:: Exercise

  1. Before the code related to the graphical part, compute the state estimation of the robot by contracting the box :math:`[\mathbf{x}]=[-\infty,\infty]^2` with a contractor network:
  
  * :math:`[\mathbf{x}]` represents the unknown position of the robot
  * ``v_range`` is the set of bounded measurements :math:`\{[\rho^{(1)}],[\rho^{(2)}],[\rho^{(3)}]\}`
  * ``v_b`` is the set of landmarks with bounded positions :math:`\{[\mathbf{b}^{(1)}],[\mathbf{b}^{(2)}],[\mathbf{b}^{(3)}]\}`

.. figure:: img/final_result.png
  :width: 500px

  Range-only localization: expected result.