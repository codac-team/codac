Handling tubes
==============

Uncertain trajectories can be handled by tubes. This page gives simple operations that can be made on tubes. Next sections will provide concrete applications and show the main advantages of this framework.

Tubes and slices
----------------

In Tubex, tubes are necessarily implemented as lists of slices. The details about this choice are provided thereafter.

More precisely, a tube :math:`[x](\cdot)` with a sampling time :math:`\delta>0` is implemented as a box-valued function which is constant for all :math:`t` inside intervals :math:`[k\delta,k\delta+\delta]`, :math:`k\in\mathbb{N}`.

The box :math:`[k\delta,k\delta+\delta]\times\left[x\right]\left(t_{k}\right)`, with :math:`t_{k}\in[k\delta,k\delta+\delta]`, is called the :math:`k`-th slice of the tube :math:`[x](\cdot)` and is denoted by :math:`[x](k)`. The resulting approximation of a tube encloses :math:`[x^{-}(\cdot),x^{+}(\cdot)]` inside an interval of step functions :math:`[\underline{x^{-}}(\cdot),\overline{x^{+}}(\cdot)]` such that:

.. math::

  \forall t\in[t_0,t_f],~\underline{x^{-}}(t)\leqslant x^{-}(t)\leqslant x^{+}(t)\leqslant\overline{x^{+}}(t)

.. figure:: ../../img/03_04_tube_slices.png

  A tube :math:`[x](\cdot)` represented by a set of :math:`\delta`-width slices. This implementation can be used to enclose signals such as :math:`x^*(\cdot)`.

This implementation then takes rigorously into account floating point precision when building a tube, thanks to reliable numerical libraries such as `GAOL <http://frederic.goualard.net/#research-software>`_.
Further computations involving :math:`[x](\cdot)` will be based on its slices, thus giving an outer approximation of the solution set. 


Creating tubes
--------------

To create a ``Tube`` with a constant codomain:

.. code-block:: c++
  
  Interval domain(0.,10.);
  
  // One slice tubes:
  Tube x1(domain);                                // [0,10]->[-oo,oo]
  Tube x2(domain, Interval(0.,2.));               // [0,10]->[0,2]

  // 100 slices tubes:
  float timestep = 0.1;
  Tube x3(domain, timestep, Interval(0.,2.));     // [0,10]->[0,2]
  Tube x4(domain, timestep, Interval::POS_REALS); // [0,10]->[0,oo]

The ``timestep`` variable defines 
To create a copy of a tube with the same time discretization, use:

.. code-block:: c++
  
  Tube x5(x4);                     // identical tube (100 slices, [0,10]->[0,oo])
  Tube x6(x4, Interval(5.));       // 100 slices, same timestep, but [0,10]->[5]

As tubes are interval of trajectories, a ``Tube`` can be defined from ``Trajectory`` objects:

.. code-block:: c++
  
  Trajectory traj1(domain, tubex::Function("cos(t)"));
  Trajectory traj2(domain, tubex::Function("cos(t)+t/10"));

  Tube x8(traj1, timestep);        // 100 slices tube enclosing cos(t)
  Tube x9(traj1, traj2, timestep); // 100 slices tube defined as [cos(t),cos(t)+t/10]

.. figure:: ../../img/03_04_interval_trajs.png

  Result of tube :math:`[x_9](\cdot)=[\cos(t),\cos(t)+\frac{t}{10}]`, made of 100 slices.

It is also possible to create a tube from an uncertain function:

.. code-block:: c++
  
  Tube x10(domain, timestep/10.,
           tubex::Function("-abs(cos(t)+t/5)+(t/2)*[-0.1,0.1]"));

.. figure:: ../../img/03_04_tube_fnc.png

  Result of tube :math:`[x_{10}](\cdot)` made of 1000 slices.

Finally, a tube can be seen as a union of trajectories. And so the following operations are allowed:

.. code-block:: c++

  float timestep = 0.01;
  Interval domain(0.,10.);

  tubex::Function f("(cos(t) ; cos(t)+t/10 ; sin(t)+t/10 ; sin(t))"); // 4d function
  TrajectoryVector traj(domain, f); // 4d trajectory defined over [0,10]

  // 1d tube [x](.) defined as a union of the 4 trajectories
  Tube x = Tube(traj[0], timestep) | traj[1] | traj[2] | traj[3];

  // Graphics ...
  fig.add_tube(&x, "x");
  fig.add_trajectories(&traj, "trajs"); // plots all components of the TrajectoryVector

Which produces:

.. figure:: ../../img/03_04_union.png


Updates and evaluations
-----------------------

evaluations bornées
functions?
inversion de tube


Operations on sets
------------------

functions?
unions, intersections
tests is_subset, contains (BoolInterval), etc.


Integral computations
---------------------

intégrales avec bornes incertaines ?



The vector case
---------------

.

