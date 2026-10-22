.. _sec-trajectories:

Trajectories
============

A trajectory describes a time-dependent value :math:`x(\cdot):[t_0,t_f]\to\mathbb{R}^n`. Two representations are available:

* :doc:`sampledtraj` stores time/value samples and interpolates linearly between them.
* :doc:`analytictraj` evaluates a known expression of time through an ``AnalyticFunction``.

``SampledTraj`` is useful for numerical signals, while ``AnalyticTraj`` represents trajectories defined by analytical expressions. Both support scalar, vector and matrix trajectories. In Codac, trajectories are intended to represent approximations of solutions or measured signals, while tubes are used to perform guaranteed computations by enclosing uncertainties.


The following pages describe their construction, evaluation, common operations, and input/output:

* :doc:`sampledtraj`
* :doc:`analytictraj`
* :doc:`trajectory_operations`
* :doc:`trajectory_io`
