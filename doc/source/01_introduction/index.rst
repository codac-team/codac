Introduction
============

Tubex is a C++ numerical library based on **tube arithmetic** and **constraint programming**.
It can be seen as an extension of the `IBEX library <http://www.ibex-lib.org>`_ for dynamical systems.

Variables and domains
---------------------

The unknown solutions of a system are called **trajectories**, denoted by :math:`x(\cdot)` (or :math:`\mathbf{x}(\cdot)` in the vector case).
In the literature, the dot notation :math:`(\cdot)` may be used to represent the independent system variable.
This notation is chosen in order to clearly distinguish a whole trajectory :math:`\mathbf{x}(\cdot):\mathbb{R}\to\mathbb{R}^n` from a local evaluation: :math:`\mathbf{x}(t)\in\mathbb{R}^n`. Indeed, the time :math:`t` may also be a variable to be estimated.

In this approach, the estimation of a trajectory :math:`x(\cdot)` consists in computing its reliable enclosure set, defined as an interval of trajectories: a so-called **tube**, denoted by :math:`[x](\cdot)`.

Constraints
-----------

In the same way as with IBEX, we deal with a system by defining it as a set of **constraints**. In the case of dynamical systems, the constraints can be:

- non-linear functions: :math:`x_3(\cdot)=\cos\big(x_1(\cdot)+\exp(x_2(\cdot))\big)`
- differential equations: :math:`\dot{x}(\cdot)=f\big(x(\cdot),u(\cdot)\big)`
- temporal delays: :math:`x(t)=y(t-\tau)`
- time uncertainties: :math:`x(t)=y`, with :math:`t\in[t]`
- *etc.*

The aim of Tubex is to easily deal with these constraints in order to eventually characterize a tube of trajectories compliant with the defined rules.

Reliable outputs
----------------

One of the advantages of this set-membership approach is the **reliable output** that is obtained.
By *reliable*, we mean that all sources of uncertainty are taken into account, including:

- approximation of real numbers by floating-point numbers
- linearization truncatures
- model parameter uncertainties
- measurement noise
- uncertainties related to time discretization

The outcomes are intervals and tubes that are guaranteed to contain the solutions of the system.
This is well suited for **proof purposes** as we always consider worst-case possibilities when delineating the boundaries of the solution sets.

------------------------------------------------------

The API
-------

The API of Tubex can be broken down into three layers:

- an extended (symbolic-numeric) tube calculator
- a contractor programming library
- a system solver (currently in development, part of the *Contredo* project)

Each usage corresponds to a different layer and each layer is built on top of the previous one. This structure is similar to the one of IBEX, but dedicated to dynamic systems.


.. An extended interval calculator
.. -------------------------------
.. 
.. Tubex allows you to declare symbolically a temporal function and to perform interval computations with it. For example:
.. 
..    Function f(x, y, "sin(x)+exp(t*y)");
.. 
.. defines the "mathematical" object :math:`\left(\begin{array}{c}x(\cdot)\\y(\cdot)\end{array}\right) \mapsto \sin\big(x(t)\big)+\exp\big(t\cdot y(t)\big)`.


------------------------------------------------------

Mobile robotics
---------------

Finally, Tubex proposes various realistic applications based on the tools provided in this library. In particular, several examples present an original way to deal with state estimation and mobile robotics.

Marine robotics and underwater exploration provide challenging academic problems to work on. This application field motivated the development of tools related to tubes, and then the proposal of such library.