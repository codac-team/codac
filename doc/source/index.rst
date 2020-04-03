.. tubex-lib documentation master file

##########################################
Tubex: constraint-programming for robotics
##########################################

In a nutshell
=============

Tubex is a library providing tools for **constraint programming** over reals and trajectories. It has many applications in **state estimation** or **robot localization**.

| **What is constraint programming?**
| In this paradigm, users concentrate on the properties of a solution to be found (*e.g.* the pose of a robot, the location of a landmark) by stating **constraints on the variables**. Then, a solver performs constraint propagation on the variables and provides a **reliable** set of feasible solutions corresponding to the considered problem. In this approach, the user concentrates on *what* is the problem instead of *how* to solve it, thus leaving the computer dealing with the *how*. The strength of this declarative paradigm lies in its **simpleness**, as it allows one to describe a complex problem without requiring the knowledge of resolution tools coming with specific parameters to choose.

| **What about mobile robotics?**
| In the field of robotics, complex problems such as **non-linear state estimation**, **delays**, **SLAM** or **kidnapeed robot problems** can be solved in a very few steps. Tubex provides solutions to deal with these problems, that are usually hardly solvable by conventional methods such as particle approaches or Kalman filters.

In a nutshell, Tubex is a high-level **constraint programming framework** providing tools to easily solve a wide range of robotic problems.

.. It is available in C++ and Python.


.. Tubex is a C++ library providing tools for computations over sets of trajectories.
.. It has been designed to deal with **dynamical systems** defined by **non-linear differential equations** and involving constraints such as trajectory evaluations, time uncertainties or **delays**.
.. These constraints are commonly encountered in mobile robotics: Tubex provides solutions to deal with problems that are usually hardly solvable by conventional methods such as particle approaches or Kalman filters.

.. The building blocks of this library are *tubes*, that represent domains for **trajectory** variables (*e.g.* signals evolving with time). When a trajectory :math:`x(\cdot)` comes with uncertainties (due to numerical approximations or errors coming from sensors), a reliable enclosure of it can be made thanks to a tube :math:`[x](\cdot)`.

.. A tube is defined as an interval of two trajectories. Its bounds delineate the **uncertainties** of a dynamical system, thus defining a solution set.
.. Then, any operation related to :math:`x(\cdot)` will be made on its reliable enclosure :math:`[x](\cdot)`, which allows **guaranteed outputs**. These computations stand on interval analysis, a well suited tool that reliably propagates uncertainties.

.. **Mobile robotics** has been the initial motivation of this project: the mathematical tools provided in Tubex come together with robotic applications.

.. Computations stands on the `IBEX library <http://www.ibex-lib.org/>`_ that provides reliable tools for static systems.


Getting started: 2 minutes to Tubex
-----------------------------------

We only have to define **domains** for our variables and a set of **contractors** to implement our constraints.
The core of Tubex stands on a **Contractor Network** representing a solver. 

In a few steps, a problem is solved by

1. Defining the initial domains (boxes, tubes) of our variables (vectors, trajectories)
2. Take contractors from a catalog of already existing operators, provided in the library
3. Add the contractors and domains to the Contractor Network
4. Let the Contractor Network solve the problem 
5. Obtain a reliable set of feasible variables

**For instance.** Let us consider the robotic problem of localization with range-only measurements.
A robot is described by a state vector :math:`\mathbf{x}=\{x_1,x_2,\psi,\vartheta\}^\intercal` depicting its position, its heading and its speed.
It evolves between three landmarks :math:`\mathbf{b}_1`, :math:`\mathbf{b}_2`, :math:`\mathbf{b}_3` and measures distances :math:`y_i` from these points.
The problem is summarized by classical state equations:

.. math::

  \left\{ \begin{array}{l}
    \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),\mathbf{u}(t)\big)\\
    y_i=g\big(\mathbf{x}(t_i),\mathbf{b}_i\big)
  \end{array}\right.

| **First step.**
| Defining domains for our variables.
We have three variables evolving with time: the trajectories :math:`\mathbf{x}(t)`, :math:`\dot{\mathbf{x}}(t)`, :math:`\mathbf{u}(t)`. We define three tubes to enclose them:

.. code-block:: c++
  
  float dt = 0.01;                                  // timestep for tubes accuracy
  Interval tdomain(0, 3);                           // temporal limits [t_0,t_f]

  TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors
  TubeVector xdot(tdomain, dt, 4);                  // 4d tube for derivatives of the states
  TubeVector u(tdomain, dt, 2);                     // 2d tube for inputs of the system

We assume that we have measurements on the headings :math:`\psi(t)` and :math:`\vartheta(t)`, with some bounded uncertainties defined by intervals :math:`[e_\psi]=[-0.01,0.01]`, :math:`[e_\vartheta]=[-0.01,0.01]`:

.. code-block:: c++
  
  x[2] = Tube(measured_psi, dt).inflate(0.01);      // measured_psi is a set of measurements
  x[3] = Tube(measured_speed, dt).inflate(0.01);

Finally, we define the domains for the three observations :math:`(t_i,y_i)` and the position of the landmarks. The distances :math:`y_i` are bounded by the interval :math:`[e_y]=[-0.1,0.1]`.

.. code-block:: c++
  
  Interval e_y(-0.1,0.1);
  vector<Interval> y = {1.9+e_y, 3.6+e_y, 2.8+e_y}; // set of range-only observations
  vector<Vector>   b = {{8,3}, {0,5}, {-2,1}};      // positions of 2d landmarks
  vector<double>   t = {0.3, 1.5, 2.0};             // times of measurements

| **Second step.**
| Defining contractors to deal with the state equations.
We look at the state equations and use contractors to deal with them. The distance function :math:`g(\mathbf{x},\mathbf{b})` between the robot and a landmark, and the evolution function :math:`\mathbf{f}(\mathbf{x},\mathbf{u})=\big(x_4\cos(x_3),x_4\sin(x_3),u_1,u_2\big)` can be handled by custom-built contractors.
Other contractors already exist in the catalog of contractors.

.. code-block:: c++

  CtcEval ctc_eval; // evaluates a trajectory at a given instant
  CtcFunction ctc_dist("a[2]", "b[2]", "d",
                    "d = sqrt((a[0]-b[0])^2+(a[1]-b[1])^2)");
  CtcFunction ctc_f("v[4]", "x[4]", "u[2]",
                   "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])");

| **Third step.**
| Adding the contractors to a network, together with there related domains, is as easy as:

.. code-block:: c++

  ContractorNetwork cn;                             // creating a network
  cn.add(ctc_f, {xdot, x, u});                      // adding the f constraint

  for(int i = 0 ; i < 3 ; i++)                      // for each range-only observation
  {
    IntervalVector& p_i = cn.create_var(IntervalVector(2)); // intermediate variable
    cn.add(ctc_dist, {p_i, b[i], obs[i]});          // adding the g constraint
    cn.add(ctc_eval, {t[i], p_i, x, v});            // link between an observation at t_i,
  }                                                 //   and all states over [t_0,t_f]

| **Fourth step.**
| Solving the problem.

.. code-block:: c++

  cn.contract();

| **Fifth step.**
| Obtain a reliable set of feasible positions: a tube, depicted in blue. The three yellow robots illustrate the three instants of observation. The white line is the unknown truth.

.. figure:: ../img/rangeonly-nox0.png

*You just solved a non-linear state-estimation without knowledge about initial condition.*

In the examples folder of this library, you will find more advanced problems such as delayed systems, Simultaneous Localization And Mapping (SLAM) or data association problems.


User manual
-----------

**User manual in progress**

.. toctree::
   :maxdepth: 2

   01_introduction/index
   02_installation/index
   03_basics/index
   10_references/index
..   04_causal_systems/index
..   05_contractors/index
..   06_looped_systems/index
..   07_graphics/index
..   08_data/index
..   09_slices/index

Further pages will be written soon, presenting *contractors*, *bisections*, *fixed point resolutions*, *graphical tools* and *robotic applications*.

The technical documentation is also available here:

- `API technical documentation <./doxygen/html/annotated.html>`_ 


Support
-------

You can post bug reports and feature requests on the `GitHub repository <https://github.com/SimonRohou/tubex-lib/>`_.



Main related publications
-------------------------

.. |tubint-pdf| replace:: PDF
.. _tubint-pdf: http://simon-rohou.fr/research/tubint/tubint_paper.pdf
.. |tubeval-pdf| replace:: PDF
.. _tubeval-pdf: http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf
.. |loopproof-pdf| replace:: PDF
.. _loopproof-pdf: http://simon-rohou.fr/research/loopproof/loopproof_paper.pdf
.. |robloc-pdf| replace:: PDF
.. _robloc-pdf: http://simon-rohou.fr/research/robloc/robloc_toc.pdf

+--------------------+---------------------------------+------------------------------+-------------------------+------+---------------------+
|                    | Title                           | Author(s)                    | Journal / Editor        | Year | Link                |
+====================+=================================+==============================+=========================+======+=====================+
|                    |                                 |                              |                         |      |                     |
| .. _loopproof:     | Proving the existence of loops  | S. Rohou, P. Franek,         | IJRR                    | 2018 | |loopproof-pdf|_    |
|                    | in robot trajectories           | C. Aubry, L. Jaulin          |                         |      |                     |
| [Rohou,            |                                 |                              |                         |      |                     |
| Franek et al. 2018]|                                 |                              |                         |      |                     |
+--------------------+---------------------------------+------------------------------+-------------------------+------+---------------------+
|                    |                                 |                              |                         |      |                     |
| .. _tubeval:       | Reliable non-linear state       | S. Rohou, L. Jaulin,         | Automatica              | 2018 | |tubeval-pdf|_      |
|                    | estimation involving time       | L. Mihaylova, F. Le Bars,    |                         |      |                     |
| [Rohou et al. 2018]| uncertainties                   | S. M. Veres                  |                         |      |                     |
+--------------------+---------------------------------+------------------------------+-------------------------+------+---------------------+
|                    |                                 |                              |                         |      |                     |
| .. _tubint:        | Guaranteed computation of       | S. Rohou, L. Jaulin,         | RAS                     | 2017 | |tubint-pdf|_       |
|                    | robot trajectories              | L. Mihaylova, F. Le Bars,    |                         |      |                     |
| [Rohou et al. 2017]|                                 | S. M. Veres                  |                         |      |                     |
+--------------------+---------------------------------+------------------------------+-------------------------+------+---------------------+
|                    |                                 |                              |                         |      |                     |
| .. _robloc:        | Reliable robot localization: a  | S. Rohou, L. Jaulin,         | ISTE Ltd, Wiley         | 2019 | |robloc-pdf|_       |
|                    | constraint-programming approach | L. Mihaylova, F. Le Bars,    |                         |      |                     |
| [Rohou et al. 2019]| over dynamical systems          | S. M. Veres                  |                         |      |                     |
+--------------------+---------------------------------+------------------------------+-------------------------+------+---------------------+


Licence and development
-----------------------

This software is under `GNU Lesser General Public License <https://www.gnu.org/copyleft/lgpl.html>`_.

For recent improvements and activities, see the `Github Tubex repository <https://github.com/SimonRohou/tubex-lib>`_.
A Python version (`pyIbex <http://www.ensta-bretagne.fr/desrochers/pyibex>`_ wrapping) is planned.