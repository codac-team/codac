**********************
Constraint programming
**********************

Tubex is designed to deal with static and dynamical systems that are usually encountered in robotics. 
It stands on constraint programming: a paradigm in which the properties of a solution to be found (*e.g.* the pose of a robot, the location of a landmark) are stated by constraints coming from the equations of the problem.

Then, a solver performs **constraint propagation** on the variables and provides a reliable set of feasible solutions corresponding to the considered problem. In this approach, the user concentrates on *what* is the problem instead of *how* to solve it, thus leaving the computer dealing with the *how*. The strength of this declarative paradigm lies in its simpleness, as it allows one to describe a complex problem without requiring the knowledge of resolution tools coming with specific parameters to choose.

The following figure presents a conceptual view of a constraint propagation.

.. Figure:: /manual/01-introduction/img/cst.png

  In this theoretical view, a domain :math:`\mathbb{X}` depicted in yellow is known to enclose a solution :math:`\mathbf{x}^*` pictured by a red dot and consistent with two constraints :math:`\mathcal{L}_1` and :math:`\mathcal{L}_2`. The estimation of :math:`\mathbf{x}^*` consists in reducing :math:`\mathbb{X}` while satisfying :math:`\mathcal{L}_1` and :math:`\mathcal{L}_2`. After the propagation process, the set of feasible solutions has been reduced by removing vectors that were not consistent with the constraints: we obtain a finer approximation of :math:`\mathbf{x}^*`.

.. When working with finite domains, a propagation technique can be used to simplify a problem. The process is run several times up to a fixed point reached when the domains cannot be reduced anymore. Interval analysis can be efficiently used for this purpose, taking advantage of interval arithmetic and its capacity to preserve any feasible solution.

Variables
---------

The unknown solutions of a system are reals :math:`x`, vectors :math:`\mathbf{x}`, or in the dynamic case: so-called trajectories denoted by :math:`x(\cdot)`, or :math:`\mathbf{x}(\cdot)` in the vector case.

.. admonition:: Dot notation :math:`(\cdot)`

  Note that in the literature, the dot notation :math:`(\cdot)` may be used to represent the independent system variable (time).
  This notation may be chosen in order to clearly distinguish a whole trajectory :math:`\mathbf{x}(\cdot):\mathbb{R}\to\mathbb{R}^n` from a local evaluation: :math:`\mathbf{x}(t)\in\mathbb{R}^n`. Indeed, the time :math:`t` may also be a variable to be estimated.

In the constraint programming approach, the estimation of a variable consists in computing its reliable enclosure set, defined here as an interval of values.
The obtained set is said **reliable**: the resolution guarantees that no solution can be lost during the solving process, according to the equations defining the problem. In other words, a variable :math:`x` to be estimated will surely be enclosed in the resulting set :math:`[x]`.


Domains
-------

As depicted in the figure, a variable is known to be enclosed in some domain :math:`\mathbb{X}` on which we will apply constraints.
In Tubex, the domains are represented by the following sets:

- for reals :math:`x`, domains are intervals :math:`[x]`;
- for vectors :math:`\mathbf{x}`, we define boxes :math:`[\mathbf{x}]`;
- for trajectories :math:`x(\cdot)`, we introduce tubes :math:`[x](\cdot)`.


Constraints
-----------

The approach consists in defining the problem as a set of constraints. They usually come from state equations, numerical models, or measurements.
In mobile robotics, we usually have to deal with:

- non-linear functions: :math:`x_3=\cos\big(x_1+\exp(x_2)\big)`
- differential equations: :math:`\dot{x}(\cdot)=f\big(x(\cdot),u(\cdot)\big)`
- temporal delays: :math:`x(t)=y(t-\tau)`
- time uncertainties: :math:`x(t)=y`, with :math:`t\in[t]`
- *etc.*

The aim of Tubex is to easily deal with these constraints in order to eventually characterize sets of variables compliant with the defined rules.
Each constraint comes from equations or measurements. And each constraint will be applied by means of an operator called **contractor**.


Contractors
-----------

Contractors are operators designed to reduce domains without losing any solution consistent with the related constraint. In Tubex, the contractors act on intervals, boxes and tubes.

.. Figure:: /manual/01-introduction/img/ctc.png

  Implementation of the constraints :math:`\mathcal{L}_1` and :math:`\mathcal{L}_2` of the previous figure by means of respective contractors :math:`\mathcal{C}_1` and :math:`\mathcal{C}_2`. On this theoretical example, the domain :math:`\mathbb{X}` is now a subset of a box :math:`[\mathbf{x}]`, easily representable and contractible.

Tubex already provides a catalog a contractors that one can use to deal with many applications. New contractors can also be designed and embedded in this framework without difficulty.

.. admonition:: Constraint programming *vs* probabilistic methods

  The approach does differ from probabilistic methods in regards of the nature of the estimated solution. Probabilistic methods compute a punctual potential solution – for instance a vector – while in set-membership ones, it is the **set of all feasible solutions** that is evaluated, and thus an infinity of potential solutions.

  Another main distinction lies in the way things are computed: with set-membership methods, estimations are not randomly performed. **Computations are deterministic**: given a set of parameters or inputs, algorithms will always output the same result.


Reliable outputs
----------------

One of the advantages of this set-membership approach is the reliable outputs that are obtained.
By *reliable*, we mean that all sources of uncertainties are taken into account, including:

- model parameter uncertainties
- measurement noise
- uncertainties related to time discretization
- linearization truncations
- approximation of real numbers by floating-point values

The outcomes are intervals and tubes that are guaranteed to contain the solutions of the system.
This is well suited for proof purposes as we always consider worst-case possibilities when delineating the boundaries of the solution sets.

The main drawback however, is that we may obtain large sets that may not be useful to characterize the solutions of the problem. We call this *pessimism*. This can be overcome by reformulating some constraints or by using bisections on sets.
