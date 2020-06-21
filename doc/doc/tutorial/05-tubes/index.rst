.. _sec-tuto-05:

Lesson E: Dynamic localization with asynchronous measurements
=============================================================

Mobile robot motion is usually described by an evolution function :math:`\mathbf{f}` involved in a **differential equation**. The interval state estimation then amounts to estimating the set of **feasible trajectories** that are solutions of this differential equation. The difficulty is to propagate the uncertainties in time, and to consider observations from asynchronous measurements in the same time. We will see that these difficulties are easily handled with constraint propagations on **tubes**.


.. contents:: Content of this lesson


Formalism
---------

We now address the following problem:

.. math:: 

  \left\{ \begin{array}{lll}
  \dot{\mathbf{x}}(t)=\mathbf{f}\big(\mathbf{x}(t),\mathbf{u}(t)\big) &  & \textrm{(evolution equation)}\\
  \mathbf{y}^{i}=\mathbf{g}\big(\mathbf{x}(t_{i})\big) &  & \textrm{(observation equation)}
  \end{array}\right.

We recall that

* :math:`\mathbf{x}\in\mathbb{R}^n` is the state vector
* :math:`\mathbf{u}\in\mathbb{R}^m` is some input vector
* :math:`\mathbf{y}^i\in\mathbb{R}^p` is a measurement made at time :math:`t_i`
* and :math:`\mathbf{f}`, :math:`\mathbf{g}` are respectively *evolution* and *observation* functions, possibly non-linear or uncertain

The :math:`\mathbf{x}` and :math:`\mathbf{u}` values :underline:`continuously` evolve with time. We will now handle them as **trajectories**.


.. rubric:: What is a trajectory

A trajectory :math:`x(\cdot):[t_0,t_f]\to\mathbb{R}` is a set of real values defined over some temporal domain :math:`[t_0,t_f]`, called *t*-domain.
They are aimed at **representing temporal evolutions**. We implement them with the ``Trajectory`` class (or the ``TrajectoryVector`` class for trajectories :math:`\mathbf{x}(\cdot):[t_0,t_f]\to\mathbb{R}^n`).

These trajectories correspond to a new kind of variable in the constraint programming approach. We will consider them together with other values such as reals of vectors, as we did in the previous lessons.


.. rubric:: New type of variables :math:`\implies` new type of domain

Because we will deal with a new kind of variable (trajectories), we also define a new type of domain to handle them with contractors: **tubes**.





Tubes: intervals of trajectories
--------------------------------



Contractors for differential equations
--------------------------------------



Dead-reckoning
--------------