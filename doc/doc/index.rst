.. _home-page:

.. tubex-lib documentation master file

..  # with overline, for parts
..  * with overline, for chapters
..  =, for sections
..  -, for subsections
..  ^, for subsubsections
..  ", for paragraphs

##########################################
Tubex: constraint-programming for robotics
##########################################

Tubex is a C++/Python library providing tools for **constraint programming** over reals and trajectories. It has many applications in **state estimation** or **robot localization**.

| **What is constraint programming?**
| In this paradigm, users concentrate on the properties of a solution to be found (*e.g.* the pose of a robot, the location of a landmark) by stating **constraints on the variables**. Then, a solver performs constraint propagation on the variables and provides a reliable set of **feasible solutions** corresponding to the problem. In this approach, the user concentrates on *what* is the problem instead of *how* to solve it, thus leaving the computer dealing with the *how*.

.. The strength of this declarative paradigm lies in its **simpleness**, as it allows one to describe a complex problem without requiring the knowledge of resolution tools coming with specific parameters to choose.

| **What about mobile robotics?**
| In the field of robotics, complex problems such as **non-linear state estimation**, **delays**, **SLAM** or **kidnapped robot problems** can be solved in a very few steps by using constraint programming. Even though the Tubex library is not meant to target only robotics problems, the design of its interface has been largely influenced by the needs of the above class of applications. Tubex provides solutions to deal with these problems, that are usually hardly solvable by conventional methods such as particle approaches or Kalman filters. 

In a nutshell, Tubex is a high-level **constraint programming framework** providing tools to easily solve a wide range of robotic problems.


.. contents:: Contents of this page


.. rubric:: Keywords

.. hlist::
  :columns: 3

  * constraint-programming
  * mobile robotics
  * interval-analysis
  * dynamical-systems
  * tubes
  * localization
  * state-estimation
  * SLAM
  * solver



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
===================================

We only have to define **domains** for our variables and a set of **contractors** to implement our constraints.
The core of Tubex stands on a **Contractor Network** representing a solver. 

In a few steps, a problem is solved by

1. Defining the initial domains (boxes, tubes) of our variables (vectors, trajectories)
2. Take contractors from a catalog of already existing operators, provided in the library
3. Add the contractors and domains to a Contractor Network
4. Let the Contractor Network solve the problem 
5. Obtain a reliable set of feasible variables

| **For instance.**
| Let us consider the robotic problem of localization with range-only measurements. A robot is described by the state vector :math:`\mathbf{x}=\{x_1,x_2,\psi,\vartheta\}^\intercal` depicting its position, its heading and its speed. It evolves between three landmarks :math:`\mathbf{b}_1`, :math:`\mathbf{b}_2`, :math:`\mathbf{b}_3` and measures distances :math:`y_i` from these points.

The problem is defined by classical state equations:

.. math::

  \left\{ \begin{array}{l}
    \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),\mathbf{u}(t)\big)\\
    y_i=g\big(\mathbf{x}(t_i),\mathbf{b}_i\big)
  \end{array}\right.

where :math:`\mathbf{u}(t)` is the input of the system, known with some uncertainties. :math:`\mathbf{f}` and :math:`g` are non-linear functions.


| **First step.**
| Defining domains for our variables.

We have three variables evolving with time: the trajectories :math:`\mathbf{x}(t)`, :math:`\mathbf{v}(t)=\dot{\mathbf{x}}(t)`, :math:`\mathbf{u}(t)`. We define three tubes to enclose them:

.. tabs::

  .. code-tab:: py

    dt = 0.01                                         # timestep for tubes accuracy
    tdomain = Interval(0, 3)                          # temporal limits [t_0,t_f]=[0,3]

    x = TubeVector(tdomain, dt, 4)                    # 4d tube for state vectors
    v = TubeVector(tdomain, dt, 4)                    # 4d tube for derivatives of the states
    u = TubeVector(tdomain, dt, 2)                    # 2d tube for inputs of the system

  .. code-tab:: c++

    float dt = 0.01;                                  // timestep for tubes accuracy
    Interval tdomain(0, 3);                           // temporal limits [t_0,t_f]=[0,3]

    TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors
    TubeVector v(tdomain, dt, 4);                     // 4d tube for derivatives of the states
    TubeVector u(tdomain, dt, 2);                     // 2d tube for inputs of the system

We assume that we have measurements on the headings :math:`\psi(t)` and the speeds :math:`\vartheta(t)`, with some bounded uncertainties defined by intervals :math:`[e_\psi]=[-0.01,0.01]`, :math:`[e_\vartheta]=[-0.01,0.01]`:

.. tabs::

  .. code-tab:: py

    x[2] = Tube(measured_psi, dt).inflate(0.01)       # measured_psi is a set of measurements
    x[3] = Tube(measured_speed, dt).inflate(0.01)

  .. code-tab:: c++

    x[2] = Tube(measured_psi, dt).inflate(0.01);      // measured_psi is a set of measurements
    x[3] = Tube(measured_speed, dt).inflate(0.01);

Finally, we define the domains for the three range-only observations :math:`(t_i,y_i)` and the position of the landmarks. The distances :math:`y_i` are bounded by the interval :math:`[e_y]=[-0.1,0.1]`.

.. tabs::

  .. code-tab:: py

    e_y = Interval(-0.1,0.1)
    y = [Interval(1.9+e_y), Interval(3.6+e_y), \      # set of range-only observations
         Interval(2.8+e_y)]
    b = [[8,3],[0,5],[-2,1]]                          # positions of the three 2d landmarks
    t = [0.3, 1.5, 2.0]                               # times of measurements

  .. code-tab:: c++

    Interval e_y(-0.1,0.1);
    vector<Interval> y = {1.9+e_y, 3.6+e_y, 2.8+e_y}; // set of range-only observations
    vector<Vector>   b = {{8,3}, {0,5}, {-2,1}};      // positions of the three 2d landmarks
    vector<double>   t = {0.3, 1.5, 2.0};             // times of measurements

| **Second step.**
| Defining contractors to deal with the state equations.

The distance function :math:`g(\mathbf{x},\mathbf{b})` between the robot and a landmark corresponds to the ``CtcDist`` contractor provided in the library. The evolution function :math:`\mathbf{f}(\mathbf{x},\mathbf{u})=\big(x_4\cos(x_3),x_4\sin(x_3),u_1,u_2\big)` can be handled by a custom-built contractor:

.. tabs::

  .. code-tab:: py

    ctc_f = CtcFunction(
      Function("v[4]", "x[4]", "u[2]",
               "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"))

  .. code-tab:: c++

    CtcFunction ctc_f(
      Function("v[4]", "x[4]", "u[2]",
               "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"));

| **Third step.**
| Adding the contractors to a network, together with there related domains, is as easy as:

.. tabs::

  .. code-tab:: py

    cn = ContractorNetwork()   # creating a network

    cn.add(ctc_f, [v, x, u])   # adding the f constraint

    for i in range (0,len(y)): # we add the observ. constraint for each range-only measurement

      p = cn.create_dom(IntervalVector(4)) # intermed. variable (state at t_i)

      # Distance constraint: relation between the state at t_i and the ith beacon position
      cn.add(ctc.dist, [cn.subvector(p,0,1), b[i], y[i]])

      # Eval constraint: relation between the state at t_i and all the states over [t_0,t_f]
      cn.add(ctc.eval, [t[i], p, x, v])

  .. code-tab:: c++

    ContractorNetwork cn;        // creating a network
    cn.add(ctc_f, {v, x, u});    // adding the f constraint

    for(int i = 0 ; i < 3 ; i++) // we add the observ. constraint for each range-only measurement
    {
      IntervalVector& p = cn.create_dom(IntervalVector(4)); // intermed. variable (state at t_i)

      // Distance constraint: relation between the state at t_i and the ith beacon position
      cn.add(ctc::dist, {cn.subvector(p,0,1), b[i], y[i]});
      
      // Eval constraint: relation between the state at t_i and all the states over [t_0,t_f]
      cn.add(ctc::eval, {t[i], p, x, v});
    }


| **Fourth step.**
| Solving the problem.

.. tabs::

  .. code-tab:: py

    cn.contract()

  .. code-tab:: c++

    cn.contract();


| **Fifth step.**
| Obtain a reliable set of feasible positions: a tube, depicted in blue. The three yellow robots illustrate the three instants of observation. The white line is the unknown truth.

.. figure:: img/rangeonly-nox0.png

| *You just solved a non-linear state-estimation without knowledge about initial condition.*
| See the full example on Github: `in C++ <https://github.com/SimonRohou/tubex-lib/blob/master/examples/tuto/ex_01_getting_started/tubex_tuto_01.cpp>`_ or `in Python <https://github.com/SimonRohou/tubex-lib/blob/master/examples/tuto/ex_01_getting_started/tubex_tuto_01.py>`_.

In the tutorial and in the examples folder of this library, you will find more advanced problems such as Simultaneous Localization And Mapping (SLAM), data association problems or delayed systems.


User manual
===========

*Want to use Tubex?* The first thing to do is to install the library, or try it online:

.. toctree::
  :maxdepth: 1
 
  /install/02-py-project-online
  /install/01-installation
  /install/02-start-py-project
  /install/03-start-cpp-project

Then you have two options: read the details about the features of Tubex (domains, tubes, contractors, slices, and so on) or jump to the standalone tutorial about how to :ref:`use Tubex for mobile robotics <sec-mainpage-tuto>`, with telling examples.

.. sidebar:: Dependencies
  
  .. rubric:: IBEX

  .. Figure:: img/logo_ibex.jpg
    :align: center

  | Note that Tubex stands on the `IBEX library <http://www.ibex-lib.org/>`_ for interval analysis computations and static contractors on boxes.
  | `Read the IBEX documentation. <http://www.ibex-lib.org/doc/>`_

  .. rubric:: pyIbex

  .. Figure:: img/logo_pyibex.jpg
    :align: center

  | The Python API of Tubex is available thanks to the `pyIbex library <http://benensta.github.io/pyIbex/>`_ providing Python access to the IBEX library.
  | `Read the pyIbex documentation. <http://benensta.github.io/pyIbex/sphinx/index.html>`_


.. toctree:: 
  :maxdepth: 2

  /manual/01-introduction/index
  /manual/02-variables/index
  /manual/03-domains/index
  /manual/04-static-contractors/index
  /manual/05-dynamic-contractors/index
  /manual/06-contractor-network/index
  /manual/07-graphics/index
  /manual/10-dev/index
..  /manual/08-going-further/index
..  /manual/09-extensions/index

.. versionadded:: 3.0.0
   The Contractor Network tool.


.. seealso::

  The `C++ API technical documentation <../api/html/annotated.html>`_ of the library.


.. _sec-mainpage-tuto:

.. sidebar:: ICRA 2020 tutorial

  .. Figure:: img/logo_icra.png
    :align: center

  This tutorial is proposed in the `ICRA 2020 Conference <https://www.icra2020.org/program/workshops-and-tutorials>`_ (International Conference on Robotics and Automation), 8th June -- 5th July.


Tutorial for mobile robotics
============================

The :ref:`following tutorial <sec-tuto-main-page>` is standalone and tells about how to use Tubex for mobile robotic applications, with telling examples, namely:

  * Static range-only localization
  * Static range-and-bearing localization
  * Localization with asynchronous measurements
  * Range-only SLAM
  * Localization by solving data association
  * Real-time state estimation
  * Proving loops in robot trajectories

You can see more on :ref:`the related page <sec-tuto-main-page>`.

.. .. toctree:: 
..   :maxdepth: 1
.. 
..   /tutorial/01-introduction/index
..   /tutorial/02-basics/index
..   /tutorial/03-static-rangeonly-loc/index
..   /tutorial/04-static-loc/index
..   /tutorial/05-dynamic-loc/index
..   /tutorial/06-rangeonly-slam/index
..   /tutorial/07-data-association/index
..   /tutorial/08-realtime-loc/index
..   /tutorial/09-distributed-loc/index
..   /tutorial/10-loop-detections/index


License and support
===================

This software is under `GNU Lesser General Public License <https://www.gnu.org/copyleft/lgpl.html>`_.

For recent improvements and activities, see the `Tubex Github repository <https://github.com/SimonRohou/tubex-lib>`_.
You can post bug reports and feature requests on the `Issues page <https://github.com/SimonRohou/tubex-lib/issues>`_.


Contributors
============

.. hlist::
  :columns: 3

  * `Simon Rohou <http://simon-rohou.fr/research/>`_
  * `Luc Jaulin <https://www.ensta-bretagne.fr/jaulin/>`_
  * `Benoît Desrochers <http://www.ensta-bretagne.fr/desrochers>`_
  * `Gilles Chabert <https://web.imt-atlantique.fr/x-info/gchabe08/>`_
  * Julien Damers
  * Raphael Voges
  * `Thomas Le Mézo <https://www.ensta-bretagne.fr/lemezo/>`_
  * `Cyril Bouvier <http://www.lirmm.fr/~bouvier/index.en.html>`_
  * `Bertrand Neveu <http://imagine.enpc.fr/~neveub/>`_


Main related publications
=========================

.. |polar-pdf| replace:: PDF
.. _polar-pdf: https://www.ensta-bretagne.fr/jaulin/paper_polar.pdf
.. |datasso-pdf| replace:: PDF
.. _datasso-pdf: http://simon-rohou.fr/research/datasso/datasso_paper.pdf
.. |robloc-pdf| replace:: PDF
.. _robloc-pdf: http://simon-rohou.fr/research/robloc/robloc_toc.pdf
.. |loopproof-pdf| replace:: PDF
.. _loopproof-pdf: http://simon-rohou.fr/research/loopproof/loopproof_paper.pdf
.. |tubeval-pdf| replace:: PDF
.. _tubeval-pdf: http://simon-rohou.fr/research/tubeval/tubeval_paper.pdf
.. |tubint-pdf| replace:: PDF
.. _tubint-pdf: http://simon-rohou.fr/research/tubint/tubint_paper.pdf
.. |loopdetect-pdf| replace:: PDF
.. _loopdetect-pdf: https://www.ensta-bretagne.fr/jaulin/paper_loop.pdf


====  =================  ===============================  =========================  ========================
2020  |datasso-pdf|_     Set-membership state             S. Rohou, B. Desrochers,   ICRA Conference         
                         estimation by solving            L. Jaulin                                          
                         data association                                                                    
                                                                                                             
2019  |robloc-pdf|_      Reliable robot localization: a   S. Rohou, L. Jaulin,       ISTE Ltd, Wiley         
                         constraint-programming approach  L. Mihaylova, F. Le Bars,                          
                         over dynamical systems           S. M. Veres                                        
                                                                                                             
2018  |loopproof-pdf|_   Proving the existence of loops   S. Rohou, P. Franek,       International Journal   
                         in robot trajectories            C. Aubry, L. Jaulin        of Robotics Research    
                                                                                                             
2018  |tubeval-pdf|_     Reliable non-linear state        S. Rohou, L. Jaulin,       Automatica              
                         estimation involving time        L. Mihaylova, F. Le Bars,                          
                         uncertainties                    S. M. Veres                                        
                                                                                                             
2017  |tubint-pdf|_      Guaranteed computation of        S. Rohou, L. Jaulin,       Robotics and Autonomous 
                         robot trajectories               L. Mihaylova, F. Le Bars,  Systems                 
                                                          S. M. Veres                                        
                                                                                                             
2016  |polar-pdf|_       A Minimal contractor for the     B. Desrochers,             Engineering             
                         Polar equation                   L. Jaulin                  Applications of         
                                                                                     Artificial Intelligence 
                                                                                                             
2013  |loopdetect-pdf|_  Loop detection of mobile robots  C. Aubry, R. Desmare,      Automatica              
                         using interval analysis          L. Jaulin                                          
====  =================  ===============================  =========================  ========================


How to cite this project?
=========================

We suggest the following BibTeX template to cite Tubex in scientific discourse:

.. code-block:: none

  @misc{tubex,
     author = {Simon Rohou and others},
     year = {2017},
     note = {http://simon-rohou.fr/research/tubex-lib/},
     title = {The Tubex library -- Constraint-programming for robotics}
  }