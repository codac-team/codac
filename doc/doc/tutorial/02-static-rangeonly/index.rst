.. _sec-tuto-02:

Lesson B: Static range-only localization
========================================

We now have all the material to compute a solver for state estimation.

.. contents:: Content of this lesson


Problem statement
-----------------

Our goal is to deal with the problem of localizing a robot of state :math:`\mathbf{x}\in\mathbb{R}^2` that measures distances :math:`d^{k}` from three landmarks :math:`\mathbf{b}^{k}`, :math:`k\in\{1,2,3\}`. We consider uncertainties on both the measurements and the location of the landmarks, which means that :math:`d^{k}\in[d^{k}]` and :math:`\mathbf{b}^{k}\in[\mathbf{b}^{k}]`. For now, the robot does not move.

.. figure:: img/prior_result.png
  :width: 500px

  The :math:`\mathbf{b}^{k}` landmarks are depicted in orange while range-only measurements with bounded uncertainties are drawn by rings.

Our problem corresponds to the following Constraint Network:

.. math::

  \left\{
  \begin{array}{l}
  \textrm{Variables:}~~ \mathbf{x}, \mathbf{b}^{1}, \mathbf{b}^{2}, \mathbf{b}^{3}, d^{1}, d^{2}, d^{3}\\
  \textrm{Constraints:}~~ \\
  -~ \mathcal{L}_{\textrm{dist}}^{1}\left(\mathbf{x},\mathbf{b}^{1},d^{1}\right) \\
  -~ \mathcal{L}_{\textrm{dist}}^{2}\left(\mathbf{x},\mathbf{b}^{2},d^{2}\right) \\
  -~ \mathcal{L}_{\textrm{dist}}^{3}\left(\mathbf{x},\mathbf{b}^{3},d^{3}\right) \\
  \textrm{Domains:}~~ [\mathbf{x}], [\mathbf{b}^{1}], [\mathbf{b}^{2}], [\mathbf{b}^{3}], [d^{1}], [d^{2}], [d^{3}]
  \end{array}\right.

This formalization can be seen in the literature and summarizes the problem in terms of variables, constraints on the variables, and domains for their sets of feasible values. As explained in the previous section, we will represent domains by means of intervals and boxes (interval vectors). In addition, constraints will be applied with contractors.


The :math:`\mathcal{L}_{\textrm{dist}}^{k}` is the distance constraint from a landmark :math:`\mathbf{b}^{k}`. It links a measurement :math:`d^{k}` to the state :math:`\mathbf{x}` with:

.. math::

  \mathcal{L}_{\textrm{dist}}^{k}:~d^{k}=\sqrt{\left(x_1-b_1^{k}\right)^2+\left(x_2-b_2^{k}\right)^2}.


Solving the problem
-------------------

The following code provides a simulation of random landmarks and related range-only measurements:

.. tabs::

  .. code-tab:: py

    from tubex_lib import *
    from pyibex import *
    import math

    # Truth (unknown pose)
    x_truth = [0,0,math.pi/6] # (x,y,heading)

    # Creating random map of landmarks
    map_area = IntervalVector(2, [-8,8])
    v_b = DataLoader.generate_landmarks_boxes(map_area, nb_landmarks = 3)

    # The following function generates a set of [range]x[bearing] values
    v_obs = DataLoader.generate_static_observations(x_truth, v_b, False)

    # We keep range-only observations from v_obs, and add uncertainties
    v_d = []
    for obs in v_obs:
      d = obs[0].inflate(0.1) # adding uncertainties: [-0.1,0.1]
      v_d.append(d)

    # Set of feasible positions for x: x ϵ [-∞,∞]×[-∞,∞]
    x = IntervalVector(2) # this is equivalent to: IntervalVector([[-oo,oo],[-oo,oo]])

    # ...

  .. code-tab:: c++

    #include <tubex.h>
    
    using namespace std;
    using namespace tubex;
    
    int main()
    {
      // Truth (unknown pose)
      Vector x_truth({0.,0.,M_PI/6.}); // (x,y,heading)

      // Creating random map of landmarks
      int nb_landmarks = 3;
      IntervalVector map_area(2, Interval(-8.,8.));
      vector<IntervalVector> v_b =
          DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

      // The following function generates a set of [range]x[bearing] values
      vector<IntervalVector> v_obs =
          DataLoader::generate_static_observations(x_truth, v_b, false);

      // We keep range-only observations from v_obs, and add uncertainties
      vector<Interval> v_d;
      for(auto& obs : v_obs)
        v_d.push_back(obs[0].inflate(0.1)); // adding uncertainties: [-0.1,0.1]

      // Set of feasible positions for x: x ϵ [-∞,∞]×[-∞,∞]
      IntervalVector x(2);

      // ...


Finally, the graphical functions are given by:

.. tabs::

  .. code-tab:: py

    # ...

    beginDrawing()

    fig = VIBesFigMap("Map")
    fig.set_properties(50, 50, 600, 600)

    for b in v_b:
      fig.add_beacon(b.mid(), 0.2)

    for i in range(0,len(v_d)):
      fig.draw_ring(v_b[i][0].mid(), v_b[i][1].mid(), v_d[i], "gray")

    fig.draw_vehicle(x_truth, size=0.7)
    fig.draw_box(x) # estimated position
    fig.show()

    endDrawing()

  .. code-tab:: c++

      // ...

      vibes::beginDrawing();

      VIBesFigMap fig("Map");
      fig.set_properties(50, 50, 600, 600);

      for(const auto& b : v_b)
        fig.add_beacon(b.mid(), 0.2);

      for(int i = 0 ; i < nb_landmarks ; i++)
        fig.draw_ring(v_b[i][0].mid(), v_b[i][1].mid(), v_d[i], "gray");

      fig.draw_vehicle(x_truth, 0.7); // last param: vehicle size
      fig.draw_box(x); // estimated position
      fig.show();

      vibes::endDrawing();
    }


.. admonition:: Exercise

  **B.1.** Before the code related to the graphical part, compute the state estimation of the robot by contracting the box :math:`[\mathbf{x}]` initialized to :math:`[-\infty,\infty]^2` with a Contractor Network:
  
  * :math:`[\mathbf{x}]` represents the unknown 2d position of the robot
  * ``v_d`` is the set of bounded measurements :math:`\{[d^{1}],[d^{2}],[d^{3}]\}`
  * ``v_b`` is the set of landmarks with bounded positions :math:`\{[\mathbf{b}^{1}],[\mathbf{b}^{2}],[\mathbf{b}^{3}]\}`

  For this, you can use the :math:`\mathcal{C}_{\textrm{dist}}` contractor you defined in the previous section.

  You should obtain a figure similar to this:

  .. figure:: img/final_result.png
    :width: 500px

    Range-only localization: expected result. The black painted box represents the set of feasible positions for our robot.


  Due to the randomness of the landmarks, the geometry is sometimes bad and does not allow an accurate contraction: symmetrical solutions are possible, and the box :math:`[\mathbf{x}]` encloses them all. You can execute the code several times to see how the geometry influences the result.


How does it work?
-----------------

.. rubric:: Combining the constraints

The Contractor Network you have defined managed the contractions provided by the three :math:`\mathcal{C}_{\textrm{dist}}` contractors.

Each constraint alone would not allow a good contraction, since it would contract :math:`[\mathbf{x}]` to the box enclosing the circle corresponding to :math:`d^k`. It is the intersection of the three constraints that makes the approach powerful.

.. rubric:: Fixed point resolution

There are **dependencies between the constraints** that all act on the same variable :math:`\mathbf{x}`.
The Contractor Network has then made a **fixed point resolution method** for solving the problem.

When a :math:`\mathcal{C}_{\textrm{dist}}` contractor reduces the box :math:`[\mathbf{x}]`, it may raise new contraction possibilities coming from the other constraints. It becomes interesting to call again the previous contractors (start another iteration) in order to take benefit from any contraction. An iterative resolution process is then used, where the contractors are called until a fixed point has been reached. By *fixed point* we mean that none of the domains :math:`[\mathbf{x}]` and :math:`[d^{k}]` has been contracted during a complete iteration.

The following figure provides the synoptic of this state estimation, performed by the Contractor Network. In this example, constraints have been propagated over 7 iterations in a very short amount of time.

.. figure:: img/fixedpoint_animation.gif
  :width: 500px




Submit your exercise
--------------------

| That's about all for this first week!
| You can submit your answers for the questions of Lessons A and B to the `MOOC platform <https://mooc.ensta-bretagne.fr/course/view.php?id=7>`_ so that we can evaluate them for the diploma.

Next lessons will introduce other concepts of constraint propagation. We will also see how to build our own contractor.